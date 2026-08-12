#include "Apple.h"
#include <cstdlib>

namespace SnakeGame
{
    void InitApple(Apple& apple, const sf::Texture& appleTexture)
    {
        apple.sprite.setTexture(appleTexture);
        apple.position = Position2D(rand() % GRID_WIDTH, rand() % GRID_HEIGHT);
        apple.sprite.setPosition(
            apple.position.x * CELL_SIZE,
            apple.position.y * CELL_SIZE
        );
    }

    void RespawnApple(Apple& apple, const Snake& snake)
    {
        bool isOnSnake = false;
        do
        {
            apple.position = Position2D(rand() % GRID_WIDTH, rand() % GRID_HEIGHT);
            isOnSnake = false;

            for (const auto& segment : snake.segments)
            {
                if (segment == apple.position)
                {
                    isOnSnake = true;
                    break;
                }
            }
        } while (isOnSnake);

        apple.sprite.setPosition(
            apple.position.x * CELL_SIZE,
            apple.position.y * CELL_SIZE
        );
    }

    void DrawApple(const Apple& apple, sf::RenderWindow& window)
    {
        window.draw(apple.sprite);
    }
}