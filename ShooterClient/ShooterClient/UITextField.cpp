#include "UITextField.h"

UITextField::UITextField(const std::string& id, const sf::Vector2f& position, const sf::Vector2f& size, const std::string& placeholder) : _placeHolder(placeholder), _text(FontManager::GetMainFont(), "", 20)
{
    SetID(id);

    _box.setPosition(position);
    _box.setSize(size);
    _box.setFillColor(sf::Color(255, 255, 255, 200));
    _box.setOutlineThickness(2);
    _box.setOutlineColor(sf::Color::Black);

    _text.setPosition(sf::Vector2f(position.x + 10, position.y + 10));
    _text.setFillColor(sf::Color::Black);
}

void UITextField::Update(float dt)
{
    if (_focused) 
    {
        _cursorBlink += dt;

        if (_cursorBlink >= 0.5f) 
        {
            _cursorBlink = 0.f;
            _cursorVisible = !_cursorVisible;
        }
    }
}

void UITextField::Render(sf::RenderWindow* window)
{
    window->draw(_box);

    std::string toDisplay;

    if (_firstClick)
    {
        toDisplay = _input.empty() ? _placeHolder : _input;

        if (_focused && _cursorVisible)
        {
            toDisplay += "|";
        }
    }
    else 
    {
        toDisplay = _input;

        if (_focused && _cursorVisible)
        {
            toDisplay += "|";
        }
    }

    _text.setString(toDisplay);
    window->draw(_text);
}

void UITextField::HandleEvent(const sf::Event& event)
{
    if (const sf::Event::MouseButtonPressed* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f mousePos(mousePressed->position.x, mousePressed->position.y);

        if (_box.getGlobalBounds().contains(mousePos))
        {
            _focused = true;
            if (_firstClick) {
                _input.clear();
                _firstClick = false;
            }
        }
        else
        {
            _focused = false;
        }
    }
    else if (const sf::Event::TextEntered* textEntered = event.getIf<sf::Event::TextEntered>())
    {
        HandleInput(textEntered->unicode);
    }

}

void UITextField::HandleInput(std::uint32_t unicode)
{
    if (!_focused) return;

    if (unicode == 8)
    {
        if (!_input.empty())
        {
            _input.pop_back();
        }
    }
    else if (unicode >= 32 && unicode < 128)
    {
        _input += static_cast<char>(unicode);
    }
}

std::string UITextField::GetText() const { return _input; }

void UITextField::SetText(const std::string text)
{
    if (_input == text) return;

    _input = text;
}
