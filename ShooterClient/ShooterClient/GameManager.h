#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include "SceneManager.h"
#include "Window.h"
#include "LoginScene.h"
#include "NetworkManager.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
	void Run();

private:
	void Render();
	void HandleEvents();
	Window* _window;
	sf::Clock _clock;
};

