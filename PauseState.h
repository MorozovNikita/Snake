#pragma once

#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Game
{
    class PauseState : public State
    {
    public:
        PauseState(StateStack& stack, State::Context& context);
        ~PauseState() override;

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        void beginResume();
        void refreshCountdown();

        sf::RectangleShape mDimmer;
        sf::Text mText;
        bool mCounting = false;
        float mRemaining = 0.f;
    };
}
