#include "Grid.h"

#include "Constants.h"
#include "ResourceHolder.h"

#include <ranges>

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game  
{

    Grid::Grid(TextureHolder& textures)
        : mMatrix(Game::SCREEN_HEIGHT / Game::CELL_SIZE, std::vector<Cell>(Game::SCREEN_WIDTH / Game::CELL_SIZE, Cell::Empty))
        , mBorder({ static_cast<float>(Game::CELL_SIZE), static_cast<float>(Game::CELL_SIZE) })
    {
        const sf::Texture& grass = textures.get(Textures::Grass);

        const sf::Vector2u textureGrassSize = grass.getSize();
        const float scaleGrass = static_cast<float>(Game::CELL_SIZE) / static_cast<float>(textureGrassSize.x);
        mGrassSprite = std::make_unique<sf::Sprite>(grass);
        mGrassSprite->setScale({ scaleGrass, scaleGrass });

        const sf::Texture& wall = textures.get(Textures::Wall);
        const sf::Vector2u textureWallSize = wall.getSize();
        const float scaleWall = static_cast<float>(Game::CELL_SIZE) / static_cast<float>(textureWallSize.x);
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

    Cell Grid::get(const sf::Vector2i& pos) const
    {
        return mMatrix[pos.y][pos.x];
    }

    void Grid::draw(sf::RenderWindow& window)
    {
        for (std::size_t y = 0; y < mMatrix.size(); ++y)
        {
            for (std::size_t x = 0; x < mMatrix[y].size(); ++x)
            {
                const sf::Vector2f pos(x * static_cast<float>(Game::CELL_SIZE), y * static_cast<float>(Game::CELL_SIZE));

                switch (mMatrix[y][x])
                {
                case Cell::Empty:
                    mGrassSprite->setPosition(pos);
                    window.draw(*mGrassSprite); 
                    break;
                case Cell::Wall: 
                    mWallSprite->setPosition(pos);
                    window.draw(*mWallSprite); 
                    break;
                default: 
                    window.draw(*mGrassSprite); 
                    break;
                }

                mBorder.setPosition(pos);
                window.draw(mBorder);
            }
        }
    }

}