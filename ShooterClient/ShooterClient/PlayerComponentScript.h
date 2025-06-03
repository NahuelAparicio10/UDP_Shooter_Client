#pragma once
#include "Component.h"
#include "Rigidbody2D.h"
#include "InputComponent.h"
#include "Transform.h"
#include "BulletHandler.h"
#include "SpriteRenderer.h"
#include "NetworkDefs.h"
#include "NetworkManager.h"
#include "UtilsMaths.h"
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

	void UpdateReconciliation(float dt);

	void ApplyReconciliation(const MovementPacket& correction);

	float _moveSpeed = 150.f;
	float _jumpForce = -300.f;
	GameObject* playerGo;
	Rigidbody2D* _rigidbody;
	InputComponent* _input;
	SpriteRenderer* _spriteRenderer;
	BulletHandler* _bulletHandler;
	std::deque<MovementPacket> _movementHistory;
	unsigned int _tick = 0;

	sf::Vector2f _reconciliationTarget;
	bool _isReconciling = false;

	int nextGhostBulletID = -1;
	int ghostID = nextGhostBulletID--;
};

