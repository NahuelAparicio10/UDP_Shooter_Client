#pragma once
#include "UIElement.h"
class UIImage : public UIElement
{
public:
	UIImage(const std::string& id, const std::string& texturePath, sf::Color color, bool centerOrigin);
	UIImage(const std::string& id, const std::string& texturePath, sf::Color color, const sf::Vector2f& size, const sf::Vector2f& position, bool centerOrigin);

	void Update(float dt) override;
	void Render(sf::RenderWindow* window) override;
	void HandleEvent(const sf::Event& event) override;

	sf::Sprite GetSprite();
	void SetSprite(sf::Sprite sprite);

private:
	sf::Texture _texture;
	sf::Sprite _sprite;
};

