#pragma once
#include <SFML/Graphics.hpp>
#define WIDTH 1080
#define HEIGHT 720

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

