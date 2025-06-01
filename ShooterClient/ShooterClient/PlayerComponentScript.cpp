#include "PlayerComponentScript.h"

PlayerComponentScript::PlayerComponentScript(BulletHandler* bHandler, GameObject* go)
{
	playerGo = go;
	_bulletHandler = bHandler;
	_rigidbody = go->GetComponent<Rigidbody2D>();
	_input = go->GetComponent<InputComponent>();
	_spriteRenderer = go->GetComponent<SpriteRenderer>();
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
}

const std::type_index PlayerComponentScript::GetType()
{
    return typeid(PlayerComponentScript);
}

void PlayerComponentScript::HandlePlayerEvents(const sf::Event& event)
{
    _input->ProcessEvent(event);
}
