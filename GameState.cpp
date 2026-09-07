#include "GameState.h"

#include <SFML/Graphics/RenderWindow.hpp>

Game::GameState::GameState(StateStack& stack, State::Context& context)
	: State(stack, context)
    , mGrid(context.textures)
{
}

void Game::GameState::draw()
{
    auto& window = getContext().window;
    window.clear(sf::Color(40, 40, 40));

    mGrid.draw(window);
}

bool Game::GameState::update(sf::Time dt)
{
	return false;
}

bool Game::GameState::handleEvent(const sf::Event& event)
{
    const auto* keyPressed = event.getIf<sf::Event::KeyPressed>();
    if (!keyPressed)
        return true;

    switch (keyPressed->code)
    {
    case sf::Keyboard::Key::Escape:
        requestStackPop();
        break;
    default:
        break;
    }

    return false;
}
