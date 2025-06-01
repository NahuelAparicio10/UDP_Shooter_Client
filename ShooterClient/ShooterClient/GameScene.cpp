#include "GameScene.h"

GameScene::GameScene(int numPlayers)
{
	_mapManager = new MapManager(&_physicsManager);

	_bulletHandler = new BulletHandler(&_physicsManager);
	CreatePlayers(numPlayers);


	NetworkManager::GetInstance().GetUDPClient()->onMovementPacketRecived.Subscribe(
		[this](const MovementPacket& packet)
		{
			// - Ignore if its me
			if (packet.playerID == NetworkManager::GetInstance().GetUDPClient()->currentPlayerID)
				return;	

			// - Search enemy by ID
			auto it = std::find_if(_enemies.begin(), _enemies.end(),
				[&](GameObject* go) { return go->id == packet.playerID; });

			if (it != _enemies.end())
			{
				InterpolationData& data = _enemyInterpolations[packet.playerID];
				data.previous = data.current;
				data.current = packet.position;
				data.timer = 0.f;
			}
		}
	);
}

GameScene::~GameScene()
{
	delete _mapManager;
}

void GameScene::Update(float dt)
{
	_player->GetComponent<PlayerComponentScript>()->Update(dt);
	_bulletHandler->UpdateBullets(dt);

	for (auto* p : _enemies)
	{
		p->GetComponent<Rigidbody2D>()->Update(p->transform, dt);
	}

	for (auto* enemy : _enemies)
	{
		int id = enemy->id;

		if (_enemyInterpolations.count(id))
		{
			InterpolationData& data = _enemyInterpolations[id];
			data.timer += dt;

			float alpha = std::clamp(data.timer / 0.025f, 0.f, 1.f);
			enemy->transform->position = data.previous * (1.f - alpha) + data.current * alpha;
		}
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

	for (auto* p : _enemies)
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
		if (i == NetworkManager::GetInstance().GetUDPClient()->currentMatchID) continue;

		auto* enemy = new GameObject();
		enemy->GetComponent<Transform>()->position = { 150, 150 };
		enemy->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Red, true);
		size = enemy->GetComponent<SpriteRenderer>()->GetSprite().getTexture().getSize();
		enemy->AddComponent<BoxCollider2D>()->size = static_cast<sf::Vector2f>(size);
		enemy->AddComponent<Rigidbody2D>();
		
		enemy->id = i;
		
		_physicsManager.Register(enemy);
		_enemies.push_back(enemy);
	}
}
