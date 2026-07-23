
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Game.h"

int main()
{
    using namespace ApplesGame;

    
    int seed = (int)time(nullptr);
    srand(seed);

    // создание окна
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples game!");

    // инициализация игры
    Game game;
    InitGame(game);

    //  deltaTime
    sf::Clock gameClock;
    float lastTime = gameClock.getElapsedTime().asSeconds();

  
    while (window.isOpen())
    {
       
        sf::sleep(sf::milliseconds(16));

        // вычисление deltaTime
        float currentTime = gameClock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // обработка событий
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                break;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                break;
            }
        }

       
        window.clear();

        if (game.isModeSelection)
        {
            // обновление выбора режима
            UpdateModeSelection(game);

            //  экран выбора
            DrawModeSelection(game, window);
        }
        else
        {
           
            UpdateGame(game, deltaTime);

       
            DrawGame(game, window);
        }

      
        window.display();
    }

    // деинициализация
    DeinializeGame(game);

    return 0;
}