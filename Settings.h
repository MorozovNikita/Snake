#pragma once

#include <SFML/Window/Keyboard.hpp>

namespace Game
{
    struct InputBindings
    {
        sf::Keyboard::Key moveUp    = sf::Keyboard::Key::Up;
        sf::Keyboard::Key moveDown  = sf::Keyboard::Key::Down;
        sf::Keyboard::Key moveLeft  = sf::Keyboard::Key::Left;
        sf::Keyboard::Key moveRight = sf::Keyboard::Key::Right;
        sf::Keyboard::Key pause     = sf::Keyboard::Key::Space;
        sf::Keyboard::Key confirm   = sf::Keyboard::Key::Enter;
        sf::Keyboard::Key back      = sf::Keyboard::Key::Escape;
        sf::Keyboard::Key menuUp    = sf::Keyboard::Key::Up;
        sf::Keyboard::Key menuDown  = sf::Keyboard::Key::Down;
    };

    struct Settings
    {
        bool sfx = true;
        bool menuMusic = true;
        bool gameMusic = true;

        int difficultyLevel = 3;
        InputBindings input;
    };
}
