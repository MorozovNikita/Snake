#pragma once

#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <array>
#include <string>

namespace Game
{
    class SettingsState : public State
    {
    public:
        SettingsState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        struct Option
        {
            std::string label;
            bool* value = nullptr;
        };

        void toggleSelected();
        void refreshVisuals();

        sf::Text mTitle;
        sf::Text mHint;
        std::array<Option, 3> mOptions{};
        std::array<sf::Text, 3> mLabels;
        std::array<sf::RectangleShape, 3> mBoxes;
        std::array<sf::RectangleShape, 3> mMarks;
        int mSelected = 0;
    };
}
