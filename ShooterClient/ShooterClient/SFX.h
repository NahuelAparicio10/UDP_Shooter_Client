#pragma once
#include <SFML/Audio.hpp>
#include "Component.h"

class SFX : public Component
{
public:
    SFX(const std::string& filepath);

    void Play();
    bool IsFinished() const;

    virtual const std::type_index GetType() override;

private:
    sf::SoundBuffer _buffer;
    sf::Sound _sound;
    bool _hasPlayed = false;
};


