#include "LoginScene.h"

LoginScene::LoginScene()
{
	onLoginSuccess = false;
	_customColor = sf::Color(227, 189, 90);

	NetworkManager::GetInstance().GetTCPClient()->ConnectToBootstrapServer(Constants::ServiceServerIP.value().toString(), 50000);

	UIImage* background = new UIImage("bg", "Assets/Backgrounds/loginmenu.png", sf::Color::White, false);
	UIButton* loginButton = new UIButton("BLogin", { 200, 50 }, {330, 560}, "LOGIN", FontManager::GetMainFont(), _customColor);

	loginButton->OnClick.Subscribe([this]() { NetworkManager::GetInstance().GetTCPClient()->LoginRegister("LOGIN", _usernameTextField->GetText(), _passwordTextField->GetText()); });

	UIButton* registerButton = new UIButton("BRegister", { 200, 50 }, {540, 560}, "REGISTER", FontManager::GetMainFont(), _customColor);

	registerButton->OnClick.Subscribe([this]() { NetworkManager::GetInstance().GetTCPClient()->LoginRegister("REGISTER", _usernameTextField->GetText(), _passwordTextField->GetText()); });

	NetworkManager::GetInstance().GetTCPClient()->onLoginSuccess.Subscribe([this] { onLoginSuccess = true; });

	 _usernameTextField = new UITextField("Tusername", {440, 390}, { 200, 50 }, "Username");
	 _passwordTextField = new UITextField("Tpassword", {440, 470}, { 200, 50 }, "Password");

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
