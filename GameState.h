#pragma once

#include "State.h"
#include "Grid.h"
#include "Snake.h"

namespace Game
{
    class GameState : public State
    {
    public:
        GameState(StateStack& stack, State::Context& context);

        void draw() override;
        bool update(sf::Time dt) override;
        bool handleEvent(const sf::Event& event) override;

    private:
        Grid mGrid;
        Snake mSnake;
    };
}