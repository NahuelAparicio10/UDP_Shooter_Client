#include "MatchMakingScene.h"

MatchMakingScene::MatchMakingScene() 
{
	_customColor = sf::Color(227, 189, 90);

	UIImage* background = new UIImage("bg", "Assets/Backgrounds/matchmakingmenu.png", sf::Color::White, false);
	UIImage* background2 = new UIImage("bg2", "Assets/Backgrounds/waitingroom.png", sf::Color::White, false);
	UIButton* casualButton = new UIButton("Bcasual", { 200, 50 }, { 330, 460 }, "UNRANKED", FontManager::GetMainFont(), _customColor);
	UIButton* rankedButton = new UIButton("Branked", { 200, 50 }, { 540, 460 }, "RANKED", FontManager::GetMainFont(), _customColor);
	UIButton* rankingButton = new UIButton("Branking", { 200, 50 }, { 440, 600 }, "SHOW RANKING", FontManager::GetMainFont(), _customColor);
	UIButton* cancelButton = new UIButton("Bcancel", { 200, 50 }, { 440, 505 }, "CANCEL", FontManager::GetMainFont(), _customColor);
	cancelButton->SetActive(false);
	background2->SetActive(false);

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

	NetworkManager::GetInstance().GetUDPClient()->onMatchFound.Subscribe([this](const int nPlayers)
		{
			numPlayers = nPlayers;
			_onMatchFound = true;
		});
	NetworkManager::GetInstance().GetUDPClient()->onCancelConfirmed.Subscribe([this]() {
		ResetUI();
	});
		_canvas.AddElement(background);
		_canvas.AddElement(casualButton);
		_canvas.AddElement(rankedButton);
		_canvas.AddElement(rankingButton);
		_canvas.AddElement(background2);
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
	_canvas.GetElementByID("bg2")->SetActive(true);
	_canvas.GetElementByID("Branked")->SetActive(false);
	_canvas.GetElementByID("Bcasual")->SetActive(false);
	_canvas.GetElementByID("Branking")->SetActive(false);
}

void MatchMakingScene::ResetUI()
{
	_canvas.GetElementByID("Bcancel")->SetActive(false);
	_canvas.GetElementByID("bg2")->SetActive(false);
	_canvas.GetElementByID("Branked")->SetActive(true);
	_canvas.GetElementByID("Bcasual")->SetActive(true);
	_canvas.GetElementByID("Branking")->SetActive(true);
}
