#include "GameScene.h"

GameScene::GameScene(int numPlayers)
{
	_mapManager = new MapManager(&_physicsManager);

	_bulletHandler = new BulletHandler(&_physicsManager);
	CreatePlayers(numPlayers);
}

GameScene::~GameScene()
{
	delete _mapManager;
}

void GameScene::Update(float dt)
{
	_player->GetComponent<PlayerComponentScript>()->Update(dt);
	_bulletHandler->UpdateBullets(dt);

	for (auto* p : _players)
	{
		p->GetComponent<Rigidbody2D>()->Update(p->transform, dt);
	}

	_canvas.Update(dt);
	_physicsManager.Update(dt);
}

void GameScene::Render(sf::RenderWindow* window)
{
	_mapManager->Draw(window);

	_bulletHandler->RenderBullets(window);
	
	_player->GetComponent<PlayerComponentScript>()->RenderPlayer(window);

	_player->GetComponent<BoxCollider2D>()->DrawDebug(window, _player->transform);

	for (auto* p : _players)
	{
		p->GetComponent<SpriteRenderer>()->Draw(window, p->transform);
		p->GetComponent<BoxCollider2D>()->DrawDebug(window, p->transform);
	}

	_canvas.Render(window);
}

void GameScene::HandleEvent(const sf::Event& event)
{
	_player->GetComponent<PlayerComponentScript>()->HandlePlayerEvents(event);
	_canvas.HandleEvent(event);
}

void GameScene::CreatePlayers(int numPlayers)
{
	_player = new GameObject();
	_player->GetComponent<Transform>()->position = { 50, 50 };
	_player->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Blue, true);
	sf::Vector2u size = _player->GetComponent<SpriteRenderer>()->GetSprite().getTexture().getSize();
	_player->AddComponent<BoxCollider2D>()->size = static_cast<sf::Vector2f>(size);
	_player->AddComponent<Rigidbody2D>();
	_player->AddComponent<InputComponent>();
	_player->AddComponent<PlayerComponentScript>(_bulletHandler, _player);
	_physicsManager.Register(_player);

	// - Generetes enemies
	for (int i = 0; i < numPlayers; i++)
	{
		auto* enemy = new GameObject();
		enemy->GetComponent<Transform>()->position = { 150, 150 };
		enemy->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Red, true);
		size = enemy->GetComponent<SpriteRenderer>()->GetSprite().getTexture().getSize();
		enemy->AddComponent<BoxCollider2D>()->size = static_cast<sf::Vector2f>(size);
		enemy->AddComponent<Rigidbody2D>();
		_physicsManager.Register(enemy);
		_players.push_back(enemy);
	}
}
