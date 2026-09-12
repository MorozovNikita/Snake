#include "Snake.h"

#include "Constants.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <algorithm>
#include <cmath>
#include <iterator>
#include <utility>

namespace Game
{
    namespace
    {
        constexpr bool BodyArtIsHorizontal = true;
        constexpr Direction CornerArtA = Direction::Up;
        constexpr Direction CornerArtB = Direction::Left;

        constexpr float TailTurnStart = 0.45f;
        constexpr float TailTurnEnd = 0.75f;
    }

    float Snake::cornerDegrees(Direction a, Direction b)
    {
        for (int k = 0; k < 4; ++k)
        {
            const Direction ra = rotateClockwise(CornerArtA, k);
            const Direction rb = rotateClockwise(CornerArtB, k);
            if ((ra == a && rb == b) || (ra == b && rb == a))
                return 90.f * static_cast<float>(k);
        }
        return 0.f;
    }

    void Snake::fitToCell(sf::Sprite& sprite, const sf::Texture& texture)
    {
        const auto size = texture.getSize();
        const float scale = static_cast<float>(CELL_SIZE) / static_cast<float>(size.x);
        sprite.setScale({ scale, scale });
        sprite.setOrigin({ size.x / 2.f, size.y / 2.f });
    }

    sf::Vector2f Snake::centerOf(sf::Vector2i cell)
    {
        return {
            (cell.x + 0.5f) * static_cast<float>(CELL_SIZE),
            (cell.y + 0.5f) * static_cast<float>(CELL_SIZE)
        };
    }

    sf::Vector2f Snake::lerp(sf::Vector2f a, sf::Vector2f b, float t)
    {
        return a + (b - a) * t;
    }

    float Snake::lerpAngle(float from, float to, float t)
    {
        return from + std::remainder(to - from, 360.f) * t;
    }

    float Snake::smoothstep(float t)
    {
        t = std::clamp(t, 0.f, 1.f);
        return t * t * (3.f - 2.f * t);
    }

    Snake::Snake(TextureHolder& textures, sf::Vector2i startCell,
        float stepTime, int cols, int rows)
        : m_cols(cols)
        , m_rows(rows)
        , m_stepTime(stepTime)
        , m_head(textures.get(Textures::SnakeHead))
        , m_body(textures.get(Textures::SnakeBody))
        , m_tail(textures.get(Textures::SnakeTail))
        , m_corner(textures.get(Textures::SnakeCorner))
    {
        fitToCell(m_head, textures.get(Textures::SnakeHead));
        fitToCell(m_body, textures.get(Textures::SnakeBody));
        fitToCell(m_tail, textures.get(Textures::SnakeTail));
        fitToCell(m_corner, textures.get(Textures::SnakeCorner));

        const sf::Vector2i back = toVector(m_direction);
        m_cells = {
            startCell,
            startCell - back,
            startCell - back * 2,
            startCell - back * 3
        };
        m_prevHead = m_cells.front();
        m_prevTail = m_cells.back();
    }

    sf::Vector2i Snake::nextHeadCell() const
    {
        return m_cells.front() + toVector(m_pendingDirection);
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
        while (m_timer >= m_stepTime)
        {
            m_timer -= m_stepTime;
            commitStep();
        }
    }

    void Snake::commitStep()
    {
        m_direction = m_pendingDirection;
        const sf::Vector2i next = m_cells.front() + toVector(m_direction);

        if (next.x < 0 || next.x >= m_cols || next.y < 0 || next.y >= m_rows)
            return;

        m_prevHead = m_cells.front();
        m_prevTail = m_cells.back();

        m_cells.push_front(next);
        m_entered = next;

        if (m_pendingGrowth > 0)
        {
            --m_pendingGrowth;
            m_vacated.reset();
        }
        else
        {
            m_vacated = m_cells.back();
            m_cells.pop_back();
        }
    }

    void Snake::grow(int cells)
    {
        m_pendingGrowth += cells;
    }

    bool Snake::occupies(sf::Vector2i cell, bool includeHead) const
    {
        const auto begin = includeHead ? m_cells.begin() : std::next(m_cells.begin());
        return std::find(begin, m_cells.end(), cell) != m_cells.end();
    }

    std::optional<sf::Vector2i> Snake::popEnteredCell()
    {
        return std::exchange(m_entered, std::nullopt);
    }

    std::optional<sf::Vector2i> Snake::popVacatedCell()
    {
        return std::exchange(m_vacated, std::nullopt);
    }

    Snake::Piece Snake::pieceBetween(sf::Vector2i towardA, sf::Vector2i towardB)
    {
        if (towardA + towardB == sf::Vector2i{ 0, 0 })
        {
            const bool horizontal = towardA.x != 0;
            const float angle = (horizontal == BodyArtIsHorizontal) ? 0.f : 90.f;
            return { &m_body, angle };
        }

        return {
            &m_corner,
            cornerDegrees(directionOf(towardA), directionOf(towardB))
        };
    }

    Snake::Piece Snake::pieceAtCell(std::size_t index)
    {
        const sf::Vector2i cell = m_cells[index];
        return pieceBetween(m_cells[index - 1] - cell, m_cells[index + 1] - cell);
    }

    void Snake::drawSprite(sf::RenderWindow& window, sf::Sprite& sprite,
        sf::Vector2f pos, float degrees)
    {
        sprite.setPosition(pos);
        sprite.setRotation(sf::degrees(degrees));
        window.draw(sprite);
    }

    void Snake::drawBody(sf::RenderWindow& window)
    {
        const std::size_t n = m_cells.size();
        for (std::size_t i = 1; i + 1 < n; ++i)
        {
            const Piece piece = pieceAtCell(i);
            drawSprite(window, *piece.sprite, centerOf(m_cells[i]), piece.angle);
        }
    }

    void Snake::drawTail(sf::RenderWindow& window, float t)
    {
        const std::size_t n = m_cells.size();
        const sf::Vector2i tip = m_cells.back();
        const sf::Vector2i neck = m_cells[n - 2];
        const bool sliding = m_prevTail != tip;

        if (sliding)
        {
            const sf::Vector2i toNeck = neck - tip;
            const sf::Vector2i toOld = m_prevTail - tip;
            const bool bend = (toNeck + toOld != sf::Vector2i{ 0, 0 });

            const Piece stub = pieceBetween(toNeck, toOld);
            drawSprite(window, *stub.sprite, centerOf(tip), stub.angle);

            const float travelDeg = toAngle(directionOf(tip - m_prevTail));
            const float neckDeg = toAngle(directionOf(toNeck));

            float faceDeg = travelDeg;
            if (bend)
            {
                const float span = TailTurnEnd - TailTurnStart;
                const float turnT = smoothstep(std::clamp((t - TailTurnStart) / span, 0.f, 1.f));
                faceDeg = lerpAngle(travelDeg, neckDeg, turnT);
            }

            drawSprite(window, m_tail,
                lerp(centerOf(m_prevTail), centerOf(tip), t),
                faceDeg);
            return;
        }

        drawSprite(window, m_tail, centerOf(tip), toAngle(directionOf(neck - tip)));
    }

    void Snake::drawHead(sf::RenderWindow& window, float t)
    {
        drawSprite(window, m_head,
            lerp(centerOf(m_prevHead), centerOf(m_cells.front()), t),
            toAngle(m_direction));
    }

    void Snake::draw(sf::RenderWindow& window)
    {
        if (m_cells.size() < 2)
            return;

        const float t = std::clamp(m_timer / m_stepTime, 0.f, 1.f);

        drawBody(window);
        drawTail(window, t);
        drawHead(window, t);
    }
}
