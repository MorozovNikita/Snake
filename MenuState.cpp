#include "MenuState.h"

#include "Constants.h"
#include "MenuConstants.h"
#include "ResourceHolder.h"
#include "StateStack.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game
{
    MenuState::MenuState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , m_selectedIndex(0)
        , m_titleText(context.fonts.get(Fonts::Main), "Main menu"s)
    {   
        m_items = {"Game"s, "Difficulty"s, "Leaderboard"s, "Settings"s, "Exit"s};

        m_titleText.setCharacterSize(40);
        m_titleText.setFillColor(sf::Color::White);

        // center title
        auto titleBounds = m_titleText.getLocalBounds();
        m_titleText.setOrigin(titleBounds.position + titleBounds.size / 2.f);
        m_titleText.setPosition({ SCREEN_WIDTH / 2.f, 80.f });

        const float centerX = SCREEN_WIDTH / 2.f;

        m_itemTexts.clear();
        for (size_t i = 0; i < m_items.size(); ++i)
        {
            sf::Text text(context.fonts.get(Fonts::Main), m_items[i], 24);

            auto bounds = text.getLocalBounds();
            text.setOrigin(bounds.position + bounds.size / 2.f);
            text.setPosition({ centerX, FIRST_POINT_Y + i * DISTANCE });

            m_itemTexts.push_back(text);
        }

        m_arrow.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_arrow.resize(3);
        for (int v = 0; v < 3; ++v)
            m_arrow[v].color = sf::Color::Green;

        updateTextColors();
    }

    void MenuState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));

        // title
        window.draw(m_titleText);

        // menu points
        for (const auto& text : m_itemTexts)
            window.draw(text);

        // left arrow
        const auto& selected = m_itemTexts[m_selectedIndex];
        auto bounds = selected.getGlobalBounds();

        float arrowX = bounds.position.x - ARROW_SIZE - 10.f;
        float arrowY = bounds.position.y + bounds.size.y / 2.f - ARROW_SIZE / 2.f;

        m_arrow[0].position = { arrowX, arrowY };
        m_arrow[1].position = { arrowX, arrowY + ARROW_SIZE };
        m_arrow[2].position = { arrowX + ARROW_SIZE, arrowY + ARROW_SIZE / 2.f };

        window.draw(m_arrow);
    }

    bool MenuState::update(sf::Time dt)
    {
        return true;
    }

    bool MenuState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return true;

        switch (keyPressed->code)
        {
        case sf::Keyboard::Key::Up:
            --m_selectedIndex;
            if (m_selectedIndex < 0)
                m_selectedIndex = static_cast<int>(m_items.size()) - 1;
            updateTextColors();
            break;
        case sf::Keyboard::Key::Down:
            ++m_selectedIndex;
            if (m_selectedIndex >= static_cast<int>(m_items.size()))
                m_selectedIndex = 0;
            updateTextColors();
            break;
        case sf::Keyboard::Key::Enter:
            onItemSelected();
            break;
        case sf::Keyboard::Key::Escape:
            requestStackPop();
            break;
        default:
            break;
        }

        return false;
    }

    void MenuState::updateTextColors()
    {
        for (size_t i = 0; i < m_itemTexts.size(); ++i)
        {
            bool isSelected = (static_cast<int>(i) == m_selectedIndex);
            m_itemTexts[i].setFillColor(isSelected ? sf::Color::Green : sf::Color::White);
        }
    }

    void MenuState::onItemSelected()
    {
        switch (m_selectedIndex)
        {
        case Menu::Game:
            requestStackPush(States::Game);
            break;
        case Menu::Difficulty:
            break;
        case Menu::Leaderboard:
            break;
        case Menu::Settings:
            break;
        case Menu::Exit:
            requestStackPop();
            break;

        default:
            break;
        }
    }
}