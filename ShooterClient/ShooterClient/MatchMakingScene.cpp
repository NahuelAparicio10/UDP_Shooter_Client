#include "MatchMakingScene.h"

MatchMakingScene::MatchMakingScene() 
{
	UIImage* background = new UIImage("bg", "Assets/Backgrounds/bg_mm.png", sf::Color::White, false);
	UIButton* casualButton = new UIButton("Bcasual", { 200, 50 }, { 290, 661 }, "UNRANKED", FontManager::GetMainFont(), sf::Color::White);
	UIButton* rankedButton = new UIButton("Branked", { 200, 50 }, { 608, 661 }, "RANKED", FontManager::GetMainFont(), sf::Color::White);
	UIButton* rankingButton = new UIButton("Branking", { 200, 50 }, { 608, 661 }, "SHOW RANKING", FontManager::GetMainFont(), sf::Color::White);

	_canvas.AddElement(background);
	_canvas.AddElement(casualButton);
	_canvas.AddElement(rankedButton);
	_canvas.AddElement(rankingButton);
}

MatchMakingScene::~MatchMakingScene() 
{
	_canvas.Clear();
}

void MatchMakingScene::Update(float dt)
{
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
