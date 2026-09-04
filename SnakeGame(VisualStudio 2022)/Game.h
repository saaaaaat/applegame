#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include "Constants.h"
#include "Snake.h"
#include "Apple.h"

namespace SnakeGame
{
    // структура для таблици
    struct Record
    {
        std::string name;
        int score;
    };

    struct Game
    {
        Snake snake;
        SnakeTextures textures;
        Apple apple;

        int score = 0;
        bool isGameOver = false;
        float moveTimer = 0.f;

        // состояние
        bool isMenuActive = true;
        bool isModsActive = false;
        bool isLeaderboardActive = false;
    
        bool isDifficultyActive = false;

        int selectedMenuItem = 0;
        int selectedModItem = 0;
        int selectedDifficultyItem = 0;

        // настройки
        bool modNoGrowth = false;
        bool modFiniteApples = false;
        bool modSoundOn = true;
        int modAppleCount = 20;
        int modInitialAppleCount = 20;

        // уровни
        int difficulty = 1;  

        // таблица
        std::vector<Record> leaderboard;
       

        //фоны
        sf::RectangleShape menuBackground;
        sf::RectangleShape gameBackground;

        // звуки
        sf::SoundBuffer eatSoundBuffer;
        sf::SoundBuffer deathSoundBuffer;
        sf::Sound eatSound;
        sf::Sound deathSound;

        //тексты
        sf::Font font;

        // меню
        sf::Text titleText;
        sf::Text startGameText;
        sf::Text modsText;
        sf::Text leaderboardText;
        sf::Text difficultyText;
        sf::Text exitGameText;

        // моды
        sf::Text modsTitleText;
        sf::Text modNoGrowthText;
        sf::Text modFiniteApplesText;
        sf::Text modSoundText;
        sf::Text modBackText;

        // окно таблицы рекордов
        sf::Text leaderboardTitleText;
        sf::Text leaderboardEntriesText;
        sf::Text leaderboardBackText;

        // уровни
        sf::Text difficultyTitleText;
        sf::Text difficultyEasyText;
        sf::Text difficultyNormalText;
        sf::Text difficultyHardText;
        sf::Text difficultyBackText;

        sf::Text scoreText;
        sf::Text applesLeftText;
        sf::Text gameOverText;
        sf::Text restartHintText;
        sf::Text menuHintText;
    };

    // основные функции 
    void InitGame(Game& game);
    void UpdateGame(Game& game, float deltaTime);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void HandleInput(Game& game);
    void RestartGame(Game& game);

    // меню
    void InitMenuTexts(Game& game);
    void UpdateMenu(Game& game);
    void DrawMenu(Game& game, sf::RenderWindow& window);

    // моды
    void InitModsTexts(Game& game);
    void UpdateMods(Game& game);
    void DrawMods(Game& game, sf::RenderWindow& window);

    // таблица
    void InitLeaderboardTexts(Game& game);
    void GenerateLeaderboard(Game& game);
    void UpdateLeaderboard(Game& game);
    void UpdateLeaderboardMenu(Game& game);
    void DrawLeaderboard(Game& game, sf::RenderWindow& window);
    bool SerializeLeaderboard(const Game& game);
    bool DeserializeLeaderboard(Game& game);

    // уровни
    void InitDifficultyTexts(Game& game);
    void UpdateDifficulty(Game& game);
    void DrawDifficulty(Game& game, sf::RenderWindow& window);

  
    void SortLeaderboard(std::vector<Record>& leaderboard);
}