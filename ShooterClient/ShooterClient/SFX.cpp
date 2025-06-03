#include "SFX.h"

//SFX::SFX(const std::string& filepath)
//{
//    if (!_buffer.loadFromFile(filepath)) {
//        throw std::runtime_error("Failed to load SFX file: " + filepath);
//    }
//
//    _sound.setBuffer(_buffer);
//}
//
//void SFX::Play()
//{
//    if (!_hasPlayed) {
//        _sound.play();
//        _hasPlayed = true;
//    }
//}
//
//bool SFX::IsFinished() const
//{
//    return _sound.getStatus() == sf::SoundSource::stop;
//}
//
//const std::type_index SFX::GetType()
//{
//    return typeid(SFX);
//}
