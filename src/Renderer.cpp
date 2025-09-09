#include "Renderer.h"
#include "Game.h"
#include "WordManager.h"
#include <iostream>
#include <cmath>

Renderer::Renderer(sf::RenderWindow& window)
    : window(window)
    , backgroundColor(sf::Color(20, 30, 40))
    , primaryColor(sf::Color(255, 255, 255))
    , secondaryColor(sf::Color(150, 150, 150))
    , accentColor(sf::Color(100, 200, 255))
    , animationTimer(0.0f) {
}

bool Renderer::initialize() {
    return loadAssets();
}

bool Renderer::loadAssets() {
    // Try to load custom fonts, fallback to default
    if (!mainFont.loadFromFile("assets/fonts/main.ttf")) {
        // Use default font - this should work on most systems
        std::cout << "Custom font not found, using default font" << std::endl;
    }
    
    if (!titleFont.loadFromFile("assets/fonts/title.ttf")) {
        titleFont = mainFont; // Use main font as fallback
    }
    
    return true;
}

void Renderer::clear() {
    window.clear(backgroundColor);
}

void Renderer::present() {
    window.display();
}

void Renderer::renderMainMenu() {
    renderBackground();
    
    // Title
    renderCenteredText("HANGMAN GAME", 100.0f, 64, accentColor);
    renderCenteredText("Graphics Edition with SFML", 180.0f, 24, secondaryColor);
    
    // Menu items (will be handled by UIManager)
    renderCenteredText("Use UP/DOWN arrows to navigate", 600.0f, 18, secondaryColor);
    renderCenteredText("Press ENTER to select", 630.0f, 18, secondaryColor);
}

void Renderer::renderGame(const Game& game) {
    renderBackground();
    
    // Hangman drawing
    renderHangman(game.getWrongGuesses(), game.getMaxWrongGuesses());
    
    // Word display area (will be handled by UIManager)
    renderCenteredText("Current Word:", 450.0f, 24, primaryColor);
    
    // Lives display
    renderLives(game.getMaxWrongGuesses() - game.getWrongGuesses(), 
               game.getMaxWrongGuesses(), 800, 100);
    
    // Score display
    renderScore(game.getScore(), 800, 150);
}

void Renderer::renderPauseMenu() {
    renderBackground();
    
    // Semi-transparent overlay
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);
    
    renderCenteredText("GAME PAUSED", 250.0f, 48, accentColor);
}

void Renderer::renderGameOver(const Game& game) {
    renderBackground();
    
    renderCenteredText("GAME OVER", 200.0f, 64, sf::Color::Red);
    
    // Show the correct word
    const WordManager* wm = game.getWordManager();
    if (wm) {
        std::string correctWordText = "The word was: " + wm->getCurrentWord();
        renderCenteredText(correctWordText, 280.0f, 32, sf::Color::Yellow);
    }
    
    renderCenteredText("Final Score: " + std::to_string(game.getScore()), 350.0f, 32, primaryColor);
}

void Renderer::renderVictory(const Game& game) {
    renderBackground();
    
    renderCenteredText("VICTORY!", 200.0f, 64, sf::Color::Green);
    renderCenteredText("Congratulations!", 270.0f, 32, accentColor);
    renderCenteredText("Final Score: " + std::to_string(game.getScore()), 330.0f, 32, primaryColor);
}

void Renderer::renderDifficultyMenu() {
    renderBackground();
    
    renderCenteredText("SELECT DIFFICULTY", 200.0f, 48, accentColor);
    renderCenteredText("Choose your challenge level:", 270.0f, 24, secondaryColor);
}

void Renderer::renderHangman(int wrongGuesses, int maxWrongGuesses) {
    sf::Vector2f pos(HANGMAN_X, HANGMAN_Y);
    
    // Calculate how many parts to draw based on wrong guesses
    float partProgress = static_cast<float>(wrongGuesses) / maxWrongGuesses;
    int partsToShow = static_cast<int>(partProgress * 7); // 7 hangman parts total
    
    // Draw gallows (always visible)
    drawHangmanPart(0, pos, HANGMAN_SCALE);
    
    // Draw hangman parts progressively
    for (int i = 1; i <= partsToShow && i <= 7; i++) {
        drawHangmanPart(i, pos, HANGMAN_SCALE);
    }
}

void Renderer::drawHangmanPart(int part, const sf::Vector2f& position, float scale) {
    sf::Color color = primaryColor;
    
    switch (part) {
        case 0: { // Gallows
            sf::RectangleShape base(sf::Vector2f(100 * scale, 10 * scale));
            base.setPosition(position.x, position.y + 200 * scale);
            base.setFillColor(color);
            window.draw(base);
            
            sf::RectangleShape pole(sf::Vector2f(10 * scale, 200 * scale));
            pole.setPosition(position.x + 20 * scale, position.y);
            pole.setFillColor(color);
            window.draw(pole);
            
            sf::RectangleShape crossbar(sf::Vector2f(60 * scale, 10 * scale));
            crossbar.setPosition(position.x + 20 * scale, position.y);
            crossbar.setFillColor(color);
            window.draw(crossbar);
            
            sf::RectangleShape noose(sf::Vector2f(5 * scale, 30 * scale));
            noose.setPosition(position.x + 75 * scale, position.y + 10 * scale);
            noose.setFillColor(color);
            window.draw(noose);
            break;
        }
        case 1: { // Head
            sf::CircleShape head(15 * scale);
            head.setPosition(position.x + 60 * scale, position.y + 40 * scale);
            head.setFillColor(sf::Color::Transparent);
            head.setOutlineThickness(3 * scale);
            head.setOutlineColor(color);
            window.draw(head);
            break;
        }
        case 2: { // Body
            sf::RectangleShape body(sf::Vector2f(5 * scale, 60 * scale));
            body.setPosition(position.x + 75 * scale, position.y + 70 * scale);
            body.setFillColor(color);
            window.draw(body);
            break;
        }
        case 3: { // Left arm
            sf::RectangleShape leftArm(sf::Vector2f(30 * scale, 5 * scale));
            leftArm.setPosition(position.x + 50 * scale, position.y + 85 * scale);
            leftArm.setFillColor(color);
            window.draw(leftArm);
            break;
        }
        case 4: { // Right arm
            sf::RectangleShape rightArm(sf::Vector2f(30 * scale, 5 * scale));
            rightArm.setPosition(position.x + 80 * scale, position.y + 85 * scale);
            rightArm.setFillColor(color);
            window.draw(rightArm);
            break;
        }
        case 5: { // Left leg
            sf::RectangleShape leftLeg(sf::Vector2f(5 * scale, 30 * scale));
            leftLeg.setPosition(position.x + 65 * scale, position.y + 130 * scale);
            leftLeg.setFillColor(color);
            window.draw(leftLeg);
            break;
        }
        case 6: { // Right leg
            sf::RectangleShape rightLeg(sf::Vector2f(5 * scale, 30 * scale));
            rightLeg.setPosition(position.x + 85 * scale, position.y + 130 * scale);
            rightLeg.setFillColor(color);
            window.draw(rightLeg);
            break;
        }
        case 7: { // Face (X eyes)
            sf::RectangleShape eyeLeft1(sf::Vector2f(8 * scale, 2 * scale));
            eyeLeft1.setPosition(position.x + 68 * scale, position.y + 50 * scale);
            eyeLeft1.setFillColor(sf::Color::Red);
            window.draw(eyeLeft1);
            
            sf::RectangleShape eyeLeft2(sf::Vector2f(2 * scale, 8 * scale));
            eyeLeft2.setPosition(position.x + 71 * scale, position.y + 47 * scale);
            eyeLeft2.setFillColor(sf::Color::Red);
            window.draw(eyeLeft2);
            
            sf::RectangleShape eyeRight1(sf::Vector2f(8 * scale, 2 * scale));
            eyeRight1.setPosition(position.x + 80 * scale, position.y + 50 * scale);
            eyeRight1.setFillColor(sf::Color::Red);
            window.draw(eyeRight1);
            
            sf::RectangleShape eyeRight2(sf::Vector2f(2 * scale, 8 * scale));
            eyeRight2.setPosition(position.x + 83 * scale, position.y + 47 * scale);
            eyeRight2.setFillColor(sf::Color::Red);
            window.draw(eyeRight2);
            break;
        }
    }
}

void Renderer::renderText(const std::string& text, float x, float y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(mainFont);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    sfText.setPosition(x, y);
    window.draw(sfText);
}

void Renderer::renderCenteredText(const std::string& text, float y, int size, sf::Color color) {
    sf::Text sfText;
    sfText.setFont(mainFont);
    sfText.setString(text);
    sfText.setCharacterSize(size);
    sfText.setFillColor(color);
    
    sf::FloatRect textBounds = sfText.getLocalBounds();
    sfText.setPosition((window.getSize().x - textBounds.width) / 2, y);
    window.draw(sfText);
}

void Renderer::renderWordDisplay(const std::string& displayWord, float y) {
    // Create a formatted version with better spacing
    std::string formattedWord;
    for (size_t i = 0; i < displayWord.length(); i++) {
        formattedWord += displayWord[i];
        if (i < displayWord.length() - 1 && displayWord[i] != ' ') {
            formattedWord += "  "; // Add extra spacing between letters
        }
    }
    renderCenteredText(formattedWord, y, 48, primaryColor);
}

void Renderer::renderGuessedLetters(const std::set<char>& letters, float y, bool correct) {
    std::string label = correct ? "Correct Letters: " : "Wrong Letters: ";
    std::string letterStr = label;
    
    if (letters.empty()) {
        letterStr += "(none)";
    } else {
        for (char c : letters) {
            letterStr += c;
            letterStr += " ";
        }
    }
    
    sf::Color color = correct ? sf::Color::Green : sf::Color::Red;
    renderText(letterStr, 50.0f, y, 18, color);
}

void Renderer::renderScore(int score, int x, int y) {
    std::string scoreText = "Score: " + std::to_string(score);
    renderText(scoreText, x, y, 24, primaryColor);
}

void Renderer::renderLives(int lives, int maxLives, int x, int y) {
    std::string livesText = "Lives: " + std::to_string(lives) + "/" + std::to_string(maxLives);
    sf::Color color = (lives <= maxLives / 3) ? sf::Color::Red : primaryColor;
    renderText(livesText, x, y, 24, color);
}

void Renderer::renderButton(const std::string& text, float x, float y, float width, float height, bool highlighted) {
    // Button background
    sf::RectangleShape button(sf::Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(highlighted ? accentColor : sf::Color(60, 60, 60));
    button.setOutlineThickness(2);
    button.setOutlineColor(highlighted ? primaryColor : secondaryColor);
    window.draw(button);
    
    // Button text
    sf::Text buttonText;
    buttonText.setFont(mainFont);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(highlighted ? sf::Color::Black : primaryColor);
    
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(x + (width - textBounds.width) / 2, y + (height - textBounds.height) / 2 - 5);
    window.draw(buttonText);
}

void Renderer::renderBackground() {
    // Simple gradient-like background
    sf::RectangleShape bg(sf::Vector2f(window.getSize().x, window.getSize().y));
    bg.setFillColor(backgroundColor);
    window.draw(bg);
    
    // Add some decorative elements
    for (int i = 0; i < 5; i++) {
        sf::CircleShape decoration(20);
        decoration.setPosition(50 + i * 200, 50);
        decoration.setFillColor(sf::Color(backgroundColor.r + 10, backgroundColor.g + 10, backgroundColor.b + 10, 50));
        window.draw(decoration);
    }
}

void Renderer::renderLetterRevealAnimation(char letter, float x, float y) {
    // Simple letter animation
    float scale = 1.0f + 0.5f * std::sin(animationTimer * 10.0f);
    
    sf::Text letterText;
    letterText.setFont(mainFont);
    letterText.setString(std::string(1, letter));
    letterText.setCharacterSize(static_cast<unsigned>(32 * scale));
    letterText.setFillColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * (1.0f - animationTimer))));
    letterText.setPosition(x, y);
    window.draw(letterText);
}

sf::Vector2f Renderer::getTextBounds(const std::string& text, const sf::Font& font, int size) {
    sf::Text tempText;
    tempText.setFont(font);
    tempText.setString(text);
    tempText.setCharacterSize(size);
    
    sf::FloatRect bounds = tempText.getLocalBounds();
    return sf::Vector2f(bounds.width, bounds.height);
}