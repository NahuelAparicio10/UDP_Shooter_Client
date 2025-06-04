#pragma once
#include "Component.h"
#include <SFML/Graphics/RenderWindow.hpp>

class InputComponent : public Component
{
public:
	bool moveLeft = false;
	bool moveRight = false;
	bool jump = false;
	bool shoot = false;
	bool emote = false;

	void ProcessEvent(const sf::Event& event);

	virtual const std::type_index GetType() override;
private:
};

