#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>

#include <vector>

namespace Game
{
    class LeaderboardState : public State
    {
    public:
        LeaderboardState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        sf::Text mTitle;
        sf::Text mHint;
        std::vector<sf::Text> mRows;
    };
}
