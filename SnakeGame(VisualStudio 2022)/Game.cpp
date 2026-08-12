#include "Game.h"
#include <cassert>
#include <cstdlib>
#include <fstream>  

namespace SnakeGame
{
   //запись в файл
    bool SerializeLeaderboard(const Game& game)
    {
        std::ofstream file(RECORDS_FILE);
        if (file.is_open())
        {
            for (const auto& record : game.leaderboard)
            {
                file << record.name << " " << record.score << std::endl;
            }
            file.close();
            return true;
        }
        return false;
    }

    // чтение из файла
    bool DeserializeLeaderboard(Game& game)
    {
        std::ifstream file(RECORDS_FILE);
        if (file.is_open())
        {
            game.leaderboard.clear();

            std::string name;
            int score;
            while (file >> name >> score)
            {
                Record record;
                record.name = name;
                record.score = score;
                game.leaderboard.push_back(record);
            }
            file.close();
            return true;
        }
        return false;
    }

    // генерация таблици с рекордами
    void GenerateLeaderboard(Game& game)
    {
        game.leaderboard.clear();

        
        std::string botNames[] = { "Alice", "Bob", "Carol", "Dave" };

        for (int i = 0; i < 4; ++i)
        {
            Record record;
            record.name = botNames[i];
            record.score = rand() % 81 + 10;
            game.leaderboard.push_back(record);
        }

        // игрок
        Record playerRecord;
        playerRecord.name = "Player";
        playerRecord.score = 0;
        game.leaderboard.push_back(playerRecord);

        // распределение в порядке убывания
        for (int i = 0; i < (int)game.leaderboard.size() - 1; ++i)
        {
            for (int j = 0; j < (int)game.leaderboard.size() - i - 1; ++j)
            {
                if (game.leaderboard[j].score < game.leaderboard[j + 1].score)
                {
                    std::swap(game.leaderboard[j], game.leaderboard[j + 1]);
                }
            }
        }

        game.isLeaderboardGenerated = true;
        SerializeLeaderboard(game);
    }

    void UpdateLeaderboard(Game& game)
    {
        // обноление для очков игроков
        for (auto& record : game.leaderboard)
        {
            if (record.name == "Player")
            {
                record.score = game.score;
                break;
            }
        }

        // сортировка полсе окончания игры
        for (int i = 0; i < (int)game.leaderboard.size() - 1; ++i)
        {
            for (int j = 0; j < (int)game.leaderboard.size() - i - 1; ++j)
            {
                if (game.leaderboard[j].score < game.leaderboard[j + 1].score)
                {
                    std::swap(game.leaderboard[j], game.leaderboard[j + 1]);
                }
            }
        }

        SerializeLeaderboard(game);
    }

// иницилизация
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
        game.isGameActive = false;
        game.selectedMenuItem = 0;
        game.selectedModItem = 0;

        game.modNoGrowth = false;
        game.modFiniteApples = false;
        game.modSoundOn = true;
        game.modAppleCount = 20;

        // таблица рекордов
        if (!DeserializeLeaderboard(game))
        {
            GenerateLeaderboard(game);
        }
        else
        {
            game.isLeaderboardGenerated = true;
        }
        //  ин.текстов
        // счет
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

        // начальное меню
        game.titleText.setFont(game.font);
        game.titleText.setCharacterSize(64);
        game.titleText.setFillColor(sf::Color::Red);
        game.titleText.setString("SNAKE GAME");
        sf::FloatRect titleBounds = game.titleText.getLocalBounds();
        game.titleText.setPosition(SCREEN_WIDTH / 2.f - titleBounds.width / 2.f, 80.f);

        game.startGameText.setFont(game.font);
        game.startGameText.setCharacterSize(32);
        game.startGameText.setFillColor(sf::Color::Yellow);
        game.startGameText.setString("> Start Game");
        sf::FloatRect startBounds = game.startGameText.getLocalBounds();
        game.startGameText.setPosition(SCREEN_WIDTH / 2.f - startBounds.width / 2.f, 200.f);

        game.modsText.setFont(game.font);
        game.modsText.setCharacterSize(32);
        game.modsText.setFillColor(sf::Color::White);
        game.modsText.setString("Mods");
        sf::FloatRect modsBounds = game.modsText.getLocalBounds();
        game.modsText.setPosition(SCREEN_WIDTH / 2.f - modsBounds.width / 2.f, 258.f);

        game.leaderboardText.setFont(game.font);
        game.leaderboardText.setCharacterSize(32);
        game.leaderboardText.setFillColor(sf::Color::White);
        game.leaderboardText.setString("Leaderboard");
        sf::FloatRect lbBounds = game.leaderboardText.getLocalBounds();
        game.leaderboardText.setPosition(SCREEN_WIDTH / 2.f - lbBounds.width / 2.f, 316.f);

        game.exitGameText.setFont(game.font);
        game.exitGameText.setCharacterSize(32);
        game.exitGameText.setFillColor(sf::Color::White);
        game.exitGameText.setString("Exit Game");
        sf::FloatRect exitBounds = game.exitGameText.getLocalBounds();
        game.exitGameText.setPosition(SCREEN_WIDTH / 2.f - exitBounds.width / 2.f, 374.f);

        // модификации
        game.modsTitleText.setFont(game.font);
        game.modsTitleText.setCharacterSize(48);
        game.modsTitleText.setFillColor(sf::Color::White);
        game.modsTitleText.setString("-- MODS -- ");
        sf::FloatRect modsTitleBounds = game.modsTitleText.getLocalBounds();
        game.modsTitleText.setPosition(SCREEN_WIDTH / 2.f - modsTitleBounds.width / 2.f, 80.f);

        game.modNoGrowthText.setFont(game.font);
        game.modNoGrowthText.setCharacterSize(28);
        game.modNoGrowthText.setFillColor(sf::Color::White);
        game.modNoGrowthText.setString("[ ] Disable snake growth");
        sf::FloatRect modNoGrowthBounds = game.modNoGrowthText.getLocalBounds();
        game.modNoGrowthText.setPosition(SCREEN_WIDTH / 2.f - modNoGrowthBounds.width / 2.f, 190.f);

        game.modFiniteApplesText.setFont(game.font);
        game.modFiniteApplesText.setCharacterSize(28);
        game.modFiniteApplesText.setFillColor(sf::Color::White);
        game.modFiniteApplesText.setString("[ ] Finite apples");
        sf::FloatRect modFiniteBounds = game.modFiniteApplesText.getLocalBounds();
        game.modFiniteApplesText.setPosition(SCREEN_WIDTH / 2.f - modFiniteBounds.width / 2.f, 248.f);

        game.modSoundText.setFont(game.font);
        game.modSoundText.setCharacterSize(28);
        game.modSoundText.setFillColor(sf::Color::White);
        game.modSoundText.setString("[X] Sound ON");
        sf::FloatRect modSoundBounds = game.modSoundText.getLocalBounds();
        game.modSoundText.setPosition(SCREEN_WIDTH / 2.f - modSoundBounds.width / 2.f, 306.f);

        game.modBackText.setFont(game.font);
        game.modBackText.setCharacterSize(28);
        game.modBackText.setFillColor(sf::Color::Yellow);
        game.modBackText.setString("> Back");
        sf::FloatRect modBackBounds = game.modBackText.getLocalBounds();
        game.modBackText.setPosition(
            SCREEN_WIDTH / 2.f - modBackBounds.width / 2.f,
            440.f
        );

        // таблица реордов
        game.leaderboardTitleText.setFont(game.font);
        game.leaderboardTitleText.setCharacterSize(48);
        game.leaderboardTitleText.setFillColor(sf::Color::White);
        game.leaderboardTitleText.setString("=== LEADERBOARD ===");
        sf::FloatRect lbTitleBounds = game.leaderboardTitleText.getLocalBounds();
        game.leaderboardTitleText.setPosition(SCREEN_WIDTH / 2.f - lbTitleBounds.width / 2.f, 60.f);

        game.leaderboardEntriesText.setFont(game.font);
        game.leaderboardEntriesText.setCharacterSize(28);
        game.leaderboardEntriesText.setFillColor(sf::Color::White);
        game.leaderboardEntriesText.setPosition(SCREEN_WIDTH / 2.f - 150.f, 140.f);

        game.leaderboardBackText.setFont(game.font);
        game.leaderboardBackText.setCharacterSize(28);
        game.leaderboardBackText.setFillColor(sf::Color::Yellow);
        game.leaderboardBackText.setString("> Back");
        sf::FloatRect lbBackBounds = game.leaderboardBackText.getLocalBounds();
        game.leaderboardBackText.setPosition(
            SCREEN_WIDTH / 2.f - lbBackBounds.width / 2.f,
            520.f
        );
    }

    // меню
    void UpdateMenu(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (game.selectedMenuItem > 0)
            {
                game.selectedMenuItem--;
                sf::sleep(sf::milliseconds(150));
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (game.selectedMenuItem < 3)
            {
                game.selectedMenuItem++;
                sf::sleep(sf::milliseconds(150));
            }
        }

        // обновление цветов
        if (game.selectedMenuItem == 0)
        {
            game.startGameText.setFillColor(sf::Color::Yellow);
            game.startGameText.setString("> Start Game");
            game.modsText.setFillColor(sf::Color::White);
            game.modsText.setString("Mods");
            game.leaderboardText.setFillColor(sf::Color::White);
            game.leaderboardText.setString("Leaderboard");
            game.exitGameText.setFillColor(sf::Color::White);
            game.exitGameText.setString("Exit Game");
        }
        else if (game.selectedMenuItem == 1)
        {
            game.startGameText.setFillColor(sf::Color::White);
            game.startGameText.setString("Start Game");
            game.modsText.setFillColor(sf::Color::Yellow);
            game.modsText.setString("> Mods");
            game.leaderboardText.setFillColor(sf::Color::White);
            game.leaderboardText.setString("Leaderboard");
            game.exitGameText.setFillColor(sf::Color::White);
            game.exitGameText.setString("Exit Game");
        }
        else if (game.selectedMenuItem == 2)
        {
            game.startGameText.setFillColor(sf::Color::White);
            game.startGameText.setString("Start Game");
            game.modsText.setFillColor(sf::Color::White);
            game.modsText.setString("Mods");
            game.leaderboardText.setFillColor(sf::Color::Yellow);
            game.leaderboardText.setString("> Leaderboard");
            game.exitGameText.setFillColor(sf::Color::White);
            game.exitGameText.setString("Exit Game");
        }
        else
        {
            game.startGameText.setFillColor(sf::Color::White);
            game.startGameText.setString("Start Game");
            game.modsText.setFillColor(sf::Color::White);
            game.modsText.setString("Mods");
            game.leaderboardText.setFillColor(sf::Color::White);
            game.leaderboardText.setString("Leaderboard");
            game.exitGameText.setFillColor(sf::Color::Yellow);
            game.exitGameText.setString("> Exit Game");
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            if (game.selectedMenuItem == 0)  
            {
                game.isMenuActive = false;
                game.isGameActive = true;
                RestartGame(game);
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedMenuItem == 1)  
            {
                game.isMenuActive = false;
                game.isModsActive = true;
                game.selectedModItem = 0;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedMenuItem == 2)  
            {
                game.isMenuActive = false;
                game.isLeaderboardActive = true;
                sf::sleep(sf::milliseconds(200));
            }
        }
    }

    void DrawMenu(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.menuBackground);
        window.draw(game.titleText);
        window.draw(game.startGameText);
        window.draw(game.modsText);
        window.draw(game.leaderboardText);
        window.draw(game.exitGameText);
    }

    //настройки
    void UpdateMods(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (game.selectedModItem > 0)
            {
                game.selectedModItem--;
                sf::sleep(sf::milliseconds(150));
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (game.selectedModItem < 3)
            {
                game.selectedModItem++;
                sf::sleep(sf::milliseconds(150));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (game.selectedModItem == 0)  // рост змеи
            {
                game.modNoGrowth = !game.modNoGrowth;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedModItem == 1)  // яблоки конечны
            {
                game.modFiniteApples = !game.modFiniteApples;
                if (game.modFiniteApples)
                {
                    game.modAppleCount = rand() % 91 + 10;
                    game.applesLeftText.setString("Apples left: " + std::to_string(game.modAppleCount));
                }
                else
                {
                    game.applesLeftText.setString("Apples: infinite");
                }
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedModItem == 2)  // звук
            {
                game.modSoundOn = !game.modSoundOn;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedModItem == 3)  // выход
            {
                game.isModsActive = false;
                game.isMenuActive = true;
                sf::sleep(sf::milliseconds(200));
            }
        }

        // обновление текстов
        if (game.modNoGrowth)
        {
            game.modNoGrowthText.setString("[X] Disable snake growth");
        }
        else
        {
            game.modNoGrowthText.setString("[ ] Disable snake growth");
        }

        if (game.modFiniteApples)
        {
            game.modFiniteApplesText.setString("[X] Finite apples (" + std::to_string(game.modAppleCount) + ")");
        }
        else
        {
            game.modFiniteApplesText.setString("[ ] Finite apples");
        }

        if (game.modSoundOn)
        {
            game.modSoundText.setString("[X] Sound ON");
        }
        else
        {
            game.modSoundText.setString("[ ] Sound OFF");
        }

        // обновление цветов
        if (game.selectedModItem == 0)
        {
            game.modNoGrowthText.setFillColor(sf::Color::Yellow);
            game.modFiniteApplesText.setFillColor(sf::Color::White);
            game.modSoundText.setFillColor(sf::Color::White);
            game.modBackText.setFillColor(sf::Color::White);
            game.modBackText.setString("Back");
        }
        else if (game.selectedModItem == 1)
        {
            game.modNoGrowthText.setFillColor(sf::Color::White);
            game.modFiniteApplesText.setFillColor(sf::Color::Yellow);
            game.modSoundText.setFillColor(sf::Color::White);
            game.modBackText.setFillColor(sf::Color::White);
            game.modBackText.setString("Back");
        }
        else if (game.selectedModItem == 2)
        {
            game.modNoGrowthText.setFillColor(sf::Color::White);
            game.modFiniteApplesText.setFillColor(sf::Color::White);
            game.modSoundText.setFillColor(sf::Color::Yellow);
            game.modBackText.setFillColor(sf::Color::White);
            game.modBackText.setString("Back");
        }
        else
        {
            game.modNoGrowthText.setFillColor(sf::Color::White);
            game.modFiniteApplesText.setFillColor(sf::Color::White);
            game.modSoundText.setFillColor(sf::Color::White);
            game.modBackText.setFillColor(sf::Color::Yellow);
            game.modBackText.setString("> Back");
        }
    }

    void DrawMods(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.menuBackground);
        window.draw(game.modsTitleText);
        window.draw(game.modNoGrowthText);
        window.draw(game.modFiniteApplesText);
        window.draw(game.modSoundText);
        window.draw(game.modBackText);
    }

 
    // таблица рекордов
    void UpdateLeaderboardMenu(Game& game)
    {
        std::string entries = "\n";
        for (int i = 0; i < (int)game.leaderboard.size(); ++i)
        {
            entries += std::to_string(i + 1) + ". ";
            entries += game.leaderboard[i].name;

            int dotsCount = 20 - game.leaderboard[i].name.length();
            for (int j = 0; j < dotsCount; ++j)
            {
                entries += ".";
            }

            entries += " " + std::to_string(game.leaderboard[i].score) + "\n\n";
        }

        game.leaderboardEntriesText.setString(entries);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            game.isLeaderboardActive = false;
            game.isMenuActive = true;
            sf::sleep(sf::milliseconds(200));
        }
    }

    void DrawLeaderboard(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.menuBackground);
        window.draw(game.leaderboardTitleText);
        window.draw(game.leaderboardEntriesText);
        window.draw(game.leaderboardBackText);
    }

    //игра
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
                game.isGameActive = false;
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
                if (game.modSoundOn)
                {
                    game.deathSound.play();
                }
                game.isGameOver = true;
                UpdateLeaderboard(game);
            }
        }
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        if (game.isMenuActive) return;
        if (game.isModsActive) return;
        if (game.isLeaderboardActive) return;
        if (game.isGameOver) return;

        game.moveTimer += deltaTime;

        if (game.moveTimer >= MOVE_INTERVAL)
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
            game.applesLeftText.setString("Apples left: " + std::to_string(game.modAppleCount));
        }
        else
        {
            game.applesLeftText.setString("Apples: infinite");
        }
    }
}