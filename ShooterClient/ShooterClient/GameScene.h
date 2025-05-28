#pragma once
#include "Scene.h"
#include "PhysicsManager.h"
#include "SpriteRenderer.h"
class GameScene : public Scene
{
public:
	GameScene();
	~GameScene();

	virtual void Update(float dt) override;
	virtual void Render(sf::RenderWindow* window) override;
	virtual void HandleEvent(const sf::Event& event) override;
private:
	PhysicsManager _physicsManager;
	std::vector<GameObject*> _players;

protected:
};

