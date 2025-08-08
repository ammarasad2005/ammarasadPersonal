# ammarasadPersonal

## Space Shooter Game - x86 MASM Assembly

A classic space shooter game implemented in x86 MASM assembly language using the Irvine32 library.

### Game Features

- **Player Control**: Move left/right with A/D keys, shoot with SPACE
- **Enemy Ships**: Randomly spawning enemies that move down the screen
- **Collision Detection**: Bullets destroy enemies on contact
- **Scoring System**: Earn points for destroying enemies
- **Console Graphics**: Text-based graphics using console characters
- **Real-time Gameplay**: Smooth game loop with input handling

### Controls

- `A` - Move player left
- `D` - Move player right  
- `SPACE` - Fire bullet
- `Q` - Quit game

### Game Mechanics

- Player ship (`*`) starts at the bottom center of the screen
- Enemy ships (`V`) spawn randomly at the top and move downward
- Player bullets (`|`) move upward and destroy enemies on contact
- Score increases by 10 points for each enemy destroyed
- Game continues until player quits

### Prerequisites

To build and run this game, you need:

1. **MASM32 SDK** - Microsoft Macro Assembler for 32-bit
2. **Irvine32 Library** - Kip Irvine's assembly language library
3. **Windows Environment** - Required for MASM32 and Irvine32

### Installation

1. Install MASM32 SDK from: http://www.masm32.com/
2. Download and install Irvine32 library
3. Ensure both are properly configured in your system PATH

### Building the Game

#### Using Batch File (Windows)
```batch
build.bat
```

#### Using Makefile
```bash
make
```

#### Manual Build
```batch
ml /c /coff SpaceShooter.asm
link /subsystem:console SpaceShooter.obj irvine32.lib kernel32.lib user32.lib
```

### Running the Game

After successful build, run:
```batch
SpaceShooter.exe
```

### Code Structure

The game is implemented as a single assembly file (`SpaceShooter.asm`) with the following key procedures:

- `InitializeGame` - Sets up initial game state
- `HandleInput` - Processes keyboard input
- `UpdatePlayer` - Updates player state
- `UpdateEnemies` - Updates enemy positions and spawning
- `UpdateBullets` - Updates bullet positions
- `CheckCollisions` - Detects bullet-enemy collisions
- `RenderGame` - Draws all game objects to screen buffer
- `DisplayScreen` - Outputs the game screen to console

### Technical Details

- **Screen Size**: 80x25 characters (standard console size)
- **Max Enemies**: 10 simultaneous enemies
- **Max Bullets**: 20 simultaneous bullets
- **Rendering**: Character-based graphics using console buffer
- **Input**: Non-blocking keyboard input using Irvine32 functions
- **Timing**: Frame-based game loop with small delay

### Assembly Language Features Used

- Arrays and data structures for game objects
- Loops and conditional jumps for game logic
- Procedure calls and stack management
- Memory addressing and pointer arithmetic
- Irvine32 library integration for I/O operations

### Educational Value

This project demonstrates:
- Assembly language programming concepts
- Game development fundamentals
- Console-based graphics programming
- Real-time input handling
- Memory management in assembly
- Structured programming in low-level language

### Future Enhancements

Potential improvements could include:
- Multiple enemy types
- Power-ups and special weapons
- Lives/health system
- Sound effects
- Improved graphics with extended ASCII
- Level progression
- High score saving

---

*This project showcases x86 assembly programming skills using the Irvine32 library for educational purposes.*