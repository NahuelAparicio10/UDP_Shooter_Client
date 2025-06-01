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

    if (_input->jump)
    {
        _rigidbody->velocity.y = _jumpForce;
    }

    if (_input->shoot)
    {
        if (playerGo->transform->scale.x >= 0.f)
        {
            _bulletHandler->CreateBullet(playerGo->transform->position, { 1.f, 0.f });
            _input->shoot = false;
        }
        if (playerGo->transform->scale.x < 0.f)
        {
            _bulletHandler->CreateBullet(playerGo->transform->position, { -1.f, 0.f });
            _input->shoot = false;
        }
    }

    static float accumulator = 0.f; // its stay unless you do acumulator 0.f bc is staic btw
    accumulator += dt;

    if (accumulator >= 0.025f) 
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

void PlayerComponentScript::ApplyReconciliation(const MovementPacket& correction)
{
    // - Checks for the tick in the history
    auto it = std::find_if(_movementHistory.begin(), _movementHistory.end(),
        [&](const MovementPacket& p) { return p.tick == correction.tick; });

    if (it != _movementHistory.end())
    {
        float dx = std::abs(it->position.x - correction.position.x);
        float dy = std::abs(it->position.y - correction.position.y);

        if (dx > 5.f || dy > 5.f) // - correction 
        {
            std::cout << "[RECONCILE] Corrigiendo posición en tick " << correction.tick << "\n";
            playerGo->transform->position = correction.position;
            _rigidbody->velocity = correction.velocity;

            // - Deletes all behind that tick bc it's reconciliated 
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
