#include "UIButton.h"

UIButton::UIButton(const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, sf::Font& font) : _text(font, text, 20)
{
    _background.setSize(size);
    _background.setPosition(position);
    _background.setFillColor(sf::Color(100, 100, 255));

    _text.setFillColor(sf::Color::White);
    _text.setPosition(sf::Vector2f{position.x + 10, position.y + 5});
}

void UIButton::Update(float dt) { /* Nothing to update */ }

void UIButton::Render(sf::RenderWindow* window)
{
    window->draw(_background);
    window->draw(_text);
}

void UIButton::HandleEvent(const sf::Event& event)
{
    // Si el Usuario clica en los bounds del boton emite Onclick event

    if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) 
    {
        sf::Vector2f mousePos(mousePressed->position.x, mousePressed->position.y);
        
        if (_background.getGlobalBounds().contains(mousePos)) 
        {
            OnClick.Invoke(); 
        }
    }
}
