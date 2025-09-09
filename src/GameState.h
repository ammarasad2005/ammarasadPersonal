#pragma once

enum class GameState {
    MAIN_MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    VICTORY,
    DIFFICULTY_SELECTION,
    EXIT
};

enum class Difficulty {
    EASY = 8,      // 8 wrong guesses allowed
    MEDIUM = 6,    // 6 wrong guesses allowed  
    HARD = 4       // 4 wrong guesses allowed
};