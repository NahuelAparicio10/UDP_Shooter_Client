#include "SceneManager.h"

SceneManager::SceneManager()
{
	_currentScene = nullptr;
}

SceneManager::~SceneManager()
{
	delete _currentScene;
}

void SceneManager::ChangeScene(Scene* newScene)
{
	if (_currentScene) delete _currentScene;

	_currentScene = newScene;
}

Scene* SceneManager::GetCurrentScene() { return _currentScene; }

void SceneManager::Update(float dt)
{
	if (!_currentScene) return;
	_currentScene->Update(dt);
}

void SceneManager::Render(sf::RenderWindow& window)
{
	if (!_currentScene) return;
	_currentScene->Render(window);
}

void SceneManager::HandleEvent(const sf::Event& event)
{
	if (!_currentScene) return;
	_currentScene->HandleEvent(event);
}
