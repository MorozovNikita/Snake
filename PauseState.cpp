#include "PauseState.h"

#include "Constants.h"
#include "Difficulty.h"
#include "MusicPlayer.h"
#include "ResourceHolder.h"
#include "Settings.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <cmath>
#include <string>

namespace Game
{
    PauseState::PauseState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mDimmer({ static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT) })
        , mText(context.fonts.get(Fonts::Main), "Pause"s, 48)
    {
        mDimmer.setFillColor(sf::Color(0, 0, 0, 140));

        const auto bounds = mText.getLocalBounds();
        mText.setOrigin(bounds.position + bounds.size / 2.f);
        mText.setPosition({ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
        mText.setFillColor(sf::Color::White);

        getContext().music.pause();
    }

    PauseState::~PauseState()
    {
        getContext().music.resume();
    }

    void PauseState::beginResume()
    {
        const float delay = ReadyDelay;
        if (delay <= 0.f)
        {
            requestStackPop();
            return;
        }

        mCounting = true;
        mRemaining = delay;
        refreshCountdown();
    }

    void PauseState::refreshCountdown()
    {
        const int n = static_cast<int>(std::ceil(mRemaining));
        mText.setString(n > 0 ? std::to_string(n) : ""s);
        mText.setCharacterSize(72);

        const auto bounds = mText.getLocalBounds();
        mText.setOrigin(bounds.position + bounds.size / 2.f);
        mText.setPosition({ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f });
    }

    void PauseState::draw()
    {
        auto& window = getContext().window;
        window.draw(mDimmer);
        window.draw(mText);
    }

    bool PauseState::update(sf::Time dt)
    {
        if (mCounting)
        {
            mRemaining -= dt.asSeconds();
            if (mRemaining <= 0.f)
                requestStackPop();
            else
                refreshCountdown();
        }
        return false;
    }

    bool PauseState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return false;

        if (mCounting)
            return false;

        const auto& input = getContext().settings.input;
        if (keyPressed->code == input.pause || keyPressed->code == input.back)
            beginResume();

        return false;
    }
}
