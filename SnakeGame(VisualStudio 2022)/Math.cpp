#include "Math.h"
#include "Constants.h"

namespace SnakeGame
{
    sf::Vector2i GetRandomGridPosition()
    {
        sf::Vector2i result;
        result.x = rand() % GRID_WIDTH;
        result.y = rand() % GRID_HEIGHT;
        return result;
    }
}