#include "Grid.h"

#include "Constants.h"
#include "ResourceHolder.h"

#include <ranges>
#include <cassert>

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game
{
    Grid::Grid(TextureHolder& textures)
        : mMatrix(SCREEN_HEIGHT / CELL_SIZE, std::vector<Cell>(SCREEN_WIDTH / CELL_SIZE, Cell::Empty))
        , mBorder({ static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE) })
    {
        const sf::Texture& grass = textures.get(Textures::Grass);
        const float scaleGrass = static_cast<float>(CELL_SIZE) / static_cast<float>(grass.getSize().x);
        mGrassSprite = std::make_unique<sf::Sprite>(grass);
        mGrassSprite->setScale({ scaleGrass, scaleGrass });

        const sf::Texture& wall = textures.get(Textures::Wall);
        const float scaleWall = static_cast<float>(CELL_SIZE) / static_cast<float>(wall.getSize().x);
        mWallSprite = std::make_unique<sf::Sprite>(wall);
        mWallSprite->setScale({ scaleWall, scaleWall });

        mBorder.setFillColor(sf::Color::Transparent);
        mBorder.setOutlineColor(sf::Color(0, 0, 0, 40));
        mBorder.setOutlineThickness(-1.f);

        std::ranges::fill(mMatrix.front(), Cell::Wall);
        std::ranges::fill(mMatrix.back(), Cell::Wall);
        std::ranges::for_each(mMatrix, [](std::vector<Cell>& row)
        {
            row.front() = Cell::Wall;
            row.back() = Cell::Wall;
        });
    }

    bool Grid::inBounds(sf::Vector2i pos) const
    {
        return pos.x >= 0 && pos.y >= 0 && pos.x < cols() && pos.y < rows();
    }

    Cell Grid::get(sf::Vector2i pos) const
    {
        assert(inBounds(pos));
        return mMatrix[pos.y][pos.x];
    }

    void Grid::set(sf::Vector2i pos, Cell cell)
    {
        assert(inBounds(pos));
        mMatrix[pos.y][pos.x] = cell;
    }

    void Grid::draw(sf::RenderWindow& window)
    {
        for (int y = 0; y < rows(); ++y)
        {
            for (int x = 0; x < cols(); ++x)
            {
                const sf::Vector2f pos(
                    static_cast<float>(x * CELL_SIZE),
                    static_cast<float>(y * CELL_SIZE));

                if (mMatrix[y][x] == Cell::Wall)
                {
                    mWallSprite->setPosition(pos);
                    window.draw(*mWallSprite);
                }
                else
                {
                    mGrassSprite->setPosition(pos);
                    window.draw(*mGrassSprite);
                }

                mBorder.setPosition(pos);
                window.draw(mBorder);
            }
        }
    }
}
