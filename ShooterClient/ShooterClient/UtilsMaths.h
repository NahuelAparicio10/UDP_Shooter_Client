#pragma once
#include "SFML/Graphics.hpp"
class UtilsMaths
{
public:
    inline static float Distance(const sf::Vector2f& a, const sf::Vector2f& b)
    {
        sf::Vector2f diff = b - a;
        return std::sqrt(diff.x * diff.x + diff.y * diff.y);
    }

    inline static sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
    {
        return a + (b - a) * t;
    }
};

