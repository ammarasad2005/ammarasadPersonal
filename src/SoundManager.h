#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>

enum class SoundEffect {
    CORRECT_GUESS,
    WRONG_GUESS,
    VICTORY,
    GAME_OVER,
    MENU_SELECT,
    MENU_NAVIGATE,
    LETTER_REVEAL,
    NEW_GAME
};

class SoundManager {
public:
    SoundManager();
    ~SoundManager() = default;
    
    // Initialization
    bool initialize();
    
    // Sound effects
    void playSound(SoundEffect effect);
    void stopAllSounds();
    
    // Background music
    void playBackgroundMusic();
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    
    // Volume control
    void setSoundVolume(float volume);          // 0.0f to 100.0f
    void setMusicVolume(float volume);          // 0.0f to 100.0f
    float getSoundVolume() const { return soundVolume; }
    float getMusicVolume() const { return musicVolume; }
    
    // Mute control
    void muteSounds(bool mute);
    void muteMusic(bool mute);
    bool isSoundsMuted() const { return soundsMuted; }
    bool isMusicMuted() const { return musicMuted; }
    
    // Update (for managing sound instances)
    void update();

private:
    // Audio resources
    std::map<SoundEffect, sf::SoundBuffer> soundBuffers;
    std::map<SoundEffect, std::unique_ptr<sf::Sound>> sounds;
    sf::Music backgroundMusic;
    
    // Volume settings
    float soundVolume;
    float musicVolume;
    bool soundsMuted;
    bool musicMuted;
    
    // Helper methods
    bool loadSoundBuffers();
    void createSoundInstances();
    std::string getSoundFilePath(SoundEffect effect) const;
    
    // Simple sound generation (fallback if no sound files available)
    void generateDefaultSounds();
    sf::SoundBuffer generateTone(float frequency, float duration, float volume = 50.0f);
};