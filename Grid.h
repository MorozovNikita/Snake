#pragma once

#include "ResourceIdentifiers.h"

#include <vector>
#include <memory>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

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

        int rows() const { return static_cast<int>(mMatrix.size()); }
        int cols() const { return static_cast<int>(mMatrix.front().size()); }

        bool inBounds(sf::Vector2i pos) const;
        Cell get(sf::Vector2i pos) const;
        void set(sf::Vector2i pos, Cell cell);

        void draw(sf::RenderWindow& window);

    private:
        std::vector<std::vector<Cell>> mMatrix;

        std::unique_ptr<sf::Sprite> mGrassSprite;
        std::unique_ptr<sf::Sprite> mWallSprite;
        sf::RectangleShape mBorder;
    };
}
