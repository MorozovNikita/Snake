#pragma once

#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf
{
    class RenderWindow;
}

namespace Game
{
    class Grid;
    class Snake;

    class Apple
    {
    public:
        explicit Apple(TextureHolder& textures);

        sf::Vector2i cell() const { return mCell; }

        void placeAt(Grid& grid, sf::Vector2i cell);
        void respawn(Grid& grid, const Snake& snake);

        void draw(sf::RenderWindow& window);

    private:
        void syncSprite();

        sf::Sprite mSprite;
        sf::Vector2i mCell{ 0, 0 };
        bool mPlaced = false;
    };
}
