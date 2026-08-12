#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Math.h"
#include "Snake.h"

namespace SnakeGame
{
    struct Apple
    {
        Position2D position;
        sf::Sprite sprite;
    };

    void InitApple(Apple& apple, const sf::Texture& appleTexture);
    void RespawnApple(Apple& apple, const Snake& snake);
    void DrawApple(const Apple& apple, sf::RenderWindow& window);
}