#include "LoginScene.h"

LoginScene::LoginScene()
{
	onLoginSuccess = false;

	//NetworkManager::GetInstance().GetTCPClient()->ConnectToBootstrapServer(Constants::SERVICE_SERVER_IP.value().toString(), 50000);
	NetworkManager::GetInstance().GetTCPClient()->ConnectToBootstrapServer();

	UIImage* background = new UIImage("bg", "Assets/Backgrounds/bg_lr.png", sf::Color::White, false);

	UIButton* loginButton = new UIButton("BLogin", { 200, 50 }, {240, 300}, "LOGIN", FontManager::GetMainFont(), sf::Color::White);

	loginButton->OnClick.Subscribe([this]() { NetworkManager::GetInstance().GetTCPClient()->LoginRegister("LOGIN", _usernameTextField->GetText(), _passwordTextField->GetText()); });

	UIButton* registerButton = new UIButton("BRegister", { 200, 50 }, {740, 300}, "REGISTER", FontManager::GetMainFont(), sf::Color::White);

	registerButton->OnClick.Subscribe([this]() { NetworkManager::GetInstance().GetTCPClient()->LoginRegister("REGISTER", _usernameTextField->GetText(), _passwordTextField->GetText()); });

	NetworkManager::GetInstance().GetTCPClient()->onLoginSuccess.Subscribe([this] { onLoginSuccess = true; });

	 _usernameTextField = new UITextField("Tusername", {440, 100}, { 150, 30 }, "Username");
	 _passwordTextField = new UITextField("Tpassword", {440, 200}, { 150, 30 }, "Password");

	_canvas.AddElement(background);
	_canvas.AddElement(loginButton);
	_canvas.AddElement(registerButton);
	_canvas.AddElement(_usernameTextField);
	_canvas.AddElement(_passwordTextField);
}

LoginScene::~LoginScene()
{
	NetworkManager::GetInstance().GetTCPClient()->DisconnectFromBootstrapServer();
	_canvas.Clear();
}

void LoginScene::Update(float dt)
{
	if (onLoginSuccess)
	{
		SceneManager::ChangeScene(new MatchMakingScene());
		onLoginSuccess = false;
	}
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
