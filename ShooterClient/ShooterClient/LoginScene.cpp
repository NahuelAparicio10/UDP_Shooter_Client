#include "LoginScene.h"

LoginScene::LoginScene()
{
	UIButton* loginButton = new UIButton({ 200, 60 }, { 540, 300 }, "LOGIN", FontManager::GetMainFont());
	loginButton->OnClick.Subscribe([]() {
		std::cout << "Login button clicked!\n";

		});
	_canvas.AddElement(loginButton);
}

LoginScene::~LoginScene()
{
}

void LoginScene::Update(float dt)
{
}

void LoginScene::Render(sf::RenderWindow* window)
{
	_canvas.Render(window);
}

void LoginScene::HandleEvent(const sf::Event& event)
{
	_canvas.HandleEvent(event);
}
