#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "GameState.h"

class WordManager;
class Renderer;
class InputHandler;
class UIManager;
class SoundManager;

class Game {
public:
    Game();
    ~Game();
    
    void run();
    
    // State management
    void setState(GameState newState);
    GameState getState() const { return currentState; }
    
    // Game control
    void newGame();
    void pauseGame();
    void resumeGame();
    void resetGame();
    
    // Settings
    void setDifficulty(Difficulty difficulty);
    Difficulty getDifficulty() const { return currentDifficulty; }
    
    // Score and stats
    int getScore() const { return score; }
    int getWrongGuesses() const { return wrongGuesses; }
    int getMaxWrongGuesses() const { return static_cast<int>(currentDifficulty); }
    
    // Letter guessing
    bool guessLetter(char letter);
    bool hasGuessedLetter(char letter) const;
    
    // Word access for UI
    const WordManager* getWordManager() const { return wordManager.get(); }
    
    // Window management
    sf::RenderWindow& getWindow() { return window; }

private:
    void processEvents();
    void update(float deltaTime);
    void render();
    
    void initializeGame();
    void handleGameInput(char letter);
    void checkGameEnd();
    
    // SFML components
    sf::RenderWindow window;
    sf::Clock clock;
    
    // Game state
    GameState currentState;
    Difficulty currentDifficulty;
    
    // Game data
    int score;
    int wrongGuesses;
    bool gameWon;
    
    // Components
    std::unique_ptr<WordManager> wordManager;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<InputHandler> inputHandler;
    std::unique_ptr<UIManager> uiManager;
    std::unique_ptr<SoundManager> soundManager;
    
    // Timing
    float animationTimer;
    
    // Constants
    static constexpr int WINDOW_WIDTH = 1024;
    static constexpr int WINDOW_HEIGHT = 768;
    static constexpr int TARGET_FPS = 60;
};