#pragma once
#include "Component.h"
#include "PlayerMovementComponent.h"
#include "Rigidbody2D.h"
#include "Transform.h"+ç
#include "SpriteRenderer.h"

class PlayerComponentScript : public Component
{
public:
	void Update(float dt);
	void RenderPlayer();
	void UpdatePlayerPhysics();
	void HandlePlayerEvents();
private:
	GameObject* playerGo;
	Rigidbody2D* _rigidbody;
	Transform* _transform;
	PlayerMovementComponent* _movement;
	SpriteRenderer* _spriteRenderer;
};

