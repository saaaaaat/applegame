#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Math.h"

namespace SnakeGame
{
    // направления движения
    enum class Direction
    {
        Up,
        Down,
        Left,
        Right,
        None
    };

    
    struct SnakeTextures
    {
        sf::Texture headUp;
        sf::Texture headDown;
        sf::Texture headLeft;
        sf::Texture headRight;

        sf::Texture bodyHorizontal;
        sf::Texture bodyVertical;
        sf::Texture bodyTopLeft;
        sf::Texture bodyTopRight;
        sf::Texture bodyBottomLeft;
        sf::Texture bodyBottomRight;

        sf::Texture tailUp;
        sf::Texture tailDown;
        sf::Texture tailLeft;
        sf::Texture tailRight;

        sf::Texture apple;
    };

    struct Snake
    {
        std::vector<Position2D> segments;  
        std::vector<sf::Sprite> sprites;   
        Direction direction = Direction::Right;
        Direction nextDirection = Direction::Right;
        bool isGrowing = false;
    };

    // функции
    void LoadSnakeTextures(SnakeTextures& textures);
    void InitSnake(Snake& snake);
    bool SetSnakeDirection(Snake& snake, Direction direction);
    void UpdateSnake(Snake& snake);
    void GrowSnake(Snake& snake);
    bool CheckSelfCollision(const Snake& snake);
    bool CheckWallCollision(const Snake& snake);
    void UpdateSnakeSprites(Snake& snake, const SnakeTextures& textures);
    void DrawSnake(const Snake& snake, sf::RenderWindow& window);
}