#pragma once
#include <SFML/Graphics.hpp>

class UIElement
{
public:
	virtual ~UIElement() = default;

	virtual void Update(float dt) = 0;
	virtual void Render(sf::RenderWindow* window) = 0;
	virtual void HandleEvent(const sf::Event& event) = 0;

	void SetActive(bool b) { _active = b; }
	bool IsActive() { return _active; }

	void SetID(const std::string& id) { _id = id; }
	std::string GetID() { return _id; }

protected:
	bool _active;
	std::string _id;
};