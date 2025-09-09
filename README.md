# Hangman Game - SFML Graphics Edition

A modern, graphics-based implementation of the classic Hangman game built with C++ and SFML.

![Hangman Game](https://img.shields.io/badge/Game-Hangman-blue)
![C++](https://img.shields.io/badge/C++-17-00599C?logo=c%2B%2B)
![SFML](https://img.shields.io/badge/Graphics-SFML-green)

## Features

### Core Features
- **Graphics-Based Interface**: Modern SFML-powered graphics with smooth rendering
- **File-Based Word Management**: Dynamic word loading from external text files
- **Interactive Hangman Drawing**: Progressive hangman visualization based on wrong guesses
- **Multiple Difficulty Levels**: Easy (8 lives), Medium (6 lives), Hard (4 lives)
- **Sound Effects**: Audio feedback for correct/incorrect guesses, victories, and defeats
- **Score Tracking**: Points system rewarding correct guesses and word completion

### Game Mechanics
- **Keyboard Input**: Letter guessing via keyboard input
- **Real-Time Feedback**: Immediate visual and audio feedback for guesses
- **Word Categories**: Programming and computer science themed words
- **Pause Functionality**: Ability to pause and resume games
- **Menu System**: Intuitive navigation through game options

### Technical Features
- **Object-Oriented Design**: Clean separation of concerns with dedicated classes
- **Modern C++17**: Utilizes modern C++ practices and standards
- **Cross-Platform**: Runs on any system with SFML support
- **Extensible**: Easy to add new words, sounds, and graphics
- **Error Handling**: Robust file handling and error management

## Building and Running

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.16 or later  
- SFML 2.5 or later

### Ubuntu/Debian
```bash
sudo apt update
sudo apt install libsfml-dev cmake g++
```

### Build Instructions
```bash
# Clone and navigate to directory
cd ammarasadPersonal

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j4

# Run the game
./hangman
```

## How to Play

### Main Menu
- Use **UP/DOWN** arrow keys to navigate menu options
- Press **ENTER** to select menu items
- Choose "Play Game" to start a new game
- Select "Difficulty" to change challenge level

### During Gameplay
- Type any letter (A-Z) to make a guess
- Correct letters will appear in the word
- Wrong letters will advance the hangman drawing
- Press **ESC** to pause the game
- Press **R** to restart the current game

### Difficulty Levels
- **Easy**: 8 wrong guesses allowed
- **Medium**: 6 wrong guesses allowed  
- **Hard**: 4 wrong guesses allowed

### Scoring System
- +10 points for each correct letter
- +50 bonus points for completing a word
- No points deducted for wrong guesses

## Game Controls

| Key | Action |
|-----|--------|
| **A-Z** | Guess a letter |
| **↑/↓** | Navigate menus |
| **Enter** | Select menu item |
| **ESC** | Pause game / Return to menu |
| **R** | Restart current game |
| **Space** | Alternative select button |

## Project Structure

```
├── src/                    # Source code
│   ├── main.cpp           # Entry point
│   ├── Game.cpp/h         # Main game logic and state management
│   ├── WordManager.cpp/h  # Word loading and game logic
│   ├── Renderer.cpp/h     # Graphics rendering
│   ├── InputHandler.cpp/h # Keyboard input processing
│   ├── UIManager.cpp/h    # User interface management
│   ├── SoundManager.cpp/h # Audio effects and music
│   └── GameState.h        # Game state definitions
├── assets/                # Game assets
│   ├── words.txt         # Word list file
│   ├── fonts/            # Font files (optional)
│   ├── images/           # Image assets (optional)
│   └── sounds/           # Audio files (optional)
├── CMakeLists.txt        # Build configuration
└── README.md            # This file
```

## Architecture

The game follows object-oriented design principles with clear separation of concerns:

- **Game Class**: Main game loop and state management
- **WordManager Class**: Handles word loading, selection, and game logic  
- **Renderer Class**: All graphics rendering and visual effects
- **InputHandler Class**: Keyboard input processing and event handling
- **UIManager Class**: Menu systems and user interface
- **SoundManager Class**: Audio effects and background music

## Customization

### Adding New Words
Edit `assets/words.txt` and add one word per line. Words should be:
- 3-20 characters long
- Contain only letters (A-Z)
- One word per line

### Changing Difficulty
Modify the `Difficulty` enum in `src/GameState.h` to adjust lives per difficulty level.

### Custom Sounds
Add audio files to `assets/sounds/` directory:
- `correct.wav` - Correct guess sound
- `wrong.wav` - Wrong guess sound
- `victory.wav` - Victory sound
- `gameover.wav` - Game over sound
- `background.ogg` - Background music

## Dependencies

- **SFML 2.5+**: Graphics, window management, and audio
- **CMake 3.16+**: Build system
- **C++17**: Standard library features

## Development

To extend or modify the game:

1. **Adding New Features**: Follow the existing class structure
2. **Graphics Modifications**: Update the `Renderer` class
3. **Game Logic Changes**: Modify `Game` and `WordManager` classes  
4. **UI Improvements**: Enhance the `UIManager` class
5. **Sound Effects**: Extend the `SoundManager` class

## Troubleshooting

### Build Issues
- Ensure SFML development libraries are installed
- Verify CMake version compatibility
- Check C++17 compiler support

### Runtime Issues  
- Verify `assets/words.txt` exists and is readable
- Ensure proper graphics drivers (for SFML OpenGL context)
- Check audio system availability (ALSA/PulseAudio on Linux)

### Display Issues
- The game requires a graphical environment (X11, Wayland, etc.)
- For headless testing, use the provided test utilities

## License

This project is created for educational and entertainment purposes. Feel free to use, modify, and distribute according to your needs.