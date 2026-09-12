#include "SettingsState.h"

#include "Constants.h"
#include "MusicPlayer.h"
#include "ResourceHolder.h"
#include "Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game
{
    namespace
    {
        constexpr float BoxSize = 28.f;
        constexpr float FirstY = 200.f;
        constexpr float RowGap = 64.f;
        constexpr float CenterX = SCREEN_WIDTH / 2.f;
        constexpr float BoxX = CenterX - 220.f;
    }

    SettingsState::SettingsState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mTitle(context.fonts.get(Fonts::Main), "Settings"s, 40)
        , mHint(context.fonts.get(Fonts::Main), "Enter - toggle    Esc - back"s, 12)
        , mLabels{
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
          }
    {
        auto& settings = getContext().settings;
        mOptions = {
            Option{ "Game sounds"s, &settings.sfx },
            Option{ "Menu music"s, &settings.menuMusic },
            Option{ "Game music"s, &settings.gameMusic },
        };

        const auto titleBounds = mTitle.getLocalBounds();
        mTitle.setOrigin(titleBounds.position + titleBounds.size / 2.f);
        mTitle.setPosition({ CenterX, 80.f });
        mTitle.setFillColor(sf::Color::White);

        const auto hintBounds = mHint.getLocalBounds();
        mHint.setOrigin(hintBounds.position + hintBounds.size / 2.f);
        mHint.setPosition({ CenterX, static_cast<float>(SCREEN_HEIGHT) - 48.f });
        mHint.setFillColor(sf::Color(180, 180, 180));

        for (int i = 0; i < 3; ++i)
        {
            const float y = FirstY + i * RowGap;

            mBoxes[i].setSize({ BoxSize, BoxSize });
            mBoxes[i].setFillColor(sf::Color::Transparent);
            mBoxes[i].setOutlineThickness(3.f);
            mBoxes[i].setPosition({ BoxX, y });

            mMarks[i].setSize({ BoxSize - 12.f, BoxSize - 12.f });
            mMarks[i].setPosition({ BoxX + 6.f, y + 6.f });

            mLabels[i].setString(mOptions[i].label);
            mLabels[i].setPosition({ BoxX + BoxSize + 20.f, y + 2.f });
        }

        refreshVisuals();
    }

    void SettingsState::toggleSelected()
    {
        bool& value = *mOptions[static_cast<std::size_t>(mSelected)].value;
        value = !value;
        getContext().music.apply();
        refreshVisuals();
    }

    void SettingsState::refreshVisuals()
    {
        for (int i = 0; i < 3; ++i)
        {
            const bool selected = (i == mSelected);
            const bool checked = *mOptions[static_cast<std::size_t>(i)].value;
            const sf::Color color = selected ? sf::Color::Green : sf::Color::White;

            mBoxes[i].setOutlineColor(color);
            mLabels[i].setFillColor(color);
            mMarks[i].setFillColor(checked ? color : sf::Color::Transparent);
        }
    }

    void SettingsState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));

        window.draw(mTitle);
        for (int i = 0; i < 3; ++i)
        {
            window.draw(mBoxes[i]);
            window.draw(mMarks[i]);
            window.draw(mLabels[i]);
        }
        window.draw(mHint);
    }

    bool SettingsState::update(sf::Time)
    {
        return false;
    }

    bool SettingsState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        const auto& input = getContext().settings.input;
        if (keyPressed->code == input.menuUp)
        {
            mSelected = (mSelected + 2) % 3;
            refreshVisuals();
        }
        else if (keyPressed->code == input.menuDown)
        {
            mSelected = (mSelected + 1) % 3;
            refreshVisuals();
        }
        else if (keyPressed->code == input.confirm || keyPressed->code == input.pause)
        {
            toggleSelected();
        }
        else if (keyPressed->code == input.back)
        {
            requestStackPop();
        }

        return false;
    }
}
