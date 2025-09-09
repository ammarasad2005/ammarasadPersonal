#include "UIManager.h"
#include "Game.h"
#include "Renderer.h"
#include "WordManager.h"

UIManager::UIManager(Game& game, Renderer& renderer)
    : game(game)
    , renderer(renderer)
    , selectedMenuItem(0)
    , animationTimer(0.0f)
    , showingAnimation(false)
    , animatedLetter(0)
    , letterCorrect(false)
    , scoreAnimation(0) {
}

void UIManager::setupMainMenu() {
    currentMenu.clear();
    
    currentMenu.emplace_back("Play Game", [this]() { startNewGame(); });
    currentMenu.emplace_back("Difficulty", [this]() { showDifficultyMenu(); });
    currentMenu.emplace_back("Exit", [this]() { exitGame(); });
    
    selectedMenuItem = 0;
}

void UIManager::setupPauseMenu() {
    currentMenu.clear();
    
    currentMenu.emplace_back("Resume", [this]() { resumeGame(); });
    currentMenu.emplace_back("New Game", [this]() { startNewGame(); });
    currentMenu.emplace_back("Main Menu", [this]() { returnToMainMenu(); });
    
    selectedMenuItem = 0;
}

void UIManager::setupDifficultyMenu() {
    currentMenu.clear();
    
    currentMenu.emplace_back("Easy (8 lives)", [this]() { setEasyDifficulty(); });
    currentMenu.emplace_back("Medium (6 lives)", [this]() { setMediumDifficulty(); });
    currentMenu.emplace_back("Hard (4 lives)", [this]() { setHardDifficulty(); });
    
    selectedMenuItem = static_cast<int>(game.getDifficulty()) - 4; // Convert difficulty enum to menu index
}

void UIManager::setupGameOverMenu() {
    currentMenu.clear();
    
    currentMenu.emplace_back("Play Again", [this]() { startNewGame(); });
    currentMenu.emplace_back("Main Menu", [this]() { returnToMainMenu(); });
    currentMenu.emplace_back("Exit", [this]() { exitGame(); });
    
    selectedMenuItem = 0;
}

void UIManager::setupVictoryMenu() {
    currentMenu.clear();
    
    currentMenu.emplace_back("Play Again", [this]() { startNewGame(); });
    currentMenu.emplace_back("Main Menu", [this]() { returnToMainMenu(); });
    currentMenu.emplace_back("Exit", [this]() { exitGame(); });
    
    selectedMenuItem = 0;
}

void UIManager::renderCurrentMenu() {
    renderMenu(currentMenu, selectedMenuItem);
}

void UIManager::renderMenu(const std::vector<MenuItem>& menu, int selected) {
    float startY = MENU_START_Y;
    float centerX = 512.0f; // Assuming 1024px width
    
    for (size_t i = 0; i < menu.size(); i++) {
        float y = startY + i * MENU_ITEM_HEIGHT;
        float x = centerX - BUTTON_WIDTH / 2;
        
        bool highlighted = (static_cast<int>(i) == selected);
        sf::Color textColor = menu[i].enabled ? sf::Color::White : sf::Color(100, 100, 100);
        
        if (!menu[i].enabled) {
            highlighted = false;
        }
        
        renderer.renderButton(menu[i].text, x, y, BUTTON_WIDTH, BUTTON_HEIGHT, highlighted);
    }
}

void UIManager::handleMenuInput(int selection, bool selected) {
    if (selection >= 0 && selection < static_cast<int>(currentMenu.size())) {
        selectedMenuItem = selection;
        
        if (selected && currentMenu[selection].enabled) {
            executeMenuItem(selection);
        }
    }
}

void UIManager::executeMenuItem(int index) {
    if (index >= 0 && index < static_cast<int>(currentMenu.size())) {
        currentMenu[index].action();
    }
}

void UIManager::renderGameHUD(const Game& game) {
    // This method coordinates all game UI rendering
    renderWordArea(game);
    renderGuessedLettersArea(game);
    renderScoreArea(game);
    
    // Show any active animations
    if (showingAnimation) {
        // Render letter animation or other effects
    }
}

void UIManager::renderWordArea(const Game& game) {
    const WordManager* wm = game.getWordManager();
    if (wm) {
        std::string displayWord = wm->getDisplayWord();
        renderer.renderWordDisplay(displayWord, 500.0f);
    }
    
    // Instructions
    renderer.renderCenteredText("Guess a letter:", 560.0f, 24, sf::Color(200, 200, 200));
}

void UIManager::renderGuessedLettersArea(const Game& game) {
    const WordManager* wm = game.getWordManager();
    if (wm) {
        // Render correct and wrong letters
        renderer.renderGuessedLetters(wm->getCorrectLetters(), 600.0f, true);
        renderer.renderGuessedLetters(wm->getWrongLetters(), 630.0f, false);
    }
}

void UIManager::renderScoreArea(const Game& game) {
    renderer.renderScore(game.getScore(), 750, 50);
    
    // Show difficulty
    std::string diffText = "Difficulty: ";
    switch (game.getDifficulty()) {
        case Difficulty::EASY:   diffText += "Easy"; break;
        case Difficulty::MEDIUM: diffText += "Medium"; break;
        case Difficulty::HARD:   diffText += "Hard"; break;
    }
    renderer.renderText(diffText, 750, 200, 18, sf::Color::White);
}

void UIManager::update(float deltaTime) {
    animationTimer += deltaTime;
    
    // Update any active animations
    if (showingAnimation) {
        if (animationTimer > 1.0f) { // Animation duration
            showingAnimation = false;
            animationTimer = 0.0f;
        }
    }
}

void UIManager::showLetterAnimation(char letter, bool correct) {
    animatedLetter = letter;
    letterCorrect = correct;
    showingAnimation = true;
    animationTimer = 0.0f;
}

void UIManager::showScoreAnimation(int points) {
    scoreAnimation = points;
    showingAnimation = true;
    animationTimer = 0.0f;
}

void UIManager::setMenuSelection(int selection) {
    if (selection >= 0 && selection < static_cast<int>(currentMenu.size())) {
        selectedMenuItem = selection;
    }
}

// Menu action implementations
void UIManager::startNewGame() {
    game.newGame();
}

void UIManager::showDifficultyMenu() {
    game.setState(GameState::DIFFICULTY_SELECTION);
}

void UIManager::pauseGame() {
    game.pauseGame();
}

void UIManager::resumeGame() {
    game.resumeGame();
}

void UIManager::returnToMainMenu() {
    game.setState(GameState::MAIN_MENU);
}

void UIManager::exitGame() {
    game.setState(GameState::EXIT);
}

void UIManager::setEasyDifficulty() {
    game.setDifficulty(Difficulty::EASY);
    startNewGame();
}

void UIManager::setMediumDifficulty() {
    game.setDifficulty(Difficulty::MEDIUM);
    startNewGame();
}

void UIManager::setHardDifficulty() {
    game.setDifficulty(Difficulty::HARD);
    startNewGame();
}