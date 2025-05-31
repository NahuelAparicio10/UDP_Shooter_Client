#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Rigidbody2D.h"
#include "InputComponent.h"
#include "Transform.h"

class PlayerMovementComponent : public Component
{
public:
    float moveSpeed = 150.f;
    float jumpForce = -300.f;

    void Update(GameObject* go, float dt);
    const std::type_index GetType() override { return typeid(PlayerMovementComponent); }
};

