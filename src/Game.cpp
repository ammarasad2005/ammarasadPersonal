#include "Game.h"
#include "WordManager.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "UIManager.h"
#include "SoundManager.h"
#include <iostream>

Game::Game()
    : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Hangman Game - SFML Edition")
    , currentState(GameState::MAIN_MENU)
    , currentDifficulty(Difficulty::MEDIUM)
    , score(0)
    , wrongGuesses(0)
    , gameWon(false)
    , animationTimer(0.0f) {
    
    window.setFramerateLimit(TARGET_FPS);
    initializeGame();
}

Game::~Game() = default;

void Game::initializeGame() {
    // Initialize components
    wordManager = std::make_unique<WordManager>();
    renderer = std::make_unique<Renderer>(window);
    inputHandler = std::make_unique<InputHandler>(*this);
    uiManager = std::make_unique<UIManager>(*this, *renderer);
    soundManager = std::make_unique<SoundManager>();
    
    // Initialize systems
    if (!renderer->initialize()) {
        throw std::runtime_error("Failed to initialize renderer");
    }
    
    if (!soundManager->initialize()) {
        std::cout << "Warning: Failed to initialize sound system" << std::endl;
    }
    
    // Load words
    if (!wordManager->loadWordsFromFile("assets/words.txt")) {
        std::cout << "Warning: Failed to load words file, using defaults" << std::endl;
    }
    
    // Setup initial menu
    uiManager->setupMainMenu();
    inputHandler->setMenuItemCount(uiManager->getMenuItemCount());
    
    // Start background music
    soundManager->playBackgroundMusic();
}

void Game::run() {
    while (currentState != GameState::EXIT && window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        inputHandler->handleEvent(event);
    }
}

void Game::update(float deltaTime) {
    inputHandler->update();
    uiManager->update(deltaTime);
    soundManager->update();
    
    animationTimer += deltaTime;
    
    // Handle state-specific updates
    switch (currentState) {
        case GameState::MAIN_MENU:
            if (inputHandler->wasMenuSelectionChanged()) {
                uiManager->setMenuSelection(inputHandler->getMenuSelection());
                soundManager->playSound(SoundEffect::MENU_NAVIGATE);
                inputHandler->clearMenuInput();
            }
            if (inputHandler->wasMenuItemSelected()) {
                uiManager->handleMenuInput(inputHandler->getMenuSelection(), true);
                soundManager->playSound(SoundEffect::MENU_SELECT);
                inputHandler->clearMenuInput();
            }
            break;
            
        case GameState::PLAYING:
            if (inputHandler->hasNewLetterInput()) {
                char letter = inputHandler->getLastPressedLetter();
                handleGameInput(letter);
                inputHandler->clearLetterInput();
            }
            break;
            
        case GameState::PAUSED:
        case GameState::GAME_OVER:
        case GameState::VICTORY:
        case GameState::DIFFICULTY_SELECTION:
            if (inputHandler->wasMenuSelectionChanged()) {
                uiManager->setMenuSelection(inputHandler->getMenuSelection());
                soundManager->playSound(SoundEffect::MENU_NAVIGATE);
                inputHandler->clearMenuInput();
            }
            if (inputHandler->wasMenuItemSelected()) {
                uiManager->handleMenuInput(inputHandler->getMenuSelection(), true);
                soundManager->playSound(SoundEffect::MENU_SELECT);
                inputHandler->clearMenuInput();
            }
            break;
            
        default:
            break;
    }
}

void Game::render() {
    renderer->clear();
    
    switch (currentState) {
        case GameState::MAIN_MENU:
            renderer->renderMainMenu();
            uiManager->renderCurrentMenu();
            break;
            
        case GameState::PLAYING:
            renderer->renderGame(*this);
            uiManager->renderGameHUD(*this);
            break;
            
        case GameState::PAUSED:
            renderer->renderGame(*this);
            uiManager->renderGameHUD(*this);
            renderer->renderPauseMenu();
            uiManager->renderCurrentMenu();
            break;
            
        case GameState::GAME_OVER:
            renderer->renderGameOver(*this);
            uiManager->renderCurrentMenu();
            break;
            
        case GameState::VICTORY:
            renderer->renderVictory(*this);
            uiManager->renderCurrentMenu();
            break;
            
        case GameState::DIFFICULTY_SELECTION:
            renderer->renderDifficultyMenu();
            uiManager->renderCurrentMenu();
            break;
            
        default:
            break;
    }
    
    renderer->present();
}

void Game::setState(GameState newState) {
    currentState = newState;
    
    // Setup UI for new state
    switch (newState) {
        case GameState::MAIN_MENU:
            uiManager->setupMainMenu();
            inputHandler->setMenuItemCount(uiManager->getMenuItemCount());
            break;
            
        case GameState::PAUSED:
            uiManager->setupPauseMenu();
            inputHandler->setMenuItemCount(uiManager->getMenuItemCount());
            break;
            
        case GameState::GAME_OVER:
            uiManager->setupGameOverMenu();
            inputHandler->setMenuItemCount(uiManager->getMenuItemCount());
            soundManager->playSound(SoundEffect::GAME_OVER);
            break;
            
        case GameState::VICTORY:
            uiManager->setupVictoryMenu();
            inputHandler->setMenuItemCount(uiManager->getMenuItemCount());
            soundManager->playSound(SoundEffect::VICTORY);
            break;
            
        case GameState::DIFFICULTY_SELECTION:
            uiManager->setupDifficultyMenu();
            inputHandler->setMenuItemCount(uiManager->getMenuItemCount());
            break;
            
        case GameState::EXIT:
            window.close();
            break;
            
        default:
            break;
    }
}

void Game::newGame() {
    score = 0;
    wrongGuesses = 0;
    gameWon = false;
    
    wordManager->reset();
    wordManager->selectRandomWord();
    
    setState(GameState::PLAYING);
    soundManager->playSound(SoundEffect::NEW_GAME);
}

void Game::pauseGame() {
    if (currentState == GameState::PLAYING) {
        setState(GameState::PAUSED);
    }
}

void Game::resumeGame() {
    if (currentState == GameState::PAUSED) {
        setState(GameState::PLAYING);
    }
}

void Game::resetGame() {
    newGame();
}

void Game::setDifficulty(Difficulty difficulty) {
    currentDifficulty = difficulty;
}

bool Game::guessLetter(char letter) {
    if (currentState != GameState::PLAYING) {
        return false;
    }
    
    return wordManager->guessLetter(letter);
}

bool Game::hasGuessedLetter(char letter) const {
    return wordManager->hasGuessedLetter(letter);
}

void Game::handleGameInput(char letter) {
    bool correct = wordManager->guessLetter(letter);
    
    if (correct) {
        soundManager->playSound(SoundEffect::CORRECT_GUESS);
        uiManager->showLetterAnimation(letter, true);
        
        // Award points
        score += 10;
        
        // Check for victory
        if (wordManager->isWordComplete()) {
            gameWon = true;
            score += 50; // Bonus for completing word
            checkGameEnd();
        }
    } else {
        soundManager->playSound(SoundEffect::WRONG_GUESS);
        uiManager->showLetterAnimation(letter, false);
        wrongGuesses++;
        
        checkGameEnd();
    }
}

void Game::checkGameEnd() {
    if (gameWon) {
        setState(GameState::VICTORY);
    } else if (wrongGuesses >= getMaxWrongGuesses()) {
        setState(GameState::GAME_OVER);
    }
}