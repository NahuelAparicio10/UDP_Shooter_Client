#pragma once
#include "Scene.h"
#include "UICanvas.h"

class MatchMakingScene : public Scene
{
public:
	MatchMakingScene();
	~MatchMakingScene();

	void Update(float dt) override;

	void Render(sf::RenderWindow* window) override;

	void HandleEvent(const sf::Event& event) override;

private:
	UICanvas canvas;
};

