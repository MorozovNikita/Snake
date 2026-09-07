#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <vector>
#include <string>

#include "State.h"

namespace Game
{
    class MenuState : public State
    {
    public:
        MenuState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        void updateTextColors();
        void onItemSelected();

        std::vector<std::string> m_items;
        int m_selectedIndex;

        sf::Text m_titleText;
        std::vector<sf::Text> m_itemTexts;
        sf::VertexArray m_arrow;
    };
}