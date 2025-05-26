#include "UIImage.h"

UIImage::UIImage(const std::string& id, const std::string& texturePath, sf::Color color, bool centerOrigin) : _texture(), _sprite(_texture)
{
	SetID(id);

	_texture.loadFromFile(texturePath);

	_sprite.setTexture(_texture, true);
	_sprite.setColor(color);
	if (centerOrigin)
	{
		_sprite.setOrigin(sf::Vector2f(_texture.getSize().x / 2, _texture.getSize().y / 2));
	}
}

void UIImage::Update(float dt)
{
}

void UIImage::Render(sf::RenderWindow* window)
{
	window->draw(_sprite);
}

void UIImage::HandleEvent(const sf::Event& event)
{
}

sf::Sprite UIImage::GetSprite() { return _sprite; }

void UIImage::SetSprite(sf::Sprite sprite) { _sprite = sprite; }
