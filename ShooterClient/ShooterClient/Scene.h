#pragma once
#include <vector>
#include "GameObject.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Scene
{
public:    
    virtual ~Scene() {}

    virtual void Update(float dt) = 0;
    virtual void Render(sf::RenderWindow& window) = 0;
    virtual void HandleEvent(const sf::Event& event) = 0;
};

