#pragma once
#include "Scene.h"
#include "PhysicsManager.h"
#include "SpriteRenderer.h"
#include "InputComponent.h"
#include "MapManager.h"
#include "Rigidbody2D.h"
#include "PlayerMovementComponent.h"
#include "BulletHandler.h"

class GameScene : public Scene
{
public:
	GameScene(int numPlayers);
	~GameScene();

	virtual void Update(float dt) override;
	virtual void Render(sf::RenderWindow* window) override;
	virtual void HandleEvent(const sf::Event& event) override;
private:
	void CreatePlayers(int numPlayers);
	PhysicsManager _physicsManager;

	GameObject* _player;
	std::vector<GameObject*> _players;

	MapManager* _mapManager;
	BulletHandler* _bulletHandler;
protected:
};

