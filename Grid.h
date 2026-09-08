#pragma once

#include "ResourceIdentifiers.h"

#include <vector>
#include <memory>

#include <SFML/Graphics/Sprite.hpp> 
#include <SFML/Graphics/RectangleShape.hpp>

namespace sf
{
    class RenderWindow;
}

namespace Game
{

    enum class Cell
    {
        Empty,
        Snake,
        Apple,
        Wall
    };

    class Grid
    {
    public:
        Grid(TextureHolder& textures);

        Cell get(const sf::Vector2i& pos) const;

        void draw(sf::RenderWindow& window);

    private:
        std::vector<std::vector<Cell>> mMatrix;

        std::unique_ptr<sf::Sprite>         mGrassSprite;
        std::unique_ptr<sf::Sprite>         mWallSprite;
        sf::RectangleShape                  mBorder;
    };

}