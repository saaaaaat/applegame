#include "Snake.h"
#include <cassert>

namespace SnakeGame
{
    // загрузка текстур
    void LoadSnakeTextures(SnakeTextures& textures)
    {
        // голова
        assert(textures.headUp.loadFromFile(RESOURCES_PATH + "head_up.png"));
        assert(textures.headDown.loadFromFile(RESOURCES_PATH + "head_down.png"));
        assert(textures.headLeft.loadFromFile(RESOURCES_PATH + "head_left.png"));
        assert(textures.headRight.loadFromFile(RESOURCES_PATH + "head_right.png"));

        // тело
        assert(textures.bodyHorizontal.loadFromFile(RESOURCES_PATH + "body_horizontal.png"));
        assert(textures.bodyVertical.loadFromFile(RESOURCES_PATH + "body_vertical.png"));
        assert(textures.bodyTopLeft.loadFromFile(RESOURCES_PATH + "body_topleft.png"));
        assert(textures.bodyTopRight.loadFromFile(RESOURCES_PATH + "body_topright.png"));
        assert(textures.bodyBottomLeft.loadFromFile(RESOURCES_PATH + "body_bottomleft.png"));
        assert(textures.bodyBottomRight.loadFromFile(RESOURCES_PATH + "body_bottomright.png"));

        // хвост
        assert(textures.tailUp.loadFromFile(RESOURCES_PATH + "tail_up.png"));
        assert(textures.tailDown.loadFromFile(RESOURCES_PATH + "tail_down.png"));
        assert(textures.tailLeft.loadFromFile(RESOURCES_PATH + "tail_left.png"));
        assert(textures.tailRight.loadFromFile(RESOURCES_PATH + "tail_right.png"));

        // яблоки
        assert(textures.apple.loadFromFile(RESOURCES_PATH + "Apple.png"));
    }

    // ин.змеи
    void InitSnake(Snake& snake)
    {
        snake.segments.clear();
        snake.sprites.clear();

      
        int startX = GRID_WIDTH / 2;
        int startY = GRID_HEIGHT / 2;

        for (int i = 0; i < INITIAL_SNAKE_SIZE; ++i)
        {
            snake.segments.push_back(Position2D(startX - i, startY));
            snake.sprites.push_back(sf::Sprite());  
        }

        snake.direction = Direction::Right;
        snake.nextDirection = Direction::Right;
        snake.isGrowing = false;
    }

    //направления
    bool SetSnakeDirection(Snake& snake, Direction direction)
    {
        
        if ((snake.direction == Direction::Up && direction == Direction::Down) ||
            (snake.direction == Direction::Down && direction == Direction::Up) ||
            (snake.direction == Direction::Left && direction == Direction::Right) ||
            (snake.direction == Direction::Right && direction == Direction::Left))
        {
            return false;
        }
        snake.nextDirection = direction;
        return true;
    }

    // обновление позиции змеи
    void UpdateSnake(Snake& snake)
    {
        
        snake.direction = snake.nextDirection;

        Position2D newHead = snake.segments[0];

        switch (snake.direction)
        {
        case Direction::Up:    newHead.y--; break;
        case Direction::Down:  newHead.y++; break;
        case Direction::Left:  newHead.x--; break;
        case Direction::Right: newHead.x++; break;
        default: break;
        }

        snake.segments.insert(snake.segments.begin(), newHead);
        snake.sprites.insert(snake.sprites.begin(), sf::Sprite());

        if (!snake.isGrowing)
        {
            snake.segments.pop_back();
            snake.sprites.pop_back();
        }
        else
        {
            snake.isGrowing = false;
        }
    }

    // рост змеи
    void GrowSnake(Snake& snake)
    {
        snake.isGrowing = true;
    }

    // столкновение с собой
    bool CheckSelfCollision(const Snake& snake)
    {
        const Position2D& head = snake.segments[0];
        for (int i = 1; i < (int)snake.segments.size(); ++i)
        {
            if (snake.segments[i] == head)
            {
                return true;
            }
        }
        return false;
    }

    bool CheckWallCollision(const Snake& snake)
    {
        const Position2D& head = snake.segments[0];
        return (head.x < 0 || head.x >= GRID_WIDTH ||
            head.y < 0 || head.y >= GRID_HEIGHT);
    }


    void UpdateSnakeSprites(Snake& snake, const SnakeTextures& textures)
    {
        if (snake.segments.empty()) return;

        int size = (int)snake.segments.size();

        // голова
        sf::Sprite& headSprite = snake.sprites[0];
        switch (snake.direction)
        {
        case Direction::Up:    headSprite.setTexture(textures.headUp); break;
        case Direction::Down:  headSprite.setTexture(textures.headDown); break;
        case Direction::Left:  headSprite.setTexture(textures.headLeft); break;
        case Direction::Right: headSprite.setTexture(textures.headRight); break;
        default: break;
        }
        headSprite.setPosition(
            snake.segments[0].x * CELL_SIZE,
            snake.segments[0].y * CELL_SIZE
        );

        // тело
        for (int i = 1; i < size - 1; ++i)
        {
            sf::Sprite& bodySprite = snake.sprites[i];

            
            Position2D prev = snake.segments[i - 1] - snake.segments[i];
            Position2D next = snake.segments[i + 1] - snake.segments[i];

            // прямые сегменты
            if (prev.x == next.x) 
            {
                bodySprite.setTexture(textures.bodyVertical);
            }
            else if (prev.y == next.y) 
            {
                bodySprite.setTexture(textures.bodyHorizontal);
            }
            // углы
            else if ((prev.x == 1 && next.y == 1) || (prev.y == 1 && next.x == 1))
            {
                bodySprite.setTexture(textures.bodyBottomRight);
            }
            else if ((prev.x == -1 && next.y == 1) || (prev.y == 1 && next.x == -1))
            {
                bodySprite.setTexture(textures.bodyBottomLeft);
            }
            else if ((prev.x == 1 && next.y == -1) || (prev.y == -1 && next.x == 1))
            {
                bodySprite.setTexture(textures.bodyTopRight);
            }
            else if ((prev.x == -1 && next.y == -1) || (prev.y == -1 && next.x == -1))
            {
                bodySprite.setTexture(textures.bodyTopLeft );
            }

            bodySprite.setPosition(
                snake.segments[i].x * CELL_SIZE,
                snake.segments[i].y * CELL_SIZE
            );
        }

        //хвост
        if (size > 1)
        {
            sf::Sprite& tailSprite = snake.sprites[size - 1];

            
            Position2D prevToTail = snake.segments[size - 2] - snake.segments[size - 1];

            if (prevToTail.x == 1)      
            {
                tailSprite.setTexture(textures.tailLeft);
            }
            else if (prevToTail.x == -1) 
            {
                tailSprite.setTexture(textures.tailRight);
            }
            else if (prevToTail.y == 1) 
            {
                tailSprite.setTexture(textures.tailUp);
            }
            else if (prevToTail.y == -1) 
            {
                tailSprite.setTexture(textures.tailDown);
            }

            tailSprite.setPosition(
                snake.segments[size - 1].x * CELL_SIZE,
                snake.segments[size - 1].y * CELL_SIZE
            );
        }
    }

    // отрисовка змеи
    void DrawSnake(const Snake& snake, sf::RenderWindow& window)
    {
        for (const sf::Sprite& sprite : snake.sprites)
        {
            window.draw(sprite);
        }
    }
}