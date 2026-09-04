#include "Game.h"

namespace SnakeGame
{
    void InitMenuTexts(Game& game)
    {
        game.titleText.setFont(game.font);
        game.titleText.setCharacterSize(64);
        game.titleText.setFillColor(sf::Color::White);
        game.titleText.setString("SNAKE GAME");
        sf::FloatRect titleBounds = game.titleText.getLocalBounds();
        game.titleText.setPosition(SCREEN_WIDTH / 2.f - titleBounds.width / 2.f, 60.f);

        game.startGameText.setFont(game.font);
        game.startGameText.setCharacterSize(32);
        game.startGameText.setFillColor(sf::Color::Yellow);
        game.startGameText.setString("> Start Game");
        sf::FloatRect startBounds = game.startGameText.getLocalBounds();
        game.startGameText.setPosition(SCREEN_WIDTH / 2.f - startBounds.width / 2.f, 160.f);

        game.modsText.setFont(game.font);
        game.modsText.setCharacterSize(32);
        game.modsText.setFillColor(sf::Color::White);
        game.modsText.setString("Mods");
        sf::FloatRect modsBounds = game.modsText.getLocalBounds();
        game.modsText.setPosition(SCREEN_WIDTH / 2.f - modsBounds.width / 2.f, 215.f);

        game.leaderboardText.setFont(game.font);
        game.leaderboardText.setCharacterSize(32);
        game.leaderboardText.setFillColor(sf::Color::White);
        game.leaderboardText.setString("Leaderboard");
        sf::FloatRect lbBounds = game.leaderboardText.getLocalBounds();
        game.leaderboardText.setPosition(SCREEN_WIDTH / 2.f - lbBounds.width / 2.f, 270.f);

        game.difficultyText.setFont(game.font);
        game.difficultyText.setCharacterSize(32);
        game.difficultyText.setFillColor(sf::Color::White);
        game.difficultyText.setString("Difficulty");
        sf::FloatRect diffBounds = game.difficultyText.getLocalBounds();
        game.difficultyText.setPosition(SCREEN_WIDTH / 2.f - diffBounds.width / 2.f, 325.f);

        game.exitGameText.setFont(game.font);
        game.exitGameText.setCharacterSize(32);
        game.exitGameText.setFillColor(sf::Color::White);
        game.exitGameText.setString("Exit Game");
        sf::FloatRect exitBounds = game.exitGameText.getLocalBounds();
        game.exitGameText.setPosition(SCREEN_WIDTH / 2.f - exitBounds.width / 2.f, 380.f);
    }

    void UpdateMenu(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (game.selectedMenuItem > 0)
            {
                game.selectedMenuItem--;
                sf::sleep(sf::milliseconds(150));
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (game.selectedMenuItem < 4)
            {
                game.selectedMenuItem++;
                sf::sleep(sf::milliseconds(150));
            }
        }

        // Обновление цветов
        game.startGameText.setFillColor(game.selectedMenuItem == 0 ? sf::Color::Yellow : sf::Color::White);
        game.startGameText.setString(game.selectedMenuItem == 0 ? "> Start Game" : "Start Game");

        game.modsText.setFillColor(game.selectedMenuItem == 1 ? sf::Color::Yellow : sf::Color::White);
        game.modsText.setString(game.selectedMenuItem == 1 ? "> Mods" : "Mods");

        game.leaderboardText.setFillColor(game.selectedMenuItem == 2 ? sf::Color::Yellow : sf::Color::White);
        game.leaderboardText.setString(game.selectedMenuItem == 2 ? "> Leaderboard" : "Leaderboard");

        game.difficultyText.setFillColor(game.selectedMenuItem == 3 ? sf::Color::Yellow : sf::Color::White);
        game.difficultyText.setString(game.selectedMenuItem == 3 ? "> Difficulty" : "Difficulty");

        game.exitGameText.setFillColor(game.selectedMenuItem == 4 ? sf::Color::Yellow : sf::Color::Red);
        game.exitGameText.setString(game.selectedMenuItem == 4 ? "> Exit Game" : "Exit Game");

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            if (game.selectedMenuItem == 0)
            {
                game.isMenuActive = false;
               
                RestartGame(game);
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedMenuItem == 1)
            {
                game.isMenuActive = false;
                game.isModsActive = true;
                game.selectedModItem = 0;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedMenuItem == 2)
            {
                game.isMenuActive = false;
                game.isLeaderboardActive = true;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedMenuItem == 3)
            {
                game.isMenuActive = false;
                game.isDifficultyActive = true;
                game.selectedDifficultyItem = game.difficulty;
                sf::sleep(sf::milliseconds(200));
            }
        }
    }

    void DrawMenu(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.menuBackground);
        window.draw(game.titleText);
        window.draw(game.startGameText);
        window.draw(game.modsText);
        window.draw(game.leaderboardText);
        window.draw(game.difficultyText);
        window.draw(game.exitGameText);
    }
}