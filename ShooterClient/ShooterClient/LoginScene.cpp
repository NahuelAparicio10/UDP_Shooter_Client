#include "LoginScene.h"

LoginScene::LoginScene()
{
	_client = new ClientTCP();
	_client->ConnectToBootstrapServer("127.0.0.1", 50000);

	UIButton* loginButton = new UIButton({ 200, 60 }, { 540, 300 }, "LOGIN", FontManager::GetMainFont());
	loginButton->OnClick.Subscribe([]() { std::cout << "Login button clicked!\n"; });

	UIButton* registerButton = new UIButton({ 200, 60 }, { 740, 300 }, "REGISTER", FontManager::GetMainFont());
	registerButton->OnClick.Subscribe([]() { std::cout << "Register button clicked!\n"; });

	UITextField* textField = new UITextField({ 150, 30 }, { 740, 100 }, "Hola");

	_canvas.AddElement(loginButton);
	_canvas.AddElement(registerButton);
	_canvas.AddElement(textField);
}

LoginScene::~LoginScene()
{
	delete _client;
}

void LoginScene::Update(float dt)
{
	_canvas.Update(dt);
}

void LoginScene::Render(sf::RenderWindow* window)
{
	_canvas.Render(window);
}

void LoginScene::HandleEvent(const sf::Event& event)
{
	_canvas.HandleEvent(event);
}
