#include "Game.h"

namespace SnakeGame
{
    void InitModsTexts(Game& game)
    {
        game.modsTitleText.setFont(game.font);
        game.modsTitleText.setCharacterSize(48);
        game.modsTitleText.setFillColor(sf::Color::White);
        game.modsTitleText.setString(" MODS");
        sf::FloatRect modsTitleBounds = game.modsTitleText.getLocalBounds();
        game.modsTitleText.setPosition(SCREEN_WIDTH / 2.f - modsTitleBounds.width / 2.f, 80.f);

        game.modNoGrowthText.setFont(game.font);
        game.modNoGrowthText.setCharacterSize(28);
        game.modNoGrowthText.setFillColor(sf::Color::White);
        game.modNoGrowthText.setString("[ ] Disable snake growth");
        sf::FloatRect modNoGrowthBounds = game.modNoGrowthText.getLocalBounds();
        game.modNoGrowthText.setPosition(SCREEN_WIDTH / 2.f - modNoGrowthBounds.width / 2.f, 190.f);

        game.modFiniteApplesText.setFont(game.font);
        game.modFiniteApplesText.setCharacterSize(28);
        game.modFiniteApplesText.setFillColor(sf::Color::White);
        game.modFiniteApplesText.setString("[ ] Finite apples");
        sf::FloatRect modFiniteBounds = game.modFiniteApplesText.getLocalBounds();
        game.modFiniteApplesText.setPosition(SCREEN_WIDTH / 2.f - modFiniteBounds.width / 2.f, 248.f);

        game.modSoundText.setFont(game.font);
        game.modSoundText.setCharacterSize(28);
        game.modSoundText.setFillColor(sf::Color::White);
        game.modSoundText.setString("[X] Sound ON");
        sf::FloatRect modSoundBounds = game.modSoundText.getLocalBounds();
        game.modSoundText.setPosition(SCREEN_WIDTH / 2.f - modSoundBounds.width / 2.f, 306.f);

        game.modBackText.setFont(game.font);
        game.modBackText.setCharacterSize(28);
        game.modBackText.setFillColor(sf::Color::Yellow);
        game.modBackText.setString("> Back");
        sf::FloatRect modBackBounds = game.modBackText.getLocalBounds();
        game.modBackText.setPosition(
            SCREEN_WIDTH / 2.f - modBackBounds.width / 2.f,
            420.f
        );
    }

    void UpdateMods(Game& game)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (game.selectedModItem > 0)
            {
                game.selectedModItem--;
                sf::sleep(sf::milliseconds(150));
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (game.selectedModItem < 3)
            {
                game.selectedModItem++;
                sf::sleep(sf::milliseconds(150));
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (game.selectedModItem == 0)
            {
                game.modNoGrowth = !game.modNoGrowth;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedModItem == 1)
            {
                game.modFiniteApples = !game.modFiniteApples;
                if (game.modFiniteApples)
                {
                    game.modAppleCount = rand() % 91 + 10;
                    game.modInitialAppleCount = game.modAppleCount;
                    game.applesLeftText.setString("Apples left: " + std::to_string(game.modAppleCount));
                }
                else
                {
                    game.applesLeftText.setString("Apples: infinite");
                }
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedModItem == 2)
            {
                game.modSoundOn = !game.modSoundOn;
                sf::sleep(sf::milliseconds(200));
            }
            else if (game.selectedModItem == 3)
            {
                game.isModsActive = false;
                game.isMenuActive = true;
                sf::sleep(sf::milliseconds(200));
            }
        }

        // обновление текстов
        game.modNoGrowthText.setString(game.modNoGrowth ? "[X] Disable snake growth" : "[ ] Disable snake growth");
        game.modFiniteApplesText.setString(game.modFiniteApples ? "[X] Finite apples (" + std::to_string(game.modAppleCount) + ")" : "[ ] Finite apples");
        game.modSoundText.setString(game.modSoundOn ? "[X] Sound ON" : "[ ] Sound OFF");

        // обновление цветов
        game.modNoGrowthText.setFillColor(game.selectedModItem == 0 ? sf::Color::Yellow : sf::Color::White);
        game.modFiniteApplesText.setFillColor(game.selectedModItem == 1 ? sf::Color::Yellow : sf::Color::White);
        game.modSoundText.setFillColor(game.selectedModItem == 2 ? sf::Color::Yellow : sf::Color::White);

        game.modBackText.setFillColor(game.selectedModItem == 3 ? sf::Color::Yellow : sf::Color::White);
        game.modBackText.setString(game.selectedModItem == 3 ? "> Back" : "Back");
    }

    void DrawMods(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.menuBackground);
        window.draw(game.modsTitleText);
        window.draw(game.modNoGrowthText);
        window.draw(game.modFiniteApplesText);
        window.draw(game.modSoundText);
        window.draw(game.modBackText);
    }
}