#pragma once

#include <SFML/System/Vector2.hpp>

namespace Game
{
    enum class Direction { Up, Right, Down, Left };

    inline sf::Vector2i toVector(Direction d)
    {
        switch (d)
        {
        case Direction::Up:    return { 0, -1 };
        case Direction::Right: return { 1, 0 };
        case Direction::Down:  return { 0, 1 };
        case Direction::Left:  return { -1, 0 };
        }
        return { 0, 0 };
    }

    inline Direction opposite(Direction d)
    {
        switch (d)
        {
        case Direction::Up:    return Direction::Down;
        case Direction::Down:  return Direction::Up;
        case Direction::Left:  return Direction::Right;
        case Direction::Right: return Direction::Left;
        }
        return d;
    }

    inline Direction turnLeft(Direction d)
    {
        switch (d)
        {
        case Direction::Up:    return Direction::Left;
        case Direction::Left:  return Direction::Down;
        case Direction::Down:  return Direction::Right;
        case Direction::Right: return Direction::Up;
        }
        return d;
    }

    inline Direction turnRight(Direction d) { return opposite(turnLeft(d)); }

    inline float toAngle(Direction d)
    {
        switch (d)
        {
        case Direction::Right: return 0.f;
        case Direction::Down:  return 90.f;
        case Direction::Left:  return 180.f;
        case Direction::Up:    return -90.f;
        }
        return 0.f;
    }
}