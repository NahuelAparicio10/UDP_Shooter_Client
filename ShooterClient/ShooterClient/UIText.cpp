#include "UIText.h"

UIText::UIText(const std::string& id, const sf::Vector2f& position, sf::Color customColor, const float size, const std::string& text)
    : _input(text),_customColor(customColor), _text(FontManager::GetMainFont(), text, 20)
{
    SetID(id);
    SetActive(true);

    _text.setCharacterSize(size);
    _text.setPosition(position + sf::Vector2f(10.f, 10.f));
    _text.setFillColor(customColor);
}

void UIText::Update(float dt)
{
}

void UIText::Render(sf::RenderWindow* window)
{
  
    window->draw(_text);
}

void UIText::HandleEvent(const sf::Event& event)
{
}

std::string UIText::GetText() const
{
    return _input;
}

void UIText::SetText(const std::string& text)
{
    if (_input == text) return;

    _input = text;
    _text.setString(_input);
}
