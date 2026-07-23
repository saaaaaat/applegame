#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Math.h"
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Constants.h"
#include "Math.h"
#include "Player.h"
#include "Apple.h"
#include "Rock.h"

namespace ApplesGame
{
    struct Game
    {
        
        Rectangle screenRect;

        //  объекты
        Player player;
        Apple* apples = nullptr;
        int numApples = 0;
        Rock rocks[NUM_ROCKS];

     
        int numEatenApples = 0;
        bool isGameFinished = false;
        float timeSinceGameFinish = 0.f;
        bool isModeSelection = true;

        // битовая маска режимов
        uint8_t gameModeMask = 0;

        // ресурсы
        sf::Texture playerTexture;
        sf::Texture appleTexture;
        sf::Texture rockTexture;
        sf::SoundBuffer eatAppleSoundBuffer;
        sf::SoundBuffer gameOverSoundBuffer;

        // звуки и фон
        sf::Sound eatAppleSound;
        sf::Sound gameOverSound;
        sf::RectangleShape background;

        // тексты 
        sf::Font font;
        sf::Text scoreText;
        sf::Text applesCountText;
        sf::Text controlsHintText;
        sf::Text gameOverText;
        sf::Text gameOverScoreText;
        sf::Text modeSelectionText;
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
}