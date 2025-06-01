#pragma once
#include "Component.h"
#include "Rigidbody2D.h"
#include "InputComponent.h"
#include "Transform.h"
#include "BulletHandler.h"
#include "SpriteRenderer.h"

class PlayerComponentScript : public Component
{
public:
	PlayerComponentScript(BulletHandler* bHandler, GameObject* go);
	void Update(float dt);
	void RenderPlayer(sf::RenderWindow* window);
	void HandlePlayerEvents(const sf::Event& event);
	const std::type_index GetType() override;

private:
	void UpdatePlayerPhysics(float dt);
	void UpdateMovement(float dt);

	float _moveSpeed = 150.f;
	float _jumpForce = -300.f;
	GameObject* playerGo;
	Rigidbody2D* _rigidbody;
	InputComponent* _input;
	SpriteRenderer* _spriteRenderer;
	BulletHandler* _bulletHandler;

};

