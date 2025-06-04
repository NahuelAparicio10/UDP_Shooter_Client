#pragma once
#include "SFXManager.h"
#include <stdexcept>
#include "Constants.h"
#include <iostream>

SFXManager& SFXManager::GetInstance()
{
    static SFXManager instance;
    return instance;
}

void SFXManager::Play(const std::string& filepath, float volume)
{
    Cleanup();

    if (_buffers.find(filepath) == _buffers.end()) {
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile(filepath)) {
            throw std::runtime_error("Failed to load SFX file: " + filepath);
        }
        _buffers[filepath] = buffer;
    }

    _activeSounds.emplace_back(_buffers[filepath]);
    _activeSounds.back().setVolume(volume * (_globalVolume / 100.f));
    _activeSounds.back().play();
}

void SFXManager::SetGlobalVolume(float volume)
{
    _globalVolume = volume;
}

void SFXManager::Cleanup()
{
    _activeSounds.erase(
        std::remove_if(_activeSounds.begin(), _activeSounds.end(),
            [](const sf::Sound& s) {
                return s.getStatus() == sf::Sound::Status::Stopped;
            }),
        _activeSounds.end());
}