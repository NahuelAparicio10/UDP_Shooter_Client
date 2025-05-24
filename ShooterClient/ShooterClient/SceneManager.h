#pragma once
#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	static void ChangeScene(Scene* newScene);
	static Scene* GetCurrentScene();
	static void Update(float dt);
	static void Render(sf::RenderWindow& window);
	static void HandleEvent(const sf::Event& event);

private:
	static Scene* _currentScene;
};

