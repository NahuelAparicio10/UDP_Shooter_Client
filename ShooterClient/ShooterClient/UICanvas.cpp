#include "UICanvas.h"

void UICanvas::AddElement(UIElement* element)
{
	_elements.push_back(element);
}

void UICanvas::Update(float dt)
{
	for (auto e : _elements)
	{
		if(e->IsActive())
			e->Update(dt);
	}
}

void UICanvas::Render(sf::RenderWindow* window)
{
	for (auto* e : _elements)
	{
		if (e->IsActive())
			e->Render(window);
	}
}

void UICanvas::HandleEvent(const sf::Event& event)
{
	for (auto* e : _elements)
	{
		if (e->IsActive())
			e->HandleEvent(event);
	}
}

UIElement* UICanvas::GetElementByID(const std::string& id)
{
	for (auto* e : _elements)
	{
		if (e->GetID() == id)
		{
			return e;
		}
	}
	return nullptr;
}
