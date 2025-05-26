#pragma once
#include "UICanvas.h"
#include "Scene.h"
#include "FontManager.h"
#include "ClientTCP.h"
#include "SceneManager.h"
#include "MatchMakingScene.h"

class LoginScene : public Scene
{
public:
	LoginScene();
	~LoginScene();

	void Update(float dt) override;

	void Render(sf::RenderWindow* window) override;

	void HandleEvent(const sf::Event& event) override;

private:
	bool onLoginSuccess;
	UITextField* _usernameTextField;
	UITextField* _passwordTextField;
	UICanvas _canvas;
	ClientTCP* _client;
};

