#pragma once
#include "UIElement.h"
#include <vector>
#include "UITextField.h"
#include "UIButton.h"
#include "UIImage.h"

class UICanvas
{
public:
	void AddElement(UIElement* element);
	void Update(float dt);
	void Render(sf::RenderWindow* window);
	void HandleEvent(const sf::Event& event);
	void Clear();
	UIElement* GetElementByID(const std::string& id);

private:
	std::vector<UIElement*> _elements;
};

