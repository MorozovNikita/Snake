#include "Snake.h"

#include "Constants.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <cmath>

namespace Game
{
    Snake::Snake(TextureHolder& textures, sf::Vector2i startCell,
        float stepTime, int cols, int rows)
        : m_cols(cols)
        , m_rows(rows)
        , m_stepTime(stepTime)
    {
        m_trail.push_back(startCell);
        m_trail.push_back(startCell);

        const sf::Texture& head = textures.get(Textures::SnakeHead);

        m_sprites.emplace_back(head);
        const sf::Vector2u tex = head.getSize();
        const float scale = static_cast<float>(CELL_SIZE) / static_cast<float>(tex.x);
        m_sprites.back().setScale({ scale, scale });
        m_sprites.back().setOrigin({ tex.x / 2.f, tex.y / 2.f });
    }

    void Snake::setInput(Direction desired)
    {
        if (desired == opposite(m_direction))
            return;
        m_pendingDirection = desired;
    }

    void Snake::update(sf::Time dt)
    {
        m_timer += dt.asSeconds();
        if (m_timer < m_stepTime)
            return;

        m_timer = 0.f;
        commitStep();
    }

    void Snake::commitStep()
    {
        m_direction = m_pendingDirection;
        const sf::Vector2i next = m_trail.front() + toVector(m_direction);

        if (next.x < 0 || next.x >= m_cols || next.y < 0 || next.y >= m_rows)
            return;

        m_trail.push_front(next);

        if (m_pendingGrowth > 0)
        {
            --m_pendingGrowth;
            m_sprites.push_back(m_sprites.back());
            m_trail.back() = m_trail[m_trail.size() - 2];
        }
        else
        {
            m_trail.pop_back();
        }

        m_entered = next;
    }

    void Snake::grow(int cells)
    {
        m_pendingGrowth += cells;
    }

    std::optional<sf::Vector2i> Snake::popEnteredCell()
    {
        auto entered = m_entered;
        m_entered.reset();
        return entered;
    }

    sf::Vector2f Snake::cellCenter(sf::Vector2i cell)
    {
        return { (cell.x + 0.5f) * CELL_SIZE, (cell.y + 0.5f) * CELL_SIZE };
    }

    float Snake::angleOf(sf::Vector2f v)
    {
        return std::atan2(v.y, v.x) * 180.f / 3.14159265f;
    }

    float Snake::lerpAngle(float a, float b, float t)
    {
        const float diff = std::remainder(b - a, 360.f);
        return a + diff * t;
    }

    void Snake::draw(sf::RenderWindow& window)
    {
        const float t = m_timer / m_stepTime;

        for (int i = static_cast<int>(m_sprites.size()) - 1; i >= 0; --i)
        {
            const sf::Vector2f from = cellCenter(m_trail[static_cast<size_t>(i) + 1]);
            const sf::Vector2f to = cellCenter(m_trail[static_cast<size_t>(i)]);
            m_sprites[static_cast<size_t>(i)].setPosition(from + (to - from) * t);

            const sf::Vector2f vNow = to - from;
            sf::Vector2f vPrev = vNow;
            if (static_cast<size_t>(i) + 2 < m_trail.size())
                vPrev = cellCenter(m_trail[static_cast<size_t>(i) + 1])
                - cellCenter(m_trail[static_cast<size_t>(i) + 2]);

            if (vNow.x != 0.f || vNow.y != 0.f)
            {
                const float angle = lerpAngle(angleOf(vPrev), angleOf(vNow), t);
                m_sprites[static_cast<size_t>(i)].setRotation(sf::degrees(angle));
            }

            window.draw(m_sprites[static_cast<size_t>(i)]);
        }
    }
}