#pragma once
#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <iostream>
class FontManager {
public:
    static sf::Font& GetMainFont()
    {
        static sf::Font font;
        static bool loaded = false;

        if (!loaded)
        {
            if (!font.openFromFile("Assets/Fonts/Poppins-Bold.ttf"))
            {
                std::cerr << "Error: No se pudo cargar la fuente.\n";
                throw std::runtime_error("Failed to load font: Assets/Fonts/Poppins-Bold.ttf");
            }
            loaded = true;
        }

        return font;
    }
};
