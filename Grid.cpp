#include "Grid.h"

#include "Constants.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>

namespace Game  
{

    Grid::Grid(TextureHolder& textures)
        : mMatrix(Game::SCREEN_HEIGHT / Game::CELL_SIZE, std::vector<Cell>(Game::SCREEN_WIDTH / Game::CELL_SIZE, Cell::Empty))
        , mBorder({ static_cast<float>(Game::CELL_SIZE), static_cast<float>(Game::CELL_SIZE) })
    {
        const sf::Texture& grass = textures.get(Textures::Grass);

        const sf::Vector2u texSize = grass.getSize();
        const float scale = static_cast<float>(Game::CELL_SIZE) / static_cast<float>(texSize.x);
        mGrassSprite = std::make_unique<sf::Sprite>(grass);
        mGrassSprite->setScale({ scale, scale });

        mBorder.setFillColor(sf::Color::Transparent);
        mBorder.setOutlineColor(sf::Color(0, 0, 0, 40));
        mBorder.setOutlineThickness(-1.f);
    }

    void Grid::draw(sf::RenderWindow& window)
    {
        for (std::size_t y = 0; y < mMatrix.size(); ++y)
        {
            for (std::size_t x = 0; x < mMatrix[y].size(); ++x)
            {
                const sf::Vector2f pos(x * static_cast<float>(Game::CELL_SIZE), y * static_cast<float>(Game::CELL_SIZE));

                mGrassSprite->setPosition(pos);
                window.draw(*mGrassSprite);

                mBorder.setPosition(pos);
                window.draw(mBorder);
            }
        }
    }

}