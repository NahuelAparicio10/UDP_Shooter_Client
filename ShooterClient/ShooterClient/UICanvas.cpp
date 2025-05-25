#include "UICanvas.h"

void UICanvas::AddElement(UIElement* element)
{
	_elements.push_back(element);
}

void UICanvas::Update(float dt)
{
	for (auto e : _elements)
	{
		e->Update(dt);
	}
}

void UICanvas::Render(sf::RenderWindow* window)
{
	for (auto* e : _elements)
	{
		e->Render(window);
	}
}

void UICanvas::HandleEvent(const sf::Event& event)
{
	for (auto* e : _elements)
	{
		e->HandleEvent(event);
	}
}
