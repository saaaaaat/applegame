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
        bool isGameActive = false;

        int selectedMenuItem = 0;     
        int selectedModItem = 0;

        // настройки
        bool modNoGrowth = false;
        bool modFiniteApples = false;
        bool modSoundOn = true;
        int modAppleCount = 20;

       

        // таблица
        std::vector<Record> leaderboard;
        bool isLeaderboardGenerated = false;

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

        // Меню
        sf::Text titleText;
        sf::Text startGameText;
        sf::Text modsText;
        sf::Text leaderboardText;     
        sf::Text exitGameText;

        // моды
        sf::Text modsTitleText;
        sf::Text modNoGrowthText;
        sf::Text modFiniteApplesText;
        sf::Text modSoundText;
        sf::Text modBackText;

        sf::Text leaderboardTitleText;
        sf::Text leaderboardEntriesText;
        sf::Text leaderboardBackText;

        // игровые тексты
        sf::Text scoreText;
        sf::Text applesLeftText;
        sf::Text gameOverText;
        sf::Text restartHintText;
        sf::Text menuHintText;
    };

    void InitGame(Game& game);
    void UpdateGame(Game& game, float deltaTime);
    void DrawGame(Game& game, sf::RenderWindow& window);
    void HandleInput(Game& game);
    void RestartGame(Game& game);

    void UpdateMenu(Game& game);
    void DrawMenu(Game& game, sf::RenderWindow& window);

    void UpdateMods(Game& game);
    void DrawMods(Game& game, sf::RenderWindow& window);

    void GenerateLeaderboard(Game& game);
    void UpdateLeaderboard(Game& game);
    void UpdateLeaderboardMenu(Game& game);
    void DrawLeaderboard(Game& game, sf::RenderWindow& window);

    bool SerializeLeaderboard(const Game& game);
    bool DeserializeLeaderboard(Game& game);

}