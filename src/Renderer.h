#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include <set>
#include <string>

class Game;
class WordManager;

class Renderer {
public:
    Renderer(sf::RenderWindow& window);
    ~Renderer() = default;
    
    // Initialization
    bool initialize();
    
    // Main rendering methods
    void clear();
    void present();
    
    // Game rendering
    void renderMainMenu();
    void renderGame(const Game& game);
    void renderPauseMenu();
    void renderGameOver(const Game& game);
    void renderVictory(const Game& game);
    void renderDifficultyMenu();
    
    // Hangman drawing
    void renderHangman(int wrongGuesses, int maxWrongGuesses);
    
    // UI elements
    void renderText(const std::string& text, float x, float y, int size = 24, sf::Color color = sf::Color::White);
    void renderCenteredText(const std::string& text, float y, int size = 24, sf::Color color = sf::Color::White);
    void renderWordDisplay(const std::string& displayWord, float y);
    void renderGuessedLetters(const std::set<char>& letters, float y, bool correct = true);
    void renderScore(int score, int x, int y);
    void renderLives(int lives, int maxLives, int x, int y);
    
    // Button rendering
    void renderButton(const std::string& text, float x, float y, float width, float height, bool highlighted = false);
    
    // Animation support
    void setAnimationTimer(float timer) { animationTimer = timer; }
    
    // Background and effects
    void renderBackground();
    void renderLetterRevealAnimation(char letter, float x, float y);
    
private:
    sf::RenderWindow& window;
    
    // Fonts and textures
    sf::Font mainFont;
    sf::Font titleFont;
    std::map<std::string, sf::Texture> textures;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color primaryColor;
    sf::Color secondaryColor;
    sf::Color accentColor;
    
    // Animation
    float animationTimer;
    
    // Helper methods
    bool loadAssets();
    void drawHangmanPart(int part, const sf::Vector2f& position, float scale = 1.0f);
    sf::Vector2f getTextBounds(const std::string& text, const sf::Font& font, int size);
    
    // Constants
    static constexpr float HANGMAN_X = 150.0f;
    static constexpr float HANGMAN_Y = 200.0f;
    static constexpr float HANGMAN_SCALE = 2.0f;
};