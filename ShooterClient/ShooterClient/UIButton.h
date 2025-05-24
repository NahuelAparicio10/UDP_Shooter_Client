#pragma once
#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include "Event.h"

class UIButton : public UIElement
{
public:
    UIButton(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, sf::Font& font);

    void Render(sf::RenderWindow* window) override;
    void HandleEvent(const sf::Event& event) override;

    Event<> OnClick;

private:
    sf::RectangleShape _background;
    sf::Text _text;
};

