#pragma once

#include "State.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <array>

namespace Game
{
    class DifficultyState : public State
    {
    public:
        DifficultyState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        void refreshVisuals();

        sf::Text mTitle;
        sf::Text mHint;
        std::array<sf::Text, 5> mLabels;
        std::array<sf::RectangleShape, 5> mBoxes;
        std::array<sf::RectangleShape, 5> mMarks;
        int mSelected = 0;
    };
}
