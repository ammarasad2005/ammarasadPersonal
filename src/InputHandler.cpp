#include "InputHandler.h"
#include "Game.h"
#include <cctype>

InputHandler::InputHandler(Game& game)
    : game(game)
    , lastPressedLetter(0)
    , newLetterInput(false)
    , menuSelection(0)
    , maxMenuItems(3)
    , menuSelectionChanged(false)
    , menuItemSelected(false) {
}

void InputHandler::handleEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::KeyPressed:
            handleKeyPressed(event.key.code);
            break;
            
        case sf::Event::KeyReleased:
            handleKeyReleased(event.key.code);
            break;
            
        case sf::Event::TextEntered:
            handleTextEntered(event.text.unicode);
            break;
            
        case sf::Event::Closed:
            game.setState(GameState::EXIT);
            break;
            
        default:
            break;
    }
}

void InputHandler::update() {
    updateKeyStates();
}

void InputHandler::handleKeyPressed(sf::Keyboard::Key key) {
    currentKeys[key] = true;
    
    GameState state = game.getState();
    
    switch (state) {
        case GameState::MAIN_MENU:
        case GameState::PAUSED:
        case GameState::GAME_OVER:
        case GameState::VICTORY:
        case GameState::DIFFICULTY_SELECTION:
            handleMenuNavigation(key);
            break;
            
        case GameState::PLAYING:
            handleGameInput(key);
            break;
            
        default:
            break;
    }
}

void InputHandler::handleKeyReleased(sf::Keyboard::Key key) {
    currentKeys[key] = false;
}

void InputHandler::handleTextEntered(sf::Uint32 unicode) {
    // Handle text input for letter guessing
    if (game.getState() == GameState::PLAYING) {
        if (unicode >= 'a' && unicode <= 'z') {
            char letter = static_cast<char>(unicode - 'a' + 'A'); // Convert to uppercase
            if (!game.hasGuessedLetter(letter)) {
                lastPressedLetter = letter;
                newLetterInput = true;
            }
        } else if (unicode >= 'A' && unicode <= 'Z') {
            char letter = static_cast<char>(unicode);
            if (!game.hasGuessedLetter(letter)) {
                lastPressedLetter = letter;
                newLetterInput = true;
            }
        }
    }
}

void InputHandler::handleMenuNavigation(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Up:
            menuSelection = (menuSelection - 1 + maxMenuItems) % maxMenuItems;
            menuSelectionChanged = true;
            break;
            
        case sf::Keyboard::Down:
            menuSelection = (menuSelection + 1) % maxMenuItems;
            menuSelectionChanged = true;
            break;
            
        case sf::Keyboard::Return:
        case sf::Keyboard::Space:
            menuItemSelected = true;
            break;
            
        case sf::Keyboard::Escape:
            if (game.getState() == GameState::PAUSED) {
                game.resumeGame();
            } else if (game.getState() != GameState::MAIN_MENU) {
                game.setState(GameState::MAIN_MENU);
            }
            break;
            
        default:
            break;
    }
}

void InputHandler::handleGameInput(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Escape:
            game.pauseGame();
            break;
            
        case sf::Keyboard::R:
            // Quick restart
            game.newGame();
            break;
            
        default:
            // Letter input is handled by TextEntered event
            break;
    }
}

void InputHandler::updateKeyStates() {
    // Update previous key states for edge detection
    previousKeys = currentKeys;
}

bool InputHandler::isKeyPressed(sf::Keyboard::Key key) const {
    auto it = currentKeys.find(key);
    return (it != currentKeys.end()) ? it->second : false;
}

bool InputHandler::wasKeyJustPressed(sf::Keyboard::Key key) const {
    auto currentIt = currentKeys.find(key);
    auto previousIt = previousKeys.find(key);
    
    bool currentPressed = (currentIt != currentKeys.end()) ? currentIt->second : false;
    bool previousPressed = (previousIt != previousKeys.end()) ? previousIt->second : false;
    
    return currentPressed && !previousPressed;
}

bool InputHandler::isLetter(sf::Keyboard::Key key) const {
    return key >= sf::Keyboard::A && key <= sf::Keyboard::Z;
}

char InputHandler::keyToChar(sf::Keyboard::Key key) const {
    if (isLetter(key)) {
        return 'A' + (key - sf::Keyboard::A);
    }
    return 0;
}

void InputHandler::setMenuItemCount(int count) {
    maxMenuItems = count;
    if (menuSelection >= maxMenuItems) {
        menuSelection = 0;
    }
}