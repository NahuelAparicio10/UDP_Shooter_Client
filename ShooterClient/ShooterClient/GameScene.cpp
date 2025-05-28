#include "GameScene.h"

GameScene::GameScene()
{
	UIImage* background = new UIImage("bg", "", sf::Color::White, false);

	auto* obj1 = new GameObject();
	obj1->AddComponent<Transform>()->position = { 100, 100 };
	obj1->AddComponent<BoxCollider2D>()->size = { 50, 50 };
	obj1->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Blue, true);
	_physicsManager.Register(obj1);

	auto* obj2 = new GameObject();
	obj2->AddComponent<Transform>()->position = { 110, 110 };
	obj2->AddComponent<BoxCollider2D>()->size = { 50, 50 };
	obj2->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Red, true);
	_physicsManager.Register(obj2);


	_canvas.AddElement(background);
}

GameScene::~GameScene()
{
}

void GameScene::Update(float dt)
{
	_canvas.Update(dt);
	_physicsManager.Update(dt);
}

void GameScene::Render(sf::RenderWindow* window)
{
	for (auto* p : _players)
	{
		p->GetComponent<SpriteRenderer>()->Draw(window, p->GetComponent<Transform>());
	}
	_canvas.Render(window);
}

void GameScene::HandleEvent(const sf::Event& event)
{
	_canvas.HandleEvent(event);
}
