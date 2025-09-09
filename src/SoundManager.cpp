#include "SoundManager.h"
#include <iostream>
#include <cmath>
#include <fstream>

SoundManager::SoundManager() 
    : soundVolume(75.0f)
    , musicVolume(50.0f)
    , soundsMuted(false)
    , musicMuted(false) {
}

bool SoundManager::initialize() {
    // Try to load sound files, fallback to generated sounds
    if (!loadSoundBuffers()) {
        std::cout << "Sound files not found, generating default sounds..." << std::endl;
        generateDefaultSounds();
    }
    
    createSoundInstances();
    
    // Try to load background music
    if (!backgroundMusic.openFromFile("assets/sounds/background.ogg")) {
        std::cout << "Background music file not found" << std::endl;
    }
    
    return true;
}

void SoundManager::playSound(SoundEffect effect) {
    if (soundsMuted || sounds.find(effect) == sounds.end()) {
        return;
    }
    
    auto& sound = sounds[effect];
    if (sound) {
        sound->setVolume(soundVolume);
        sound->play();
    }
}

void SoundManager::stopAllSounds() {
    for (auto& pair : sounds) {
        if (pair.second) {
            pair.second->stop();
        }
    }
}

void SoundManager::playBackgroundMusic() {
    if (musicMuted) {
        return;
    }
    
    backgroundMusic.setVolume(musicVolume);
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
}

void SoundManager::stopBackgroundMusic() {
    backgroundMusic.stop();
}

void SoundManager::pauseBackgroundMusic() {
    backgroundMusic.pause();
}

void SoundManager::resumeBackgroundMusic() {
    if (!musicMuted) {
        backgroundMusic.play();
    }
}

void SoundManager::setSoundVolume(float volume) {
    soundVolume = std::max(0.0f, std::min(100.0f, volume));
}

void SoundManager::setMusicVolume(float volume) {
    musicVolume = std::max(0.0f, std::min(100.0f, volume));
    backgroundMusic.setVolume(musicMuted ? 0.0f : musicVolume);
}

void SoundManager::muteSounds(bool mute) {
    soundsMuted = mute;
}

void SoundManager::muteMusic(bool mute) {
    musicMuted = mute;
    backgroundMusic.setVolume(mute ? 0.0f : musicVolume);
}

void SoundManager::update() {
    // Clean up finished sounds (optional, SFML handles this automatically)
}

bool SoundManager::loadSoundBuffers() {
    const std::vector<SoundEffect> effects = {
        SoundEffect::CORRECT_GUESS,
        SoundEffect::WRONG_GUESS,
        SoundEffect::VICTORY,
        SoundEffect::GAME_OVER,
        SoundEffect::MENU_SELECT,
        SoundEffect::MENU_NAVIGATE,
        SoundEffect::LETTER_REVEAL,
        SoundEffect::NEW_GAME
    };
    
    bool allLoaded = true;
    
    for (SoundEffect effect : effects) {
        std::string filename = getSoundFilePath(effect);
        if (!soundBuffers[effect].loadFromFile(filename)) {
            allLoaded = false;
        }
    }
    
    return allLoaded;
}

void SoundManager::createSoundInstances() {
    for (const auto& pair : soundBuffers) {
        sounds[pair.first] = std::make_unique<sf::Sound>(pair.second);
    }
}

std::string SoundManager::getSoundFilePath(SoundEffect effect) const {
    switch (effect) {
        case SoundEffect::CORRECT_GUESS:    return "assets/sounds/correct.wav";
        case SoundEffect::WRONG_GUESS:      return "assets/sounds/wrong.wav";
        case SoundEffect::VICTORY:          return "assets/sounds/victory.wav";
        case SoundEffect::GAME_OVER:        return "assets/sounds/gameover.wav";
        case SoundEffect::MENU_SELECT:      return "assets/sounds/select.wav";
        case SoundEffect::MENU_NAVIGATE:    return "assets/sounds/navigate.wav";
        case SoundEffect::LETTER_REVEAL:    return "assets/sounds/reveal.wav";
        case SoundEffect::NEW_GAME:         return "assets/sounds/newgame.wav";
        default:                            return "";
    }
}

void SoundManager::generateDefaultSounds() {
    // Generate simple tones for different sound effects
    soundBuffers[SoundEffect::CORRECT_GUESS] = generateTone(800.0f, 0.3f, 60.0f);
    soundBuffers[SoundEffect::WRONG_GUESS] = generateTone(200.0f, 0.5f, 80.0f);
    soundBuffers[SoundEffect::VICTORY] = generateTone(1200.0f, 1.0f, 70.0f);
    soundBuffers[SoundEffect::GAME_OVER] = generateTone(150.0f, 1.5f, 90.0f);
    soundBuffers[SoundEffect::MENU_SELECT] = generateTone(600.0f, 0.2f, 50.0f);
    soundBuffers[SoundEffect::MENU_NAVIGATE] = generateTone(400.0f, 0.1f, 40.0f);
    soundBuffers[SoundEffect::LETTER_REVEAL] = generateTone(500.0f, 0.2f, 55.0f);
    soundBuffers[SoundEffect::NEW_GAME] = generateTone(1000.0f, 0.7f, 65.0f);
}

sf::SoundBuffer SoundManager::generateTone(float frequency, float duration, float volume) {
    const unsigned sampleRate = 44100;
    const unsigned sampleCount = static_cast<unsigned>(sampleRate * duration);
    
    std::vector<sf::Int16> samples(sampleCount);
    
    for (unsigned i = 0; i < sampleCount; i++) {
        float time = static_cast<float>(i) / sampleRate;
        float envelope = 1.0f;
        
        // Simple envelope to avoid clicking
        if (time < 0.01f) {
            envelope = time / 0.01f;
        } else if (time > duration - 0.01f) {
            envelope = (duration - time) / 0.01f;
        }
        
        float sample = std::sin(frequency * 2.0f * M_PI * time) * envelope * volume / 100.0f;
        samples[i] = static_cast<sf::Int16>(sample * 32767);
    }
    
    sf::SoundBuffer buffer;
    buffer.loadFromSamples(samples.data(), sampleCount, 1, sampleRate);
    return buffer;
}