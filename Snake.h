#pragma once

#include "Direction.h"
#include "ResourceIdentifiers.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <deque>
#include <optional>
#include <vector>

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

        sf::Vector2i headCell() const { return m_trail.front(); }
        Direction direction() const { return m_direction; }

        std::optional<sf::Vector2i> popEnteredCell();

    private:
        void commitStep();
        static sf::Vector2f cellCenter(sf::Vector2i cell);
        static float angleOf(sf::Vector2f v);
        static float lerpAngle(float a, float b, float t);

        int m_cols, m_rows;
        float m_stepTime;
        float m_timer = 0.f;

        Direction m_direction = Direction::Right;
        Direction m_pendingDirection = Direction::Right;

        std::deque<sf::Vector2i> m_trail;
        std::vector<sf::Sprite> m_sprites;
        int m_pendingGrowth = 0;

        std::optional<sf::Vector2i> m_entered;
    };
}