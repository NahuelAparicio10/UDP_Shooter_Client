#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const std::string& texturePath, sf::Color color, bool centerOrigin) : _texture(), _sprite(_texture)
{
    _texture.loadFromFile(texturePath);
    _sprite.setTexture(_texture, true);
    _sprite.setColor(color);
    sf::Vector2u size = _texture.getSize();

    if (centerOrigin)
        _sprite.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Draw(sf::RenderWindow* window, Transform* transform)
{
    _sprite.setPosition(transform->position);
    _sprite.setRotation(transform->rotation);
    _sprite.setScale(transform->scale);
    window->draw(_sprite);
}

sf::Sprite SpriteRenderer::GetSprite() { return _sprite; }

const std::type_index SpriteRenderer::GetType() { return typeid(SpriteRenderer); }
