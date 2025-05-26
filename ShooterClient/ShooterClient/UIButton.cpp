#include "UIButton.h"

UIButton::UIButton(const std::string& id, const sf::Vector2f& size, const sf::Vector2f& position, const std::string& text, sf::Font& font, sf::Color color) : _text(font, text, 20)
{
    SetID(id);
    SetActive(true);
    _background.setSize(size);
    _background.setPosition(position);
    _background.setFillColor(color);

    _text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = _text.getLocalBounds();

    _text.setOrigin(textBounds.getCenter());

    _text.setPosition( sf::Vector2f{position.x + size.x / 2.f, position.y + size.y / 2.f } );
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
