#pragma once
#include <SFML/Graphics.hpp>
#include "UIElement.h"
#include "Event.h"
#include "FontManager.h"

class UITextField : public UIElement
{
public:
	UITextField(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& placeholder);

	void Update(float dt) override;
	void Render(sf::RenderWindow* window) override;
	void HandleEvent(const sf::Event& event) override;
	void HandleInput(std::uint32_t unicode);
	std::string GetText() const;
	void SetText(const std::string text);

private:
	sf::RectangleShape _box;
	sf::Text _text;

	std::string _input;
	std::string _placeHolder;

	bool _focused;
	float _cursorBlink;
	bool _cursorVisible;
	bool _firstClick = true;
};

