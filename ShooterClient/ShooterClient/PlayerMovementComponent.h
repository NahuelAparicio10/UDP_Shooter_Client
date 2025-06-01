#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Rigidbody2D.h"
#include "InputComponent.h"
#include "Transform.h"
#include "PhysicsManager.h"
#include "SpriteRenderer.h"
#include "BulletHandler.h"

class PlayerMovementComponent : public Component
{
public:
    float moveSpeed = 150.f;
    float jumpForce = -300.f;
    void SetPhysicsManager(PhysicsManager* physicsManager);
    void Update(GameObject* go, float dt, BulletHandler* bulletHandler);
    const std::type_index GetType() override { return typeid(PlayerMovementComponent); }

private:

    PhysicsManager* _physicsManager;
};

