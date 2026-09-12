#include "Apple.h"

#include "Constants.h"
#include "Grid.h"
#include "ResourceHolder.h"
#include "Snake.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <random>
#include <vector>

namespace Game
{
    Apple::Apple(TextureHolder& textures)
        : mSprite(textures.get(Textures::Apple))
    {
        const sf::Texture& texture = textures.get(Textures::Apple);
        const auto size = texture.getSize();
        const float scale = static_cast<float>(CELL_SIZE) / static_cast<float>(size.x);
        mSprite.setScale({ scale, scale });
        mSprite.setOrigin({ size.x / 2.f, size.y / 2.f });
    }

    void Apple::syncSprite()
    {
        mSprite.setPosition({
            (mCell.x + 0.5f) * static_cast<float>(CELL_SIZE),
            (mCell.y + 0.5f) * static_cast<float>(CELL_SIZE)
        });
    }

    void Apple::placeAt(Grid& grid, sf::Vector2i cell)
    {
        if (mPlaced && grid.get(mCell) == Cell::Apple)
            grid.set(mCell, Cell::Empty);

        mCell = cell;
        mPlaced = true;
        grid.set(mCell, Cell::Apple);
        syncSprite();
    }

    bool Apple::respawn(Grid& grid, const Snake& snake)
    {
        std::vector<sf::Vector2i> freeCells;
        freeCells.reserve(static_cast<std::size_t>(grid.cols() * grid.rows()));

        for (int y = 0; y < grid.rows(); ++y)
        {
            for (int x = 0; x < grid.cols(); ++x)
            {
                const sf::Vector2i cell{ x, y };
                if (grid.get(cell) != Cell::Empty)
                    continue;
                if (snake.occupies(cell))
                    continue;
                freeCells.push_back(cell);
            }
        }

        if (freeCells.empty())
        {
            if (mPlaced && grid.inBounds(mCell) && grid.get(mCell) == Cell::Apple)
                grid.set(mCell, Cell::Empty);
            mPlaced = false;
            return false;
        }

        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<std::size_t> pick(0, freeCells.size() - 1);
        placeAt(grid, freeCells[pick(rng)]);
        return true;
    }

    void Apple::draw(sf::RenderWindow& window)
    {
        if (!mPlaced)
            return;
        window.draw(mSprite);
    }
}
