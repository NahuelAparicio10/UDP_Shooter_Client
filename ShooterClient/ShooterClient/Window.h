#pragma once
#include <SFML/Graphics.hpp>
#include "Constants.h"

class Window
{
public:
    Window();
    ~Window();

    bool IsOpen() const;
    void Clear();
    void Display();
    void Close();

    sf::RenderWindow* GetWindow();

private:
    sf::RenderWindow* _window;
};

