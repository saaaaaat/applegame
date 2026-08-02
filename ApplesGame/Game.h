#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <map>
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
    
    struct LeaderboardEntry
    {
        std::string name;
        int score;
    };

    struct Game
    {
        Rectangle screenRect;

        Player player;
        Apple* apples = nullptr;
        int numApples = 0;
        Rock rocks[NUM_ROCKS];

        int numEatenApples = 0;
        bool isGameFinished = false;
        float timeSinceGameFinish = 0.f;
        bool isModeSelection = true;
        bool isLeaderboardShown = false;
        bool isGameOverShow = false;

        uint8_t gameModeMask = 0;

        std::vector <LeaderboardEntry> leaderboard;
        std::map<std::string, int >leaderboardMap;


       
        bool isLeaderboardGenerated = false;

        // Ресурсы
        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture rockTexture;
        sf::SoundBuffer eatAppleSoundBuffer;
        sf::SoundBuffer gameOverSoundBuffer;

        sf::Sound eatAppleSound;
        sf::Sound gameOverSound;
        sf::RectangleShape background;

        sf::Font font;
        sf::Text scoreText;
        sf::Text applesCountText;
        sf::Text controlsHintText;
        sf::Text gameOverText;
        sf::Text gameOverScoreText;
        sf::Text modeSelectionText;
        sf::Text leaderboardText;
        sf::Text restartHintText;
    };

        void InitGame(Game& game);
        void UpdateGame(Game& game, float deltaTime);
        void DrawGame(Game& game, sf::RenderWindow& window);
        void DeinializeGame(Game& game);

        void StartPlayingState(Game& game);
        void UpdatePlayingState(Game& game, float deltaTime);

        void StartGameoverState(Game& game);
        void UpdateGameoverState(Game& game, float deltaTime);

        void UpdateModeSelection(Game& game);
        void DrawModeSelection(Game& game, sf::RenderWindow& window);


        void GenerateLeaderboard(Game& game);
        void SortLeaderboard(Game& game);
        void UpdateLeaderboard(Game& game);
        void DrawLeaderboard(Game& game, sf::RenderWindow& window);
        void RestartGame(Game& game);
}
