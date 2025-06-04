#include "UIImage.h"

UIImage::UIImage(const std::string& id, const std::string& texturePath, sf::Color color, bool centerOrigin) 
	: _texture(), _sprite(_texture)
{
	SetID(id);
	SetActive(true);
	_texture.loadFromFile(texturePath);

	_sprite.setTexture(_texture, true);
	_sprite.setColor(color);
	if (centerOrigin)
	{
		_sprite.setOrigin(sf::Vector2f(_texture.getSize().x / 2, _texture.getSize().y / 2));
	}
}
UIImage::UIImage(const std::string& id, const std::string& texturePath, sf::Color color, const sf::Vector2f& size, const sf::Vector2f& position, bool centerOrigin)
	: _texture(), _sprite(_texture)
{
	SetID(id);
	SetActive(true);
	_texture.loadFromFile(texturePath);

	_sprite.setTexture(_texture, true);
	_sprite.setColor(color);

	sf::Vector2u texSize = _texture.getSize();
	if (texSize.x > 0 && texSize.y > 0)
	{
		_sprite.setScale(sf::Vector2f{ size.x / texSize.x, size.y / texSize.y });
	}

	if (centerOrigin)
	{
		_sprite.setOrigin(sf::Vector2f(texSize.x / 2.f, texSize.y / 2.f));
	}

	_sprite.setPosition(position);
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
