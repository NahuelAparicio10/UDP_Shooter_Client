#include "Window.h"

Window::Window()
{
    _window = new sf::RenderWindow(sf::VideoMode({ Constants::WIDTH, Constants::HEIGHT }), "Shooter");
}

Window::~Window() { _window->close(); }

bool Window::IsOpen() const { return _window->isOpen(); }

void Window::Clear() { _window->clear(sf::Color::Black); }

void Window::Display() { _window->display(); }

void Window::Close() { _window->close(); }

sf::RenderWindow* Window::GetWindow() { return _window; }