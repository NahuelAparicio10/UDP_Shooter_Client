#include "LoginScene.h"

LoginScene::LoginScene()
{
	_client = new ClientTCP();

	_client->ConnectToBootstrapServer("127.0.0.1", 50000);

	UIImage* background = new UIImage("bg", "Assets/Backgrounds/bg_lr.png", sf::Color::White, false);

	UIButton* loginButton = new UIButton("BLogin", { 200, 60 }, {540, 300}, "LOGIN", FontManager::GetMainFont());

	loginButton->OnClick.Subscribe([this]() { _client->LoginRegister("LOGIN", _usernameTextField->GetText(), _passwordTextField->GetText()); });

	UIButton* registerButton = new UIButton("BRegister", { 200, 60 }, {740, 300}, "REGISTER", FontManager::GetMainFont());

	registerButton->OnClick.Subscribe([this]() { _client->LoginRegister("REGISTER", _usernameTextField->GetText(), _passwordTextField->GetText()); });

	_client->onLoginSuccess.Subscribe([] { 
		std::cout << "Matchmaking Menu" << std::endl;
		});

	 _usernameTextField = new UITextField("Tusername", {150, 30}, {740, 100}, "Username");
	 _passwordTextField = new UITextField("Tpassword", { 150, 30 }, {740, 100}, "Password");

	_canvas.AddElement(background);
	_canvas.AddElement(loginButton);
	_canvas.AddElement(registerButton);
	_canvas.AddElement(_usernameTextField);
	_canvas.AddElement(_passwordTextField);
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
