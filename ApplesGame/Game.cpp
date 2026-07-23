#include "Game.h"
#include <cassert>
#include <algorithm>

namespace ApplesGame
{
    
    void UpdateModeSelection(Game& game)
    {
        //клавиши для выбора режима " конечные яблоки"
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            
            if (game.gameModeMask & MODE_FINITE_APPLES)
            {
                game.gameModeMask = game.gameModeMask & ~MODE_FINITE_APPLES;
            }
            else
            {
                game.gameModeMask = game.gameModeMask & ~MODE_INFINITE_APPLES;
                game.gameModeMask = game.gameModeMask | MODE_FINITE_APPLES;
            }
            sf::sleep(sf::milliseconds(200)); // приостанавливаем програму
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))//бескрнечные яблоки
        {
            if (game.gameModeMask & MODE_INFINITE_APPLES)
            {
                game.gameModeMask = game.gameModeMask & ~MODE_INFINITE_APPLES;
            }
            else
            {
                game.gameModeMask = game.gameModeMask & ~MODE_FINITE_APPLES;
                game.gameModeMask = game.gameModeMask | MODE_INFINITE_APPLES;
            }
            sf::sleep(sf::milliseconds(200));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))// ускорение
        {
            if (game.gameModeMask & MODE_ACCELERATION)
            {
                game.gameModeMask = game.gameModeMask & ~MODE_ACCELERATION;
            }
            else
            {
                game.gameModeMask = game.gameModeMask & ~MODE_NO_ACCELERATION;
                game.gameModeMask = game.gameModeMask | MODE_ACCELERATION;
            }
            sf::sleep(sf::milliseconds(200));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))//без ускорения
        {
            if (game.gameModeMask & MODE_NO_ACCELERATION)
            {
                game.gameModeMask = game.gameModeMask & ~MODE_NO_ACCELERATION;
            }
            else
            {
                game.gameModeMask = game.gameModeMask & ~MODE_ACCELERATION;
                game.gameModeMask = game.gameModeMask | MODE_NO_ACCELERATION;
            }
            sf::sleep(sf::milliseconds(200));
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
           
            game.isModeSelection = false;

            // создаем яблоки
            if (game.apples != nullptr)
            {
                delete[] game.apples;
                game.apples = nullptr;
            }

            game.numApples = rand() % 81 + 10;
            game.apples = new Apple[game.numApples];
            assert(game.apples != nullptr);

            for (int i = 0; i < game.numApples; ++i)
            {
                InitApple(game.apples[i], game);
            }

            game.applesCountText.setString("Apples on field: " + std::to_string(game.numApples));

            StartPlayingState(game);
            sf::sleep(sf::milliseconds(200));
        }
    }

   
    void DrawModeSelection(Game& game, sf::RenderWindow& window)
    {
        // информация о режимах
        std::string modeText = " SELECT GAME MODE \n\n";
        modeText += "Press 1: Finite apples   ";
        modeText += (game.gameModeMask & MODE_FINITE_APPLES) ? "[X]\n" : "[ ]\n";

        modeText += "Press 2: Infinite apples ";
        modeText += (game.gameModeMask & MODE_INFINITE_APPLES) ? "[X]\n" : "[ ]\n";

        modeText += "Press 3: Acceleration    ";
        modeText += (game.gameModeMask & MODE_ACCELERATION) ? "[X]\n" : "[ ]\n";

        modeText += "Press 4: No acceleration ";
        modeText += (game.gameModeMask & MODE_NO_ACCELERATION) ? "[X]\n" : "[ ]\n";

        modeText += "\nPress  space to start\n";
        

        // текст
        game.modeSelectionText.setString(modeText);
        game.modeSelectionText.setFont(game.font);
        game.modeSelectionText.setCharacterSize(24);
        game.modeSelectionText.setFillColor(sf::Color::White);
        game.modeSelectionText.setPosition(SCREEN_WIDTH / 2.f - 220.f,SCREEN_HEIGHT / 2.f - 150.f);

    
        window.draw(game.modeSelectionText);
    }

    void StartPlayingState(Game& game)
    {
        // инициализация игрока
        SetPlayerPosition(game.player, { SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
        SetPlayerSpeed(game.player, INITIAL_SPEED);
        SetPlayerDirection(game.player, PlayerDirection::Right);

        // инициализация яблок
        for (int i = 0; i < game.numApples; ++i)
        {
            SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
        }

        // инициализация камней
        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            SetRockPosition(game.rocks[i], GetRandomPositionInRectangle(game.screenRect));
        }

        
        game.numEatenApples = 0;
        game.isGameFinished = false;
        game.timeSinceGameFinish = 0;

        // обновление текстов
        game.scoreText.setString("Apples eaten: " + std::to_string(game.numEatenApples));
        game.applesCountText.setString("Apples on field: " + std::to_string(game.numApples));
    }

    void UpdatePlayingState(Game& game, float deltaTime)
    {
        // управление
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            SetPlayerDirection(game.player, PlayerDirection::Right);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            SetPlayerDirection(game.player, PlayerDirection::Up);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            SetPlayerDirection(game.player, PlayerDirection::Left);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            SetPlayerDirection(game.player, PlayerDirection::Down);
        }

        UpdatePlayer(game.player, deltaTime);

        // коллизия с яблоками с учётом режимов
        for (int i = 0; i < game.numApples; ++i)
        {
            if (DoShapesCollide(GetPlayerCollider(game.player), GetAppleCollider(game.apples[i])))
            {
                //  режим "конечные яблоки"
                if (game.gameModeMask & MODE_FINITE_APPLES)
                {
                    
                    std::swap(game.apples[i], game.apples[game.numApples - 1]);
                    game.numApples = game.numApples -1;
                    game.applesCountText.setString("Apples on field: " + std::to_string(game.numApples));
                    i = i -1;
                }
                //  "бесконечные яблоки"
                else if (game.gameModeMask & MODE_INFINITE_APPLES)
                {
                    
                    SetApplePosition(game.apples[i], GetRandomPositionInRectangle(game.screenRect));
                }

                
                ++game.numEatenApples;
                game.eatAppleSound.play();
                game.scoreText.setString("Apples eaten: " + std::to_string(game.numEatenApples));

                // "ускорение"
                if (game.gameModeMask & MODE_ACCELERATION)
                {
                    SetPlayerSpeed(game.player, GetPlayerSpeed(game.player) + ACCELERATION);
                }
            }
        }

        // rоллизии с камнями
        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            if (DoShapesCollide(GetPlayerCollider(game.player), GetRockCollider(game.rocks[i])))
            {
                StartGameoverState(game);
            }
        }

        // выход за границы
        if (!DoShapesCollide(GetPlayerCollider(game.player), game.screenRect))
        {
            StartGameoverState(game);
        }
    }

    void StartGameoverState(Game& game)
    {
        game.isGameFinished = true;
        game.timeSinceGameFinish = 0.f;
        game.gameOverSound.play();
        game.gameOverScoreText.setString("Your scores: " + std::to_string(game.numEatenApples));
    }

    void UpdateGameoverState(Game& game, float deltaTime)
    {
        if (game.timeSinceGameFinish <= PAUSE_LENGTH)
        {
            game.timeSinceGameFinish += deltaTime;
            game.background.setFillColor(sf::Color::Red);
        }
        else
        {
            game.background.setFillColor(sf::Color::Black);

            // возвращаемся к выбору режима
            game.isModeSelection = true;
            game.isGameFinished = false;
        }
    }

    void InitGame(Game& game)
    {
        // ресурсы
        assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "Player.png"));
        assert(game.appleTexture.loadFromFile(RESOURCES_PATH + "Apple.png"));
        assert(game.rockTexture.loadFromFile(RESOURCES_PATH + "Rock.png"));
        assert(game.eatAppleSoundBuffer.loadFromFile(RESOURCES_PATH + "AppleEat.wav"));
        assert(game.gameOverSoundBuffer.loadFromFile(RESOURCES_PATH + "Death.wav"));
        assert(game.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Bold.ttf"));

        // игровое поля
        game.screenRect = { 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT };

        //  режим по умолчанию
        game.isModeSelection = true;
        game.gameModeMask = MODE_INFINITE_APPLES | MODE_ACCELERATION;

        // инициализация игрока
        InitPlayer(game.player, game);

        // инициализация камней
        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            InitRock(game.rocks[i], game);
        }

        //инициализация фона
        game.background.setSize(sf::Vector2f(game.screenRect.size.x, game.screenRect.size.y));
        game.background.setFillColor(sf::Color::Black);
        game.background.setPosition(0.f, 0.f);

        // инициализация звуков
        game.eatAppleSound.setBuffer(game.eatAppleSoundBuffer);
        game.gameOverSound.setBuffer(game.gameOverSoundBuffer);

        // инициализация текста с количеством яблок
        game.applesCountText.setFont(game.font);
        game.applesCountText.setCharacterSize(20);
        game.applesCountText.setFillColor(sf::Color::Yellow);
        game.applesCountText.setPosition(20.f, 40.f);
        game.applesCountText.setString("Apples on field: 0");

        // инициализация текста счёта
        game.scoreText.setFont(game.font);
        game.scoreText.setCharacterSize(20);
        game.scoreText.setFillColor(sf::Color::White);
        game.scoreText.setPosition(20.f, 10.f);

        //  "Game Over"
        game.gameOverText.setFont(game.font);
        game.gameOverText.setCharacterSize(100);
        game.gameOverText.setFillColor(sf::Color::White);
        game.gameOverText.setPosition(SCREEN_WIDTH / 2.f - 200.f, SCREEN_HEIGHT / 2.f - 50.f);
        game.gameOverText.setString("Game Over");

        // финальный счёт
        game.gameOverScoreText.setFont(game.font);
        game.gameOverScoreText.setCharacterSize(30);
        game.gameOverScoreText.setFillColor(sf::Color::White);
        game.gameOverScoreText.setString("Your score: 0");
        game.gameOverScoreText.setPosition(SCREEN_WIDTH / 2.f - 100.f, SCREEN_HEIGHT / 2.f + 50.f);

        //  выбор режима
        game.modeSelectionText.setFont(game.font);
        game.modeSelectionText.setCharacterSize(24);
        game.modeSelectionText.setFillColor(sf::Color::White);

     
        game.apples = nullptr;
        game.numApples = 0;
    }

    void UpdateGame(Game& game, float deltaTime)
    {
        if (game.isModeSelection)
        {
            
            return;
        }
        else if (!game.isGameFinished)
        {
            UpdatePlayingState(game, deltaTime);
        }
        else
        {
            UpdateGameoverState(game, deltaTime);
        }
    }

    void DrawGame(Game& game, sf::RenderWindow& window)
    {
        if (game.isModeSelection)
        {
            // отрисовка режимов
            DrawModeSelection(game, window);
            return;
        }

        // отрисовка фона
        window.draw(game.background);

        // отрисовка объектов
        DrawPlayer(game.player, window);

        for (int i = 0; i < game.numApples; ++i)
        {
            DrawApple(game.apples[i], window);
        }

        for (int i = 0; i < NUM_ROCKS; ++i)
        {
            DrawRock(game.rocks[i], window);
        }

        // отрисовка текстов
        if (!game.isGameFinished)
        {
            window.draw(game.scoreText);
            window.draw(game.applesCountText);
        }
        else
        {
            window.draw(game.gameOverText);
            window.draw(game.gameOverScoreText);
        }
    }

    void DeinializeGame(Game& game)
    {
        if (game.apples != nullptr)
        {
            delete[] game.apples;
            game.apples = nullptr;
            game.numApples = 0;
        }
    }
}