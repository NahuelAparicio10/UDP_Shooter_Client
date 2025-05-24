#pragma once
#include "UICanvas.h"
#include "Scene.h"
#include "UIButton.h"
#include "FontManager.h"
class LoginScene : public Scene
{
public:
	LoginScene();
	~LoginScene();

	void Update(float dt) override;

	void Render(sf::RenderWindow* window) override;

	void HandleEvent(const sf::Event& event) override;

private:
	UICanvas _canvas;
};

