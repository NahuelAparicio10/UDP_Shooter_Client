#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class SFXManager
{
public:
    static SFXManager& GetInstance();

    void Play(const std::string& filepath, float volume = 100.f);
    void SetGlobalVolume(float volume);

private:
    SFXManager() = default;
    SFXManager(const SFXManager&) = delete;
    SFXManager& operator=(const SFXManager&) = delete;

    void Cleanup();

    std::unordered_map<std::string, sf::SoundBuffer> _buffers;
    std::vector<sf::Sound> _activeSounds;
    float _globalVolume = 100.f;
};
