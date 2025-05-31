#include "GameScene.h"

GameScene::GameScene(int numPlayers)
{
	_mapManager = new MapManager(&_physicsManager);

	CreatePlayers(numPlayers);
}

GameScene::~GameScene()
{
	delete _mapManager;
}

void GameScene::Update(float dt)
{
	_players[0]->GetComponent<PlayerMovementComponent>()->Update(_players[0], dt);

	for (auto* p : _players)
	{
		p->GetComponent<Rigidbody2D>()->Update(p->GetComponent<Transform>(), dt);
	}
	_canvas.Update(dt);
	_physicsManager.Update(dt);
}

void GameScene::Render(sf::RenderWindow* window)
{
	_mapManager->Draw(window);

	for (auto* p : _players)
	{
		Transform* transform = p->GetComponent<Transform>();
		p->GetComponent<SpriteRenderer>()->Draw(window, transform);
		p->GetComponent<BoxCollider2D>()->DrawDebug(window, transform);
	}

	_canvas.Render(window);
}

void GameScene::HandleEvent(const sf::Event& event)
{
	_players[0]->GetComponent<InputComponent>()->ProcessEvent(event);
	_canvas.HandleEvent(event);
}

void GameScene::CreatePlayers(int numPlayers)
{
	//- Generates Player player is _players[0]
	auto* player = new GameObject();
	player->GetComponent<Transform>()->position = { 50, 50 };
	player->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Blue, true);
	sf::Vector2u size = player->GetComponent<SpriteRenderer>()->GetSprite().getTexture().getSize();
	player->AddComponent<BoxCollider2D>()->size = static_cast<sf::Vector2f>(size);
	player->AddComponent<Rigidbody2D>();
	player->AddComponent<InputComponent>();
	player->AddComponent<PlayerMovementComponent>();
	_physicsManager.Register(player);
	_players.push_back(player);

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
