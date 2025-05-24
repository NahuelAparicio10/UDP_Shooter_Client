#pragma once
#include <SFML/Graphics.hpp>

class GameManager
{
public:
	GameManager();
	void Run();

private:
	sf::RenderWindow _window;
	sf::Clock _clock;
};

