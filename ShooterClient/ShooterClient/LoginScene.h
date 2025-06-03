#pragma once
#include "Scene.h"
#include "FontManager.h"
#include "SceneManager.h"
#include "MatchMakingScene.h"
#include "NetworkManager.h"
#include "Constants.h"

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
	UIText* _testText;
	sf::Color _customColor;
};

