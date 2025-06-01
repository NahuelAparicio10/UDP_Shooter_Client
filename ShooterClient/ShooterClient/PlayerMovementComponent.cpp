#include "PlayerMovementComponent.h"

void PlayerMovementComponent::SetPhysicsManager(PhysicsManager* physicsManager) { _physicsManager = physicsManager; }

void PlayerMovementComponent::Update(GameObject* go, float dt, BulletHandler* bulletHandler)
{
    auto input = go->GetComponent<InputComponent>();
    auto rb = go->GetComponent<Rigidbody2D>();
    auto transform = go->GetComponent<Transform>();

    if (!input || !rb) return;

    if (input->moveLeft)
    {
        rb->velocity.x = -moveSpeed;
        transform->scale.x = -1.f;
    }
    else if (input->moveRight)
    {
        rb->velocity.x = moveSpeed;
        transform->scale.x = 1.f;
    }
    else
        rb->velocity.x = 0;

    if (input->jump)
    {
        rb->velocity.y = jumpForce;
    }

    if (input->shoot)
    {
        if (transform->scale.x >= 0.f)
        {
            bulletHandler->CreateBullet(go->transform->position, { 1.f, 0.f });
            input->shoot = false;
        }
        if (transform->scale.x < 0.f)
        {
            bulletHandler->CreateBullet(go->transform->position, { -1.f, 0.f });
            input->shoot = false;
        }
    }
}

