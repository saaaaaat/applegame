#include "Game.h"
#include <cassert>
#include <algorithm>

namespace ApplesGame
{
    void GenerateLeaderboard(Game& game)
    {
        game.leaderboard.clear();
        game.leaderboardMap.clear();

        // список
        std::string botNames[] = {"Alice", "Bob", "Carol", "Dave", "Eve" ,"Frank", "Grace", "Henry", "Ivy", "Jack"};

        //  4 записей со случайными именами и очками
        for (int i = 0; i < LEADERBOARD_SIZE-1; ++i)
        {
            LeaderboardEntry record;
            record.name = botNames[i % 10];
            record.score = rand() % (MAX_SCORE - MIN_SCORE + 1) + MIN_SCORE;

            game.leaderboard.push_back(record);

            
            game.leaderboardMap[record.name] = record.score;
        }

        game.isLeaderboardGenerated = true;
    }
    

   
    void SortLeaderboard(Game&game)
    {
        
        for (int i = 1; i < (int)game.leaderboard.size(); ++i)
        {
            LeaderboardEntry key = game.leaderboard[i];
            int j = i - 1;

           
            while (j >= 0 && game.leaderboard[j].score < key.score)
            {
                game.leaderboard[j + 1] = game.leaderboard[j];
                j=j-1;
            }
            game.leaderboard[j + 1] = key;


        }
    }
    
    void UpdateLeaderboard(Game& game)
    {
        
        game.leaderboardMap["Player"] = game.numEatenApples;

      
        game.leaderboard.clear();

      
        for (const auto& pair : game.leaderboardMap)
        {
            LeaderboardEntry record;
            record.name = pair.first;
            record.score = pair.second;
            game.leaderboard.push_back(record);
        }

       
        SortLeaderboard(game);

        // оставляем только 5
        if (game.leaderboard.size() > LEADERBOARD_SIZE)
        {
            game.leaderboard.resize(LEADERBOARD_SIZE);
        }
    }

  
    void DrawLeaderboard(Game& game, sf::RenderWindow& window)
    {
        std::string text = "   LEADERBOARD    \n";
        for (int i = 0; i < LEADERBOARD_SIZE;++i)
        {
            text += std::to_string(i + 1) + ". ";
            text += game.leaderboard[i].name;

            int dotsCount = 20 - game.leaderboard[i].name.length();
            for (int j = 0; j < dotsCount;++j)
            {
                text += "_";

            }
            text += " " + std::to_string(game.leaderboard[i].score) + "\n";
        }
        text += "\n              \n";
        text += " Press R to restart";

        game.leaderboardText.setString(text);
        game.leaderboardText.setFont(game.font);
        game.leaderboardText.setCharacterSize(24);
        game.leaderboardText.setFillColor(sf::Color::Yellow);
        game.leaderboardText.setPosition(SCREEN_WIDTH / 2.f - 200.f, SCREEN_HEIGHT / 2.f - 150.f);
        window.draw(game.leaderboardText);
    }

    void RestartGame(Game& game)
    {
        game.isGameFinished = false;
        game.isLeaderboardShown = false;
        game.timeSinceGameFinish = 0.f;


        if (game.apples != nullptr)
        {
            delete[] game.apples;
            game.apples = nullptr;
        }

        game.numApples = rand() % 81 + 10;
        game.apples = new Apple[game.numApples];
        assert(game.apples != nullptr);

        for (int i = 0; i < game.numApples;++i)
        {
            InitApple(game.apples[i], game);
        }
        game.applesCountText.setString("Apples on field :" + std::to_string(game.numApples));

        StartPlayingState(game);

    }


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

                    if (game.numApples == 0)
                    {
                        StartGameoverState(game);
                        return;
                    }
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
        game.isGameOverShow = true;
        game.timeSinceGameFinish = 0.f;
        game.gameOverSound.play();
        game.gameOverScoreText.setString("Your scores: " + std::to_string(game.numEatenApples));

        UpdateLeaderboard(game);
      
    }

    void UpdateGameoverState(Game& game, float deltaTime)
    {
        
        if ( game.isLeaderboardShown && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
          
            sf::sleep(sf::milliseconds(300));
            RestartGame(game);
            return;
        }

        if (game.isGameOverShow)
        {

            game.timeSinceGameFinish += deltaTime;
            game.background.setFillColor(sf::Color::Red);


            if (game.timeSinceGameFinish >= PAUSE_LENGTH)
            {
                game.isGameOverShow = false;
                game.isLeaderboardShown = true;
                game.background.setFillColor(sf::Color::Black);

            }
            return;
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
        game.isLeaderboardShown = false;
        game.isLeaderboardGenerated = false;

        GenerateLeaderboard(game);
        SortLeaderboard(game);

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

        //генерируем таблицу
        game.leaderboardText.setFont(game.font);
        game.leaderboardText.setCharacterSize(24);
        game.leaderboardText.setFillColor(sf::Color::White);

     
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

        if (game.isLeaderboardShown)
        {
            window.draw(game.background);
            DrawLeaderboard(game, window);
            window.draw(game.restartHintText);
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
        if (game.isGameOverShow)
        {
            window.draw(game. gameOverText);
            window.draw(game.gameOverScoreText);
        }
        else if (!game.isGameFinished && !game.isLeaderboardShown)
        {
            window.draw(game.scoreText);
            window.draw(game.applesCountText);
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