#pragma once
#include "Scene.h"
#include "PhysicsManager.h"
#include "SpriteRenderer.h"
#include "InputComponent.h"
#include "MapManager.h"
#include "Rigidbody2D.h"
#include "BulletHandler.h"
#include "PlayerComponentScript.h"
#include "NetworkManager.h"
#include "UtilsMaths.h"

class GameScene : public Scene
{
public:
	GameScene(int numPlayers);
	~GameScene();


	virtual void Update(float dt) override;
	virtual void Render(sf::RenderWindow* window) override;
	virtual void HandleEvent(const sf::Event& event) override;
private:
	void CreatePlayer(const std::vector<CreatePlayerPacket>& playersToCreate);
	PhysicsManager _physicsManager;

	GameObject* _player;
	std::vector<GameObject*> _enemies;

	MapManager* _mapManager;
	BulletHandler* _bulletHandler;
	std::unordered_map<int, InterpolationData> _enemyInterpolations;
	int _numPlayers;
	bool canStartGame = false;
};

