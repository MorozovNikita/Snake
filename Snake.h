#pragma once

#include "Direction.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>
#include <optional>

namespace sf
{
    class RenderWindow;
    class Texture;
}

namespace Game
{
    class Snake
    {
    public:
        Snake(TextureHolder& textures, sf::Vector2i startCell,
            float stepTime, int cols, int rows);

        void setInput(Direction desired);
        void update(sf::Time dt);
        void draw(sf::RenderWindow& window);

        void grow(int cells = 1);
        void setStepTime(float stepTime) { m_stepTime = stepTime; }
        float getStepTime() const { return m_stepTime; }

        sf::Vector2i headCell() const { return m_cells.front(); }
        sf::Vector2i nextHeadCell() const;
        int segmentCount() const { return static_cast<int>(m_cells.size()); }
        const std::deque<sf::Vector2i>& cells() const { return m_cells; }
        bool occupies(sf::Vector2i cell, bool includeHead = true) const;

        std::optional<sf::Vector2i> popEnteredCell();
        std::optional<sf::Vector2i> popVacatedCell();

    private:
        struct Piece
        {
            sf::Sprite* sprite = nullptr;
            float angle = 0.f;
        };

        void commitStep();

        Piece pieceBetween(sf::Vector2i towardA, sf::Vector2i towardB);
        Piece pieceAtCell(std::size_t index);

        void drawSprite(sf::RenderWindow& window, sf::Sprite& sprite,
            sf::Vector2f pos, float degrees);
        void drawBody(sf::RenderWindow& window);
        void drawTail(sf::RenderWindow& window, float t);
        void drawHead(sf::RenderWindow& window, float t);

        static void fitToCell(sf::Sprite& sprite, const sf::Texture& texture);
        static sf::Vector2f centerOf(sf::Vector2i cell);
        static sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t);
        static float lerpAngle(float from, float to, float t);
        static float smoothstep(float t);
        static float cornerDegrees(Direction a, Direction b);

        int m_cols = 0;
        int m_rows = 0;
        float m_stepTime = 0.15f;
        float m_timer = 0.f;

        Direction m_direction = Direction::Right;
        Direction m_pendingDirection = Direction::Right;

        std::deque<sf::Vector2i> m_cells;
        sf::Vector2i m_prevHead{};
        sf::Vector2i m_prevTail{};
        int m_pendingGrowth = 0;

        sf::Sprite m_head;
        sf::Sprite m_body;
        sf::Sprite m_tail;
        sf::Sprite m_corner;

        std::optional<sf::Vector2i> m_entered;
        std::optional<sf::Vector2i> m_vacated;
    };
}
