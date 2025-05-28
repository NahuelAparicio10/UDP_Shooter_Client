#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include "GameObject.h"
#include "UICanvas.h"

class Scene
{
public:    
    virtual ~Scene() {}

    virtual void Update(float dt) = 0;
    virtual void Render(sf::RenderWindow* window) = 0;
    virtual void HandleEvent(const sf::Event& event) = 0;
protected:
    std::vector<GameObject*> _gameObjects;
    UICanvas _canvas;
};

