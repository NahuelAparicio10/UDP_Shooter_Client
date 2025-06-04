#include "PlayerComponentScript.h"

PlayerComponentScript::PlayerComponentScript(BulletHandler* bHandler, GameObject* go)
{
	playerGo = go;
	_bulletHandler = bHandler;
	_rigidbody = go->GetComponent<Rigidbody2D>();
	_input = go->GetComponent<InputComponent>();
	_spriteRenderer = go->GetComponent<SpriteRenderer>();

    NetworkManager::GetInstance().GetUDPClient()->onReconcilePacketRecived.Subscribe([this](const MovementPacket& packet) {
        if (packet.playerID == NetworkManager::GetInstance().GetUDPClient()->currentPlayerID)
        {
            ApplyReconciliation(packet);
        }
        });
}

void PlayerComponentScript::Update(float dt)
{
    UpdateMovement(dt);
    UpdateReconciliation(dt);
    UpdatePlayerPhysics(dt);
}

void PlayerComponentScript::RenderPlayer(sf::RenderWindow* window)
{
    _spriteRenderer->Draw(window, playerGo->transform);
}

void PlayerComponentScript::UpdatePlayerPhysics(float dt)
{
    _rigidbody->Update(playerGo->transform, dt);
}

void PlayerComponentScript::UpdateMovement(float dt)
{
    if (!_input || !_rigidbody) return;

    if (_input->moveLeft)
    {
        _rigidbody->velocity.x = -_moveSpeed;
        playerGo->transform->scale.x = -1.f;
    }
    else if (_input->moveRight)
    {
        _rigidbody->velocity.x = _moveSpeed;
        playerGo->transform->scale.x = 1.f;
    }
    else
        _rigidbody->velocity.x = 0;

    if (_input->emote)  
    {
        SFXManager::GetInstance().Play("Assets/Sounds/mrworldwide.wav", 100.f);

        std::string message = std::to_string(NetworkManager::GetInstance().GetUDPClient()->currentMatchID) + ":" + std::to_string(NetworkManager::GetInstance().GetUDPClient()->currentPlayerID);
        NetworkManager::GetInstance().GetUDPClient()->Send(
            PacketHeader::URGENT,
            PacketType::EMOTE,
            message,
            NetworkManager::GetInstance().GetUDPClient()->GetGameServerIP().value(),
            NetworkManager::GetInstance().GetUDPClient()->GetCurrentGameServerPort()
        );
        _input->emote = false;
    }

    if (_input->jump)
    {
        std::cout << "[SALTO] Antes del salto. Rigidbody: " << _rigidbody << "\n";
        std::cout << "Velocity.y = " << _rigidbody->velocity.y << "\n";
        _rigidbody->velocity.y = _jumpForce;
    }

    if (_input->shoot)
    {
        ShootBulletPacket packet;
        packet.matchID = NetworkManager::GetInstance().GetUDPClient()->currentMatchID;
        packet.playerID = NetworkManager::GetInstance().GetUDPClient()->currentPlayerID;
        packet.position = playerGo->transform->position;
        packet.direction = playerGo->transform->scale.x >= 0 ? sf::Vector2f{ 1.f, 0.f } : sf::Vector2f{ -1.f, 0.f };
        if (packet.direction.x < 0)
        {
            packet.position.x -= 35.f;
        }
        else {
            packet.position.x += 35.f;
        }
        NetworkManager::GetInstance().GetUDPClient()->Send(
            PacketHeader::URGENT,
            PacketType::SHOOT_BULLET,
            packet.Serialize(),
            NetworkManager::GetInstance().GetUDPClient()->GetGameServerIP().value(),
            NetworkManager::GetInstance().GetUDPClient()->GetCurrentGameServerPort()
        );
        int ghostBulletID = nextGhostBulletID--;
        _bulletHandler->CreateBullet(ghostBulletID, packet.position, packet.direction);
        _input->shoot = false;
    }

    static float accumulator = 0.f; // its stay unless you do acumulator 0.f bc is staic btw
    accumulator += dt;

    if (accumulator >= .033f) 
    {
        accumulator = 0.f;

        MovementPacket packet;
        packet.matchID = NetworkManager::GetInstance().GetUDPClient()->currentMatchID;
        packet.playerID = NetworkManager::GetInstance().GetUDPClient()->currentPlayerID;
        packet.tick = _tick++;
        packet.position = playerGo->transform->position;
        packet.velocity = _rigidbody->velocity;

        NetworkManager::GetInstance().GetUDPClient()->Send(
            PacketHeader::NORMAL,
            PacketType::PLAYER_MOVEMENT,
            packet.Serialize(),
            NetworkManager::GetInstance().GetUDPClient()->GetGameServerIP().value(),
            NetworkManager::GetInstance().GetUDPClient()->GetCurrentGameServerPort()
        );

        _movementHistory.push_back(packet);

        // - Limit to 100 actions from moveHistory
        if (_movementHistory.size() > 100)
            _movementHistory.pop_front();
    }
}



void PlayerComponentScript::UpdateReconciliation(float dt)
{
    if (!_isReconciling) return;

    sf::Vector2f current = playerGo->transform->position;
    float alpha = 100.f * dt; 
    playerGo->transform->position = current * (1 - alpha) + _reconciliationTarget * alpha;

    if (UtilsMaths::Distance(current, _reconciliationTarget) < 1.f)
        _isReconciling = false;
}



void PlayerComponentScript::ApplyReconciliation(const MovementPacket& correction)
{
    // - Checks for the tick in the history
    auto it = std::find_if(_movementHistory.begin(), _movementHistory.end(),
        [&](const MovementPacket& p) { return p.tick == correction.tick; });

    if (it != _movementHistory.end())
    {
        float dx = std::abs(it->position.x - correction.position.x);
        float dy = std::abs(it->position.y - correction.position.y);

        if (dx > 6.5f || dy > 6.5f)
        {
            std::cout << "[RECONCILE] Corrigiendo tick " << correction.tick << "\n";
            _reconciliationTarget = correction.position;
            _rigidbody->velocity = correction.velocity;
            _isReconciling = true;

            _movementHistory.erase(_movementHistory.begin(), it + 1);
        }
    }
}

const std::type_index PlayerComponentScript::GetType()
{
    return typeid(PlayerComponentScript);
}

void PlayerComponentScript::HandlePlayerEvents(const sf::Event& event)
{
    _input->ProcessEvent(event);
}
