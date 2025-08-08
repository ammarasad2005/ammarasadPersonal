# Setup Guide for Space Shooter Game

This guide will help you set up the development environment to build and run the x86 MASM Space Shooter game.

## Prerequisites

### Required Software

1. **MASM32 SDK** - Microsoft Macro Assembler for 32-bit development
2. **Irvine32 Library** - Kip Irvine's assembly language support library
3. **Windows Environment** - Required for MASM32 and native execution

### Optional Tools

- **Text Editor** - Any text editor (Notepad++, VS Code, Visual Studio)
- **Command Prompt** - For running build commands
- **Make** (optional) - For using the provided Makefile

## Installation Steps

### Step 1: Install MASM32 SDK

1. Download MASM32 SDK from: http://www.masm32.com/
2. Run the installer as Administrator
3. Install to the default location: `C:\masm32\`
4. Add MASM32 to your system PATH:
   - Open System Properties → Advanced → Environment Variables
   - Add `C:\masm32\bin` to your PATH variable

### Step 2: Install Irvine32 Library

1. Download the Irvine32 library files:
   - `Irvine32.lib` (library file)
   - `Irvine32.inc` (include file)
   
2. Copy the files to MASM32 directories:
   - Copy `Irvine32.lib` to `C:\masm32\lib\`
   - Copy `Irvine32.inc` to `C:\masm32\include\`

### Step 3: Verify Installation

Open Command Prompt and test:
```batch
ml /?
```
You should see the MASM assembler help information.

## Building the Game

### Method 1: Using Batch File (Recommended)

Run the build script:
```batch
build.bat
```

Choose between:
- **Basic Version**: Standard space shooter with core features
- **Enhanced Version**: Advanced features including star field, borders, lives system

### Method 2: Using Makefile

Build both versions:
```bash
make all
```

Build specific version:
```bash
make basic      # Build basic version
make enhanced   # Build enhanced version
```

### Method 3: Manual Build

For basic version:
```batch
ml /c /coff SpaceShooter.asm
link /subsystem:console SpaceShooter.obj irvine32.lib kernel32.lib user32.lib
```

For enhanced version:
```batch
ml /c /coff SpaceShooterEnhanced.asm
link /subsystem:console SpaceShooterEnhanced.obj irvine32.lib kernel32.lib user32.lib
```

## Running the Game

After successful build, run:
```batch
SpaceShooter.exe          # Basic version
SpaceShooterEnhanced.exe  # Enhanced version
```

## Troubleshooting

### Common Issues

**Error: 'ml' is not recognized**
- Solution: Add MASM32\bin to your system PATH

**Error: Cannot open include file 'Irvine32.inc'**
- Solution: Copy Irvine32.inc to MASM32\include directory

**Error: Unresolved external symbol**
- Solution: Ensure Irvine32.lib is in MASM32\lib directory

**Game doesn't respond to input**
- Solution: Ensure console window has focus when playing

### Build Verification

To verify syntax without linking:
```batch
syntax_check.bat
```

### Clean Build

Remove build artifacts:
```batch
make clean
```
or manually delete:
```batch
del *.obj *.exe
```

## File Structure

```
ammarasadPersonal/
├── SpaceShooter.asm          # Basic game implementation
├── SpaceShooterEnhanced.asm  # Enhanced game with additional features
├── build.bat                 # Windows build script
├── Makefile                  # Cross-platform build script
├── syntax_check.bat          # Syntax verification script
├── .gitignore               # Git ignore patterns
├── README.md                # Project documentation
└── SETUP.md                 # This setup guide
```

## Game Controls

### Basic Version
- `A` / `D` - Move player left/right
- `SPACE` - Fire bullet
- `Q` - Quit game

### Enhanced Version
- `A` / `D` - Move player left/right
- `SPACE` - Fire bullet
- `P` - Pause/unpause game
- `Q` - Quit game

## Development Notes

### Assembly Programming Tips

1. **Debugging**: Use syntax_check.bat for quick syntax verification
2. **Comments**: Assembly code is heavily commented for learning
3. **Procedures**: Game logic is organized into modular procedures
4. **Data Structures**: Arrays simulate object-oriented concepts

### Code Organization

- **Game Loop**: Main game logic in continuous loop
- **Input Handling**: Non-blocking keyboard input
- **Collision Detection**: Efficient array-based collision checking
- **Memory Management**: Static arrays for predictable memory usage

### Performance Considerations

- **Frame Rate**: Controlled by Delay procedure calls
- **Buffer Management**: Single screen buffer for efficient rendering
- **Object Pooling**: Fixed-size arrays prevent memory allocation

## Learning Objectives

This project demonstrates:

1. **Assembly Language Fundamentals**
   - Registers and memory addressing
   - Loops and conditional jumps
   - Procedure calls and stack management

2. **Game Development Concepts**
   - Game loops and state management
   - Input handling and response
   - Collision detection algorithms
   - Real-time graphics rendering

3. **Irvine32 Library Usage**
   - Console I/O operations
   - Keyboard input handling
   - Screen manipulation
   - Random number generation

## Resources

- **MASM32**: http://www.masm32.com/
- **Irvine32 Documentation**: Search for "Kip Irvine Assembly Language"
- **Assembly Language Tutorial**: Various online resources
- **x86 Instruction Reference**: Intel/AMD processor manuals

---

For additional help or issues, refer to the main README.md or create an issue in the repository.