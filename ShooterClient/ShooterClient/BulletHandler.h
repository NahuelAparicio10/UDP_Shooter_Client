#pragma once
#include<SFML/Graphics.hpp>
#include "PhysicsManager.h"
#include "SpriteRenderer.h"
#include <unordered_map>

class BulletHandler
{
public:
	BulletHandler(PhysicsManager* pManager);
	void DestroyBulletByID(int ID);
	void CreateBullet(int bulletID,sf::Vector2f position, sf::Vector2f direction);
	void UpdateBullets(float dt);
	void RenderBullets(sf::RenderWindow* window);
	void SetPhysicsManager(PhysicsManager* pManager) { _physicsManager = pManager; }
private:
	std::vector<GameObject*> _bullets;
	PhysicsManager* _physicsManager;
	std::unordered_map<int, GameObject*> _bulletById;
};

