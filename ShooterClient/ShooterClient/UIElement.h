#pragma once
#include <SFML/Graphics.hpp>

class UIElement
{
public:
	virtual ~UIElement() = default;

	virtual void Render(sf::RenderWindow* window) = 0;
	virtual void HandleEvent(const sf::Event& event) = 0;
};