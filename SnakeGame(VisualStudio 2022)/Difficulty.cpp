#include "Game.h"

namespace SnakeGame
{
    void InitDifficultyTexts(Game& game)
    {
        game.difficultyTitleText.setFont(game.font);
        game.difficultyTitleText.setCharacterSize(48);
        game.difficultyTitleText.setFillColor(sf::Color::Red);
        game.difficultyTitleText.setString(" DIFFICULTY ");
        sf::FloatRect diffTitleBounds = game.difficultyTitleText.getLocalBounds();
        game.difficultyTitleText.setPosition(SCREEN_WIDTH / 2.f - diffTitleBounds.width / 2.f, 80.f);

        game.difficultyEasyText.setFont(game.font);
        game.difficultyEasyText.setCharacterSize(32);
        game.difficultyEasyText.setFillColor(sf::Color::White);
        game.difficultyEasyText.setString("Easy");
        sf::FloatRect easyBounds = game.difficultyEasyText.getLocalBounds();
        game.difficultyEasyText.setPosition(SCREEN_WIDTH / 2.f - easyBounds.width / 2.f, 190.f);

        game.difficultyNormalText.setFont(game.font);
        game.difficultyNormalText.setCharacterSize(32);
        game.difficultyNormalText.setFillColor(sf::Color::White);
        game.difficultyNormalText.setString("Normal");
        sf::FloatRect normalBounds = game.difficultyNormalText.getLocalBounds();
        game.difficultyNormalText.setPosition(SCREEN_WIDTH / 2.f - normalBounds.width / 2.f, 250.f);

        game.difficultyHardText.setFont(game.font);
        game.difficultyHardText.setCharacterSize(32);
        game.difficultyHardText.setFillColor(sf::Color::White);
        game.difficultyHardText.setString("Hard");
        sf::FloatRect hardBounds = game.difficultyHardText.getLocalBounds();
        game.difficultyHardText.setPosition(SCREEN_WIDTH / 2.f - hardBounds.width / 2.f, 310.f);

        game.difficultyBackText.setFont(game.font);
        game.difficultyBackText.setCharacterSize(28);
        game.difficultyBackText.setFillColor(sf::Color::Yellow);
        game.difficultyBackText.setString("> Back");
        sf::FloatRect diffBackBounds = game.difficultyBackText.getLocalBounds();
        game.difficultyBackText.setPosition(
            SCREEN_WIDTH / 2.f - diffBackBounds.width / 2.f,
            400.f
        );
    }

    void UpdateDifficulty(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (game.selectedDifficultyItem > 0)
            {
                game.selectedDifficultyItem--;
                sf::sleep(sf::milliseconds(150));
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (game.selectedDifficultyItem < 3)
            {
                game.selectedDifficultyItem++;
                sf::sleep(sf::milliseconds(150));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (game.selectedDifficultyItem == 0)
            {
                game.difficulty = 0;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedDifficultyItem == 1)
            {
                game.difficulty = 1;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedDifficultyItem == 2)
            {
                game.difficulty = 2;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedDifficultyItem == 3)
            {
                game.isDifficultyActive = false;
                game.isMenuActive = true;
                sf::sleep(sf::milliseconds(200));
            }
        }

        // обновление цветов и текстов
        // Л
        game.difficultyEasyText.setFillColor(game.selectedDifficultyItem == 0 ? sf::Color::Yellow : sf::Color::White);
        game.difficultyEasyText.setString((game.selectedDifficultyItem == 0 ? "> " : "") + std::string("Easy") + (game.difficulty == 0 ? " [X]" : " [ ]"));

        // С
        game.difficultyNormalText.setFillColor(game.selectedDifficultyItem == 1 ? sf::Color::Yellow : sf::Color::White);
        game.difficultyNormalText.setString((game.selectedDifficultyItem == 1 ? "> " : "") + std::string("Normal") + (game.difficulty == 1 ? " [X]" : " [ ]"));

        // Х
        game.difficultyHardText.setFillColor(game.selectedDifficultyItem == 2 ? sf::Color::Yellow : sf::Color::White);
        game.difficultyHardText.setString((game.selectedDifficultyItem == 2 ? "> " : "") + std::string("Hard") + (game.difficulty == 2 ? " [X]" : " [ ]"));

        // назад
        game.difficultyBackText.setFillColor(game.selectedDifficultyItem == 3 ? sf::Color::Yellow : sf::Color::White);
        game.difficultyBackText.setString(game.selectedDifficultyItem == 3 ? "> Back" : "Back");
    }

    void DrawDifficulty(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.menuBackground);
        window.draw(game.difficultyTitleText);
        window.draw(game.difficultyEasyText);
        window.draw(game.difficultyNormalText);
        window.draw(game.difficultyHardText);
        window.draw(game.difficultyBackText);
    }
}