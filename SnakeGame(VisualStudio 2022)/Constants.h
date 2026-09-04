#pragma once
#include <string>

namespace SnakeGame
{
    // ресурсы
    const std::string RESOURCES_PATH = "Resources/";

    const std::string RECORDS_FILE = "records.dat";

    // размер окна
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;

  
    const int CELL_SIZE = 30;

    // количество клеток
    const int GRID_WIDTH = SCREEN_WIDTH / CELL_SIZE;   // 26
    const int GRID_HEIGHT = SCREEN_HEIGHT / CELL_SIZE; // 20

    // змея
    const int INITIAL_SNAKE_SIZE = 3;
    const float MOVE_INTERVAL = 0.15f;  

    // УРОВНИ СКОРОСТЬ
    const float EASY_SPEED = 0.2f;
    const float NORMAL_SPEED = 0.13f;
    const float HARD_SPEED = 0.08f;

    
}