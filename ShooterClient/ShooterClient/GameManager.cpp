#include "GameManager.h"
#include "Constants.h"
GameManager::GameManager()
{
	NetworkManager::GetInstance().Initialize();
	_window = new Window();
	//SceneManager::ChangeScene(new GameScene(Constants::NUM_PLAYERS));	
	SceneManager::ChangeScene(new LoginScene());	
}

GameManager::~GameManager()
{
	delete _window;
}

void GameManager::Run()
{
	while (_window->IsOpen())
	{
		float deltaTime = _clock.restart().asSeconds();
		HandleEvents();
		SceneManager::Update(deltaTime);
		Render();
	}
}

void GameManager::Render()
{
	_window->Clear();
	SceneManager::Render(_window->GetWindow());
	_window->Display();
}

void GameManager::HandleEvents()
{
	while (const std::optional event = _window->GetWindow()->pollEvent())
	{
		SceneManager::HandleEvent(*event);
	}
}
