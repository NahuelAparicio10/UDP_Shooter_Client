#include "MatchMakingScene.h"

MatchMakingScene::MatchMakingScene() 
{
	UIImage* background = new UIImage("bg", "Assets/Backgrounds/bg_mm.png", sf::Color::White, false);
	UIButton* casualButton = new UIButton("Bcasual", { 200, 50 }, { 290, 661 }, "UNRANKED", FontManager::GetMainFont(), sf::Color::White);
	UIButton* rankedButton = new UIButton("Branked", { 200, 50 }, { 608, 661 }, "RANKED", FontManager::GetMainFont(), sf::Color::White);
	UIButton* rankingButton = new UIButton("Branking", { 200, 50 }, { 908, 661 }, "SHOW RANKING", FontManager::GetMainFont(), sf::Color::White);
	UIButton* cancelButton = new UIButton("Bcancel", { 200, 50 }, { 608, 361 }, "CANCEL", FontManager::GetMainFont(), sf::Color::White);
	cancelButton->SetActive(false);

	casualButton->OnClick.Subscribe([this]() 
	{ 
		SetUISearchMatch();
		NetworkManager::GetInstance().GetUDPClient()->StartMatchSearchWithRetry("NORMAL");
	});

	rankedButton->OnClick.Subscribe([this]() 
	{ 
		SetUISearchMatch();
		NetworkManager::GetInstance().GetUDPClient()->StartMatchSearchWithRetry("RANKED");
	});

	// - Button Pressed -> Cancel Match Search + Resets UI

	cancelButton->OnClick.Subscribe([this]() 
	{
		std::cout << "[CLIENT] Matchmaking cancelled by user." << std::endl;
		NetworkManager::GetInstance().GetUDPClient()->CancelMatchSearch();

	});

	NetworkManager::GetInstance().GetUDPClient()->onMatchFound.Subscribe([this](const std::string& msg)
		{
			numPlayers = atoi(msg.c_str());
			_onMatchFound = true;
		});
	NetworkManager::GetInstance().GetUDPClient()->onCancelConfirmed.Subscribe([this]() {
		ResetUI();
		});
	_canvas.AddElement(background);
	_canvas.AddElement(casualButton);
	_canvas.AddElement(rankedButton);
	_canvas.AddElement(rankingButton);
	_canvas.AddElement(cancelButton);
}

MatchMakingScene::~MatchMakingScene() 
{
	_canvas.Clear();
}

void MatchMakingScene::Update(float dt)
{
	if (_onMatchFound)
	{
		SceneManager::ChangeScene(new GameScene(numPlayers));
	}
	_canvas.Update(dt);
}

void MatchMakingScene::Render(sf::RenderWindow* window)
{
	_canvas.Render(window);
}

void MatchMakingScene::HandleEvent(const sf::Event& event)
{
	_canvas.HandleEvent(event);
}

void MatchMakingScene::SetUISearchMatch()
{
	_canvas.GetElementByID("Bcancel")->SetActive(true);
	_canvas.GetElementByID("Branked")->SetActive(false);
	_canvas.GetElementByID("Bcasual")->SetActive(false);
	_canvas.GetElementByID("Branking")->SetActive(false);
}

void MatchMakingScene::ResetUI()
{
	_canvas.GetElementByID("Bcancel")->SetActive(false);
	_canvas.GetElementByID("Branked")->SetActive(true);
	_canvas.GetElementByID("Bcasual")->SetActive(true);
	_canvas.GetElementByID("Branking")->SetActive(true);
}
