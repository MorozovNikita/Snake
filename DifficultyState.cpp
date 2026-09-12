#include "DifficultyState.h"

#include "Constants.h"
#include "Difficulty.h"
#include "ResourceHolder.h"
#include "Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

namespace Game
{
    namespace
    {
        constexpr float BoxSize = 28.f;
        constexpr float FirstY = 160.f;
        constexpr float RowGap = 56.f;
        constexpr float CenterX = SCREEN_WIDTH / 2.f;
        constexpr float BoxX = CenterX - 140.f;
    }

    DifficultyState::DifficultyState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mTitle(context.fonts.get(Fonts::Main), "Difficulty"s, 40)
        , mHint(context.fonts.get(Fonts::Main), "Enter - select    Esc - back"s, 12)
        , mLabels{
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
            sf::Text(context.fonts.get(Fonts::Main), ""s, 22),
          }
        , mSelected(context.settings.difficultyLevel - 1)
    {
        if (mSelected < 0 || mSelected >= DifficultyCount)
            mSelected = 2;

        const auto titleBounds = mTitle.getLocalBounds();
        mTitle.setOrigin(titleBounds.position + titleBounds.size / 2.f);
        mTitle.setPosition({ CenterX, 70.f });
        mTitle.setFillColor(sf::Color::White);

        const auto hintBounds = mHint.getLocalBounds();
        mHint.setOrigin(hintBounds.position + hintBounds.size / 2.f);
        mHint.setPosition({ CenterX, static_cast<float>(SCREEN_HEIGHT) - 48.f });
        mHint.setFillColor(sf::Color(180, 180, 180));

        for (int i = 0; i < DifficultyCount; ++i)
        {
            const float y = FirstY + i * RowGap;
            mBoxes[i].setSize({ BoxSize, BoxSize });
            mBoxes[i].setFillColor(sf::Color::Transparent);
            mBoxes[i].setOutlineThickness(3.f);
            mBoxes[i].setPosition({ BoxX, y });

            mMarks[i].setSize({ BoxSize - 12.f, BoxSize - 12.f });
            mMarks[i].setPosition({ BoxX + 6.f, y + 6.f });

            mLabels[i].setString("Level "s + std::to_string(i + 1));
            mLabels[i].setPosition({ BoxX + BoxSize + 20.f, y + 2.f });
        }

        refreshVisuals();
    }

    void DifficultyState::refreshVisuals()
    {
        const int checked = getContext().settings.difficultyLevel - 1;

        for (int i = 0; i < DifficultyCount; ++i)
        {
            const bool selected = (i == mSelected);
            const sf::Color color = selected ? sf::Color::Green : sf::Color::White;
            mBoxes[i].setOutlineColor(color);
            mLabels[i].setFillColor(color);
            mMarks[i].setFillColor((i == checked) ? color : sf::Color::Transparent);
        }
    }

    void DifficultyState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(30, 30, 30));

        window.draw(mTitle);
        for (int i = 0; i < DifficultyCount; ++i)
        {
            window.draw(mBoxes[i]);
            window.draw(mMarks[i]);
            window.draw(mLabels[i]);
        }
        window.draw(mHint);
    }

    bool DifficultyState::update(sf::Time)
    {
        return false;
    }

    bool DifficultyState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        const auto& input = getContext().settings.input;
        if (keyPressed->code == input.menuUp)
        {
            mSelected = (mSelected + DifficultyCount - 1) % DifficultyCount;
            refreshVisuals();
        }
        else if (keyPressed->code == input.menuDown)
        {
            mSelected = (mSelected + 1) % DifficultyCount;
            refreshVisuals();
        }
        else if (keyPressed->code == input.confirm)
        {
            getContext().settings.difficultyLevel = mSelected + 1;
            refreshVisuals();
        }
        else if (keyPressed->code == input.back)
        {
            requestStackPop();
        }

        return false;
    }
}
