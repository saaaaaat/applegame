#include <SFML/Graphics.hpp>
#include <ctime>
#include "Game.h"

int main()
{
    using namespace SnakeGame;

    srand((int)time(nullptr));

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Snake Game!");
    window.setFramerateLimit(60);

    Game game;
    InitGame(game);

    sf::Clock clock;
    float lastTime = clock.getElapsedTime().asSeconds();

    while (window.isOpen())
    {
       
        if (game.isMenuActive &&
            game.selectedMenuItem == 3 &&
            sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            window.close();
            break;
        }

        float currentTime = clock.getElapsedTime().asSeconds();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
            }
        }

        HandleInput(game);
        UpdateGame(game, deltaTime);

        window.clear();
        DrawGame(game, window);
        window.display();
    }

    return 0;
}