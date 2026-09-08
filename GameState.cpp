#include "GameState.h"

#include "Constants.h"

#include <SFML/Graphics/RenderWindow.hpp>

Game::GameState::GameState(StateStack& stack, State::Context& context)
	: State(stack, context)
    , mGrid(context.textures)
    , mSnake(context.textures, {3, 3}, 0.25f, SCREEN_WIDTH / CELL_SIZE, SCREEN_HEIGHT / CELL_SIZE)
{
}

void Game::GameState::draw()
{
    auto& window = getContext().window;
    window.clear(sf::Color(40, 40, 40));

    mGrid.draw(window);
    mSnake.draw(window);
}

bool Game::GameState::update(sf::Time dt)
{
    mSnake.update(dt);

    if (auto entered = mSnake.popEnteredCell())
    {
        // if (mGrid.get(*entered) == Cell::Apple) { }
        if (mGrid.get(*entered) == Cell::Wall)
        {
            // TO DO: popup screen
            requestStackPop();
        }
    }
    return true;
}

bool Game::GameState::handleEvent(const sf::Event& event)
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
