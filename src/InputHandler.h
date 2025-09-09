#pragma once
#include <SFML/Window.hpp>
#include <functional>
#include <map>

class Game;

class InputHandler {
public:
    InputHandler(Game& game);
    ~InputHandler() = default;
    
    // Event processing
    void handleEvent(const sf::Event& event);
    void update();
    
    // Input state
    bool isKeyPressed(sf::Keyboard::Key key) const;
    bool wasKeyJustPressed(sf::Keyboard::Key key) const;
    
    // Letter input
    char getLastPressedLetter() const { return lastPressedLetter; }
    bool hasNewLetterInput() const { return newLetterInput; }
    void clearLetterInput() { newLetterInput = false; lastPressedLetter = 0; }
    
    // Menu navigation
    int getMenuSelection() const { return menuSelection; }
    bool wasMenuSelectionChanged() const { return menuSelectionChanged; }
    bool wasMenuItemSelected() const { return menuItemSelected; }
    void clearMenuInput() { menuSelectionChanged = false; menuItemSelected = false; }
    
    // Menu configuration
    void setMenuItemCount(int count);
    
private:
    Game& game;
    
    // Input state tracking
    std::map<sf::Keyboard::Key, bool> currentKeys;
    std::map<sf::Keyboard::Key, bool> previousKeys;
    
    // Letter input
    char lastPressedLetter;
    bool newLetterInput;
    
    // Menu navigation
    int menuSelection;
    int maxMenuItems;
    bool menuSelectionChanged;
    bool menuItemSelected;
    
    // Helper methods
    void handleKeyPressed(sf::Keyboard::Key key);
    void handleKeyReleased(sf::Keyboard::Key key);
    void handleTextEntered(sf::Uint32 unicode);
    
    void updateKeyStates();
    bool isLetter(sf::Keyboard::Key key) const;
    char keyToChar(sf::Keyboard::Key key) const;
    
    void handleMenuNavigation(sf::Keyboard::Key key);
    void handleGameInput(sf::Keyboard::Key key);
};