#include "PlayerMovementComponent.h"

void PlayerMovementComponent::Update(GameObject* go, float dt)
{
    auto input = go->GetComponent<InputComponent>();
    auto rb = go->GetComponent<Rigidbody2D>();

    if (!input || !rb) return;

    if (input->moveLeft)
        rb->velocity.x = -moveSpeed;
    else if (input->moveRight)
        rb->velocity.x = moveSpeed;
    else
        rb->velocity.x = 0;

    //if (input->jump && rb->isGrounded)
    if (input->jump)
    {
        rb->velocity.y = jumpForce;
    }
}
