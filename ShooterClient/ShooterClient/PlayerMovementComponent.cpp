#include "PlayerMovementComponent.h"

void PlayerMovementComponent::Update(GameObject* go, float dt)
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
}
