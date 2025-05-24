#pragma once
#include "UIElement.h"
#include <vector>

class UICanvas
{
public:
	void AddElement(UIElement* element);
	void Render(sf::RenderWindow& window);
	void HandleEvent(const sf::Event& event);

private:
	std::vector<UIElement*> _elements;
};

