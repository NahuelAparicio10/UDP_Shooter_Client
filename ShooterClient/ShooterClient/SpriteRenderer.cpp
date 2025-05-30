#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(const std::string& texturePath, sf::Color color, bool centerOrigin) : _texture(), _sprite(_texture)
{
    if (texturePath.empty())
    {
        sf::Image img;
        img.resize({ 1, 1 }, sf::Color::White); 
        _texture.loadFromImage(img);
    }
    else 
    {
        _texture.loadFromFile(texturePath);
    }
    _sprite.setTexture(_texture, true);
    _sprite.setColor(color);
    sf::Vector2u size = _texture.getSize();

    if (centerOrigin)
        _sprite.setOrigin(sf::Vector2f(size.x / 2, size.y / 2));


}
SpriteRenderer::SpriteRenderer(const std::string& texturePath, sf::Color color, bool centerOrigin, sf::Vector2f size) : _texture(), _sprite(_texture)
{
    unsigned int w = std::max(1u, static_cast<unsigned int>(std::ceil(size.x)));
    unsigned int h = std::max(1u, static_cast<unsigned int>(std::ceil(size.y)));

    if (texturePath.empty())
    {
        sf::Image img;
        img.resize({ w, h }, sf::Color::White);
        _texture.loadFromImage(img);
    }
    else
    {
        _texture.loadFromFile(texturePath);
    }
    _sprite.setTexture(_texture, true);
    _sprite.setColor(color);
    sf::Vector2u sizeText = _texture.getSize();

    if (centerOrigin)
        _sprite.setOrigin(sf::Vector2f(sizeText.x / 2, sizeText.y / 2));

    std::cout << "Sprite size: " << w << "x" << h << std::endl;
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
