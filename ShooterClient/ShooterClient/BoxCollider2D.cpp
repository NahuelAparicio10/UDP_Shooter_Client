#include "BoxCollider2D.h"

void BoxCollider2D::DrawDebug(sf::RenderWindow* window, Transform* transform) const
{
    sf::FloatRect bounds = GetBounds(transform);
    sf::RectangleShape rect;
    rect.setPosition(bounds.position);
    rect.setSize(bounds.size);
    rect.setFillColor(sf::Color::Transparent);
    rect.setOutlineColor(sf::Color::Green);
    rect.setOutlineThickness(1.f);
    window->draw(rect);
}

sf::FloatRect BoxCollider2D::GetBounds(Transform* transform) const
{
    return sf::FloatRect(transform->position + offset - size / 2.f, size);
}

const std::type_index BoxCollider2D::GetType() { return typeid(BoxCollider2D); }

