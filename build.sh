#!/bin/bash

# Hangman Game Build Script
# This script builds the SFML Hangman game

set -e  # Exit on any error

echo "🎮 Building Hangman Game with SFML..."

# Check if SFML is installed
if ! pkg-config --exists sfml-all; then
    echo "❌ SFML not found. Please install SFML development libraries:"
    echo "   Ubuntu/Debian: sudo apt install libsfml-dev"
    echo "   CentOS/RHEL: sudo yum install SFML-devel"
    echo "   macOS: brew install sfml"
    exit 1
fi

echo "✓ SFML found: $(pkg-config --modversion sfml-all)"

# Create build directory
if [ ! -d "build" ]; then
    mkdir build
    echo "✓ Created build directory"
fi

# Navigate to build directory
cd build

# Configure with CMake
echo "🔧 Configuring with CMake..."
cmake ..

# Build the project
echo "🔨 Building project..."
make -j$(nproc 2>/dev/null || echo 4)

echo "🎉 Build completed successfully!"
echo ""
echo "To run the game:"
echo "   cd build && ./hangman"
echo ""
echo "To test word functionality:"
echo "   cd .. && g++ -std=c++17 -o test_wm test_wordmanager.cpp src/WordManager.cpp && ./test_wm"