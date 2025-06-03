#pragma once
#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include "FontManager.h"

class UIText : public UIElement
{
public:
    UIText(const std::string& id, const sf::Vector2f& position, sf::Color customColor, const float size, const std::string& text);

    void Update(float dt) override;
    void Render(sf::RenderWindow* window) override;
    void HandleEvent(const sf::Event& event) override;

    std::string GetText() const;
    void SetText(const std::string& text);

private:
    sf::RectangleShape _box;
    sf::Text _text;
    std::string _input;
    sf::Color _customColor;
};
