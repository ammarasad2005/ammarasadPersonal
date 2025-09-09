#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "GameState.h"

struct MenuItem {
    std::string text;
    std::function<void()> action;
    bool enabled;
    
    MenuItem(const std::string& t, std::function<void()> a, bool e = true)
        : text(t), action(a), enabled(e) {}
};

class Game;
class Renderer;

class UIManager {
public:
    UIManager(Game& game, Renderer& renderer);
    ~UIManager() = default;
    
    // Menu management
    void setupMainMenu();
    void setupPauseMenu();
    void setupDifficultyMenu();
    void setupGameOverMenu();
    void setupVictoryMenu();
    
    // Menu rendering and interaction
    void renderCurrentMenu();
    void handleMenuInput(int selection, bool selected);
    
    // Menu state
    int getMenuItemCount() const { return currentMenu.size(); }
    const std::vector<MenuItem>& getCurrentMenu() const { return currentMenu; }
    
    // Game UI
    void renderGameHUD(const Game& game);
    void renderWordArea(const Game& game);
    void renderGuessedLettersArea(const Game& game);
    void renderScoreArea(const Game& game);
    
    // Animations and effects
    void update(float deltaTime);
    void showLetterAnimation(char letter, bool correct);
    void showScoreAnimation(int points);
    
    // Utility
    void setMenuSelection(int selection);
    int getMenuSelection() const { return selectedMenuItem; }

private:
    Game& game;
    Renderer& renderer;
    
    // Menu system
    std::vector<MenuItem> currentMenu;
    int selectedMenuItem;
    
    // Animation state
    float animationTimer;
    bool showingAnimation;
    char animatedLetter;
    bool letterCorrect;
    int scoreAnimation;
    
    // UI layout constants
    static constexpr float MENU_START_Y = 300.0f;
    static constexpr float MENU_ITEM_HEIGHT = 60.0f;
    static constexpr float BUTTON_WIDTH = 300.0f;
    static constexpr float BUTTON_HEIGHT = 50.0f;
    
    // Helper methods
    void executeMenuItem(int index);
    void renderMenu(const std::vector<MenuItem>& menu, int selected);
    void renderGameStats(const Game& game);
    
    // Menu actions
    void startNewGame();
    void showDifficultyMenu();
    void pauseGame();
    void resumeGame();
    void returnToMainMenu();
    void exitGame();
    void setEasyDifficulty();
    void setMediumDifficulty();  
    void setHardDifficulty();
};