#include "Game.h"
#include <fstream>
#include <cstdlib>

namespace SnakeGame
{
    
    void SortLeaderboard(std::vector<Record>& leaderboard)
    {
        for (int i = 0; i < (int)leaderboard.size() - 1; ++i)
        {
            for (int j = 0; j < (int)leaderboard.size() - i - 1; ++j)
            {
                if (leaderboard[j].score < leaderboard[j + 1].score)
                {
                    std::swap(leaderboard[j], leaderboard[j + 1]);
                }
            }
        }
    }

    // сериализация
    bool SerializeLeaderboard(const Game& game)
    {
        std::ofstream file(RECORDS_FILE);
        if (file.is_open())
        {
            for (const auto& record : game.leaderboard)
            {
                file << record.name << " " << record.score << std::endl;
            }
            file.close();
            return true;
        }
        return false;
    }

    bool DeserializeLeaderboard(Game& game)
    {
        std::ifstream file(RECORDS_FILE);
        if (file.is_open())
        {
            game.leaderboard.clear();

            std::string name;
            int score;
            while (file >> name >> score)
            {
                Record record;
                record.name = name;
                record.score = score;
                game.leaderboard.push_back(record);
            }
            file.close();
            return true;
        }
        return false;
    }

    // генерация
    void GenerateLeaderboard(Game& game)
    {
        game.leaderboard.clear();

        std::string botNames[] = { "Alice", "Bob", "Carol", "Dave" };

        for (int i = 0; i < 4; ++i)
        {
            Record record;
            record.name = botNames[i];
            record.score = rand() % 81 + 10;
            game.leaderboard.push_back(record);
        }

        Record playerRecord;
        playerRecord.name = "Player";
        playerRecord.score = 0;
        game.leaderboard.push_back(playerRecord);

        SortLeaderboard(game.leaderboard);
       
        SerializeLeaderboard(game);
    }

    // обовление
    void UpdateLeaderboard(Game& game)
    {
        for (auto& record : game.leaderboard)
        {
            if (record.name == "Player")
            {
                record.score = game.score;
                break;
            }
        }

        SortLeaderboard(game.leaderboard);
        SerializeLeaderboard(game);
    }

    // тексты
    void InitLeaderboardTexts(Game& game)
    {
        game.leaderboardTitleText.setFont(game.font);
        game.leaderboardTitleText.setCharacterSize(48);
        game.leaderboardTitleText.setFillColor(sf::Color::White);
        game.leaderboardTitleText.setString("LEADERBOARD");
        sf::FloatRect lbTitleBounds = game.leaderboardTitleText.getLocalBounds();
        game.leaderboardTitleText.setPosition(SCREEN_WIDTH / 2.f - lbTitleBounds.width / 2.f, 60.f);

        game.leaderboardEntriesText.setFont(game.font);
        game.leaderboardEntriesText.setCharacterSize(28);
        game.leaderboardEntriesText.setFillColor(sf::Color::White);
        game.leaderboardEntriesText.setPosition(SCREEN_WIDTH / 2.f - 150.f, 140.f);

        game.leaderboardBackText.setFont(game.font);
        game.leaderboardBackText.setCharacterSize(28);
        game.leaderboardBackText.setFillColor(sf::Color::Yellow);
        game.leaderboardBackText.setString("> Back");
        sf::FloatRect lbBackBounds = game.leaderboardBackText.getLocalBounds();
        game.leaderboardBackText.setPosition(
            SCREEN_WIDTH / 2.f - lbBackBounds.width / 2.f,
            520.f
        );
    }

    void UpdateLeaderboardMenu(Game& game)
    {
        std::string entries = "\n";
        for (int i = 0; i < (int)game.leaderboard.size(); ++i)
        {
            entries += std::to_string(i + 1) + ". ";
            entries += game.leaderboard[i].name;

            int dotsCount = 20 - game.leaderboard[i].name.length();
            for (int j = 0; j < dotsCount; ++j)
            {
                entries += ".";
            }

            entries += " " + std::to_string(game.leaderboard[i].score) + "\n\n";
        }

        game.leaderboardEntriesText.setString(entries);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Space) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            game.isLeaderboardActive = false;
            game.isMenuActive = true;
            sf::sleep(sf::milliseconds(200));
        }
    }

    void DrawLeaderboard(Game& game, sf::RenderWindow& window)
    {
        window.draw(game.menuBackground);
        window.draw(game.leaderboardTitleText);
        window.draw(game.leaderboardEntriesText);
        window.draw(game.leaderboardBackText);
    }
}