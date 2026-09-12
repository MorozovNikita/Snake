#include "LeaderboardState.h"

#include "Constants.h"
#include "Leaderboard.h"
#include "ResourceHolder.h"
#include "Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <algorithm>
#include <string>

namespace Game
{
    namespace
    {
        void centerText(sf::Text& text, float x, float y)
        {
            const auto bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition({ x, y });
        }
    }

    LeaderboardState::LeaderboardState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mTitle(context.fonts.get(Fonts::Main), "Leaderboard"s, 36)
        , mHint(context.fonts.get(Fonts::Main), "Esc - back"s, 12)
    {
        centerText(mTitle, SCREEN_WIDTH / 2.f, 60.f);
        mTitle.setFillColor(sf::Color::White);

        centerText(mHint, SCREEN_WIDTH / 2.f, static_cast<float>(SCREEN_HEIGHT) - 40.f);
        mHint.setFillColor(sf::Color(180, 180, 180));

        const auto& records = context.leaderboard.records();
        const int count = std::min(Leaderboard::StoredCount, static_cast<int>(records.size()));

        if (count == 0)
        {
            sf::Text empty(context.fonts.get(Fonts::Main), "No records"s, 18);
            empty.setFillColor(sf::Color::White);
            centerText(empty, SCREEN_WIDTH / 2.f, 260.f);
            mRows.push_back(std::move(empty));
        }

        for (int i = 0; i < count; ++i)
        {
            const auto& record = records[static_cast<std::size_t>(i)];
            std::string name = record.name;
            if (name.size() > 12)
                name.resize(12);

            const std::string line = std::to_string(i + 1) + ".  "s + name + "   "s + std::to_string(record.score);
            sf::Text row(context.fonts.get(Fonts::Main), line, 16);
            row.setFillColor(sf::Color::White);
            centerText(row, SCREEN_WIDTH / 2.f, 130.f + i * 36.f);
            mRows.push_back(std::move(row));
        }
    }

    void LeaderboardState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));
        window.draw(mTitle);
        for (const auto& row : mRows)
            window.draw(row);
        window.draw(mHint);
    }

    bool LeaderboardState::update(sf::Time)
    {
        return false;
    }

    bool LeaderboardState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        if (keyPressed->code == getContext().settings.input.back)
            requestStackPop();

        return false;
    }
}
