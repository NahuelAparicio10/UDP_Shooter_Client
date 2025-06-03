#include "GameScene.h"

GameScene::GameScene(int numPlayers) : _numPlayers(numPlayers)
{
	_mapManager = new MapManager(&_physicsManager);

	_bulletHandler = new BulletHandler(&_physicsManager);	
	
	NetworkManager::GetInstance().GetUDPClient()->StartReceivingGameplayPackets();

	// - When we recieve CREATE_PLAYER from server we create players with the ID and resend to the server player created
	NetworkManager::GetInstance().GetUDPClient()->onPlayerCreatedRecieved.Subscribe(
		[this](const std::vector<CreatePlayerPacket>& playersToCreate)
		{
			std::cout << "Create Invoked";

			CreatePlayer(playersToCreate);		
		}
	);
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
	if (!canStartGame) return;
	_player->GetComponent<PlayerComponentScript>()->Update(dt);

	_bulletHandler->UpdateBullets(dt);

	for (auto* p : _enemies)
	{
		if (p->GetComponent<Rigidbody2D>()->velocity.x >= 0)
		{
			p->transform->scale.x = 1.f;
		}
		else {
			p->transform->scale.x = -1.f;

		}
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

	if (!canStartGame) return;

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
	if (!canStartGame) return;

	_player->GetComponent<PlayerComponentScript>()->HandlePlayerEvents(event);
	_canvas.HandleEvent(event);
}

void GameScene::CreatePlayer(const std::vector<CreatePlayerPacket>& playersToCreate)
{

	if (_enemies.size() + 1 >= _numPlayers)
	{
		return;
	}

	sf::Vector2u size;
	for (const auto& player : playersToCreate)
	{
		if (player.playerID == NetworkManager::GetInstance().GetUDPClient()->currentPlayerID)
		{
			_player = new GameObject();
			_player->GetComponent<Transform>()->position = { player.spawnPosition.x, player.spawnPosition.y };
			_player->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Blue, true);
			size = _player->GetComponent<SpriteRenderer>()->GetSprite().getTexture().getSize();
			_player->AddComponent<BoxCollider2D>()->size = static_cast<sf::Vector2f>(size);
			_player->AddComponent<Rigidbody2D>();
			_player->AddComponent<InputComponent>();
			_player->AddComponent<PlayerComponentScript>(_bulletHandler, _player);
			_physicsManager.Register(_player);
		}
		else 
		{
			auto* enemy = new GameObject();
			enemy->GetComponent<Transform>()->position = { player.spawnPosition.x, player.spawnPosition.y };
			enemy->AddComponent<SpriteRenderer>("Assets/Sprites/player.png", sf::Color::Red, true);
			size = enemy->GetComponent<SpriteRenderer>()->GetSprite().getTexture().getSize();
			enemy->AddComponent<BoxCollider2D>()->size = static_cast<sf::Vector2f>(size);
			enemy->AddComponent<Rigidbody2D>();

			enemy->id = player.playerID;

			_physicsManager.Register(enemy);
			_enemies.push_back(enemy);
		}
	}

		if (_enemies.size() + 1 >= _numPlayers)
		{
			std::cout << "All Players Created" << std::endl;
			std::string msg = std::to_string(NetworkManager::GetInstance().GetUDPClient()->currentMatchID) + ":";
			NetworkManager::GetInstance().GetUDPClient()->Send(PacketHeader::CRITIC, PacketType::ACK_PLAYERS_CREATED, msg, NetworkManager::GetInstance().GetUDPClient()->GetGameServerIP().value(), NetworkManager::GetInstance().GetUDPClient()->GetCurrentGameServerPort());
			std::thread([this]() {
				sf::sleep(sf::milliseconds(50));
				canStartGame = true;
				}).detach();
		}
}
