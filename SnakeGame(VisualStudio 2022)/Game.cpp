#include "Game.h"
#include <cassert>
#include <cstdlib>

namespace SnakeGame
{
    //запись в файл
    void InitGame(Game& game)
    {
        
        LoadSnakeTextures(game.textures);

        // иницилизация змейки
        InitSnake(game.snake);
        UpdateSnakeSprites(game.snake, game.textures);

        // ин. яблока
        InitApple(game.apple, game.textures.apple);

        // ин. фонов
        game.menuBackground.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        game.menuBackground.setFillColor(sf::Color::Black);
        game.menuBackground.setPosition(0.f, 0.f);

        game.gameBackground.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        game.gameBackground.setFillColor(sf::Color(144, 180, 120));
        game.gameBackground.setPosition(0.f, 0.f);

        // загрузка звуков
        assert(game.eatSoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav"));
        assert(game.deathSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav"));

        game.eatSound.setBuffer(game.eatSoundBuffer);
        game.deathSound.setBuffer(game.deathSoundBuffer);

        // шрифт
        assert(game.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Bold.ttf"));

        // ин.состояний
        game.score = 0;
        game.isGameOver = false;
        game.moveTimer = 0.f;
        game.isMenuActive = true;
        game.isModsActive = false;
        game.isLeaderboardActive = false;
        game.isDifficultyActive = false;
        game.selectedMenuItem = 0;
        game.selectedModItem = 0;
        game.selectedDifficultyItem = 0;

       
        game.modNoGrowth = false;
        game.modFiniteApples = false;
        game.modSoundOn = true;
        game.modAppleCount = 20;

        
        game.difficulty = 1;

        // таблица рекордов
        if (!DeserializeLeaderboard(game))
        {
            GenerateLeaderboard(game);
        }

        //  ин. текстов
        game.scoreText.setFont(game.font);
        game.scoreText.setCharacterSize(24);
        game.scoreText.setFillColor(sf::Color::White);
        game.scoreText.setPosition(20.f, 10.f);
        game.scoreText.setString("Score: " + std::to_string(game.score));

        // яблоки
        game.applesLeftText.setFont(game.font);
        game.applesLeftText.setCharacterSize(24);
        game.applesLeftText.setFillColor(sf::Color::Yellow);
        game.applesLeftText.setPosition(20.f, 40.f);
        game.applesLeftText.setString("Apples: infinite");

        // конец игры
        game.gameOverText.setFont(game.font);
        game.gameOverText.setCharacterSize(60);
        game.gameOverText.setFillColor(sf::Color::Red);
        game.gameOverText.setString("GAME OVER");
        sf::FloatRect gameOverBounds = game.gameOverText.getLocalBounds();
        game.gameOverText.setPosition(
            SCREEN_WIDTH / 2.f - gameOverBounds.width / 2.f,
            SCREEN_HEIGHT / 2.f - 100.f
        );

        // рестарт
        game.restartHintText.setFont(game.font);
        game.restartHintText.setCharacterSize(24);
        game.restartHintText.setFillColor(sf::Color::Yellow);
        game.restartHintText.setString("Press R to restart");
        sf::FloatRect restartBounds = game.restartHintText.getLocalBounds();
        game.restartHintText.setPosition(
            SCREEN_WIDTH / 2.f - restartBounds.width / 2.f,
            SCREEN_HEIGHT / 2.f + 20.f
        );

        // меню
        game.menuHintText.setFont(game.font);
        game.menuHintText.setCharacterSize(24);
        game.menuHintText.setFillColor(sf::Color::Yellow);
        game.menuHintText.setString("Press M to menu");
        sf::FloatRect menuBounds = game.menuHintText.getLocalBounds();
        game.menuHintText.setPosition(
            SCREEN_WIDTH / 2.f - menuBounds.width / 2.f,
            SCREEN_HEIGHT / 2.f + 60.f
        );

        
        InitMenuTexts(game);
        InitModsTexts(game);
        InitLeaderboardTexts(game);
        InitDifficultyTexts(game);
    }

     
    void HandleInput(Game& game)
    {
        if (game.isMenuActive)
        {
            UpdateMenu(game);
            return;
        }

        if (game.isModsActive)
        {
            UpdateMods(game);
            return;
        }

        if (game.isLeaderboardActive)
        {
            UpdateLeaderboardMenu(game);
            return;
        }

        if (game.isDifficultyActive)
        {
            UpdateDifficulty(game);
            return;
        }

        if (game.isGameOver)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                RestartGame(game);
                sf::sleep(sf::milliseconds(200));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                game.isGameOver = false;
                
                game.isMenuActive = true;
                game.selectedMenuItem = 0;
                sf::sleep(sf::milliseconds(200));
            }
            return;
        }

        Direction newDirection = Direction::None;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            newDirection = Direction::Up;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            newDirection = Direction::Down;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            newDirection = Direction::Left;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            newDirection = Direction::Right;
        }

        if (newDirection != Direction::None)
        {
            bool success = SetSnakeDirection(game.snake, newDirection);

            if (!success)
            {
                return;
            }
        }
    }

    
    void UpdateGame(Game& game, float deltaTime)
    {
        if (game.isMenuActive) return;
        if (game.isModsActive) return;
        if (game.isLeaderboardActive) return;
        if (game.isDifficultyActive) return;
        if (game.isGameOver) return;

        float currentSpeed = NORMAL_SPEED;
        if (game.difficulty == 0) currentSpeed = EASY_SPEED;
        else if (game.difficulty == 1) currentSpeed = NORMAL_SPEED;
        else if (game.difficulty == 2) currentSpeed = HARD_SPEED;

        game.moveTimer += deltaTime;

        if (game.moveTimer >= currentSpeed)
        {
            game.moveTimer = 0.f;

            UpdateSnake(game.snake);

            if (CheckWallCollision(game.snake) || CheckSelfCollision(game.snake))
            {
                if (game.modSoundOn)
                {
                    game.deathSound.play();
                }
                game.isGameOver = true;
                UpdateLeaderboard(game);
                return;
            }

            if (game.snake.segments[0] == game.apple.position)
            {
                if (!game.modNoGrowth)
                {
                    GrowSnake(game.snake);
                }

                game.score++;
                game.scoreText.setString("Score: " + std::to_string(game.score));

                if (game.modSoundOn)
                {
                    game.eatSound.play();
                }

                if (game.modFiniteApples)
                {
                    game.modAppleCount--;
                    game.applesLeftText.setString("Apples left: " + std::to_string(game.modAppleCount));

                    if (game.modAppleCount <= 0)
                    {
                        game.isGameOver = true;
                        game.gameOverText.setString("YOU WIN!");
                        sf::FloatRect winBounds = game.gameOverText.getLocalBounds();
                        game.gameOverText.setPosition(
                            SCREEN_WIDTH / 2.f - winBounds.width / 2.f,
                            SCREEN_HEIGHT / 2.f - 100.f
                        );
                        UpdateLeaderboard(game);
                        return;
                    }
                }

                RespawnApple(game.apple, game.snake);
            }

            UpdateSnakeSprites(game.snake, game.textures);
        }
    }

    // отрисвока
    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        if (game.isMenuActive)
        {
            DrawMenu(game, window);
            return;
        }

        if (game.isModsActive)
        {
            DrawMods(game, window);
            return;
        }

        if (game.isLeaderboardActive)
        {
            DrawLeaderboard(game, window);
            return;
        }

        if (game.isDifficultyActive)
        {
            DrawDifficulty(game, window);
            return;
        }

        window.draw(game.gameBackground);

        DrawApple(game.apple, window);
        DrawSnake(game.snake, window);

        window.draw(game.scoreText);
        window.draw(game.applesLeftText);

        if (game.isGameOver)
        {
            window.draw(game.gameOverText);
            window.draw(game.restartHintText);
            window.draw(game.menuHintText);
        }
    }

    
    void RestartGame(Game& game)
    {
        InitSnake(game.snake);
        UpdateSnakeSprites(game.snake, game.textures);
        RespawnApple(game.apple, game.snake);
        game.score = 0;
        game.isGameOver = false;
        game.moveTimer = 0.f;
        game.scoreText.setString("Score: 0");

        game.gameOverText.setString("GAME OVER");
        sf::FloatRect gameOverBounds = game.gameOverText.getLocalBounds();
        game.gameOverText.setPosition(
            SCREEN_WIDTH / 2.f - gameOverBounds.width / 2.f,
            SCREEN_HEIGHT / 2.f - 100.f
        );

        if (game.modFiniteApples)
        {
            game.modAppleCount = game.modInitialAppleCount;
            game.applesLeftText.setString("Apples left: " + std::to_string(game.modAppleCount));
        }
        else
        {
            game.applesLeftText.setString("Apples: infinite");
        }
    }
}