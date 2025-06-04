#pragma once
#include "Scene.h"
#include "NetworkManager.h"
#include "Constants.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "FontManager.h"

class MatchMakingScene : public Scene
{
public:
	MatchMakingScene();
	~MatchMakingScene();

	void Update(float dt) override;

	void Render(sf::RenderWindow* window) override;

	void HandleEvent(const sf::Event& event) override;

private:
	bool _onMatchFound = false;
	void SetUISearchMatch();
	void ResetUI();
	int numPlayers;
	sf::Color _customColor;

};

