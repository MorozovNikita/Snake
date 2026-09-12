#include "GameState.h"

#include "Constants.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game
{
    GameState::GameState(StateStack& stack, State::Context& context)
        : State(stack, context)
        , mGrid(context.textures)
        , mSnake(context.textures, { 5, 5 }, 0.25f, SCREEN_WIDTH / CELL_SIZE, SCREEN_HEIGHT / CELL_SIZE)
        , mApple(context.textures)
    {
        mApple.respawn(mGrid, mSnake);
    }

    void GameState::draw()
    {
        auto& window = getContext().window;
        window.clear(sf::Color(40, 40, 40));

        mGrid.draw(window);
        mApple.draw(window);
        mSnake.draw(window);
    }

    bool GameState::update(sf::Time dt)
    {
        mSnake.update(dt);

        if (auto entered = mSnake.popEnteredCell())
        {
            const Cell cell = mGrid.get(*entered);

            if (cell == Cell::Wall || mSnake.occupies(*entered, false))
            {
                // TO DO: popup screen
                requestStackPop();
                return true;
            }

            if (cell == Cell::Apple)
            {
                mSnake.grow();
                mApple.respawn(mGrid, mSnake);
            }
        }

        return true;
    }

    bool GameState::handleEvent(const sf::Event& event)
    {
        const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
        if (!keyPressed)
            return true;

        switch (keyPressed->code)
        {
        case sf::Keyboard::Key::Up:    mSnake.setInput(Direction::Up);    break;
        case sf::Keyboard::Key::Down:  mSnake.setInput(Direction::Down);  break;
        case sf::Keyboard::Key::Left:  mSnake.setInput(Direction::Left);  break;
        case sf::Keyboard::Key::Right: mSnake.setInput(Direction::Right); break;
        case sf::Keyboard::Key::Escape:
            requestStackPop();
            break;
        default:
            break;
        }

        return false;
    }
}
