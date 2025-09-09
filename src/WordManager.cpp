#include "WordManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <cctype>

WordManager::WordManager() {
    reset();
}

bool WordManager::loadWordsFromFile(const std::string& filename) {
    words.clear();
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open words file: " << filename << std::endl;
        return false;
    }
    
    std::string word;
    while (std::getline(file, word)) {
        // Remove whitespace and convert to uppercase
        word.erase(std::remove_if(word.begin(), word.end(), ::isspace), word.end());
        word = toUpperCase(word);
        
        // Only add valid words
        if (isValidWord(word)) {
            words.push_back(word);
        }
    }
    
    file.close();
    
    if (words.empty()) {
        std::cerr << "Warning: No valid words loaded from file" << std::endl;
        return false;
    }
    
    std::cout << "Loaded " << words.size() << " words from " << filename << std::endl;
    return true;
}

void WordManager::selectRandomWord() {
    if (words.empty()) {
        currentWord = "PROGRAMMING"; // Fallback word
        return;
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, words.size() - 1);
    
    currentWord = words[dis(gen)];
}

std::string WordManager::getDisplayWord() const {
    std::string display;
    
    for (char c : currentWord) {
        if (correctLetters.find(c) != correctLetters.end()) {
            display += c;
        } else {
            display += '_';
        }
        display += ' ';
    }
    
    // Remove trailing space
    if (!display.empty()) {
        display.pop_back();
    }
    
    return display;
}

bool WordManager::guessLetter(char letter) {
    letter = toUpperCase(letter);
    
    // Check if already guessed
    if (hasGuessedLetter(letter)) {
        return false;
    }
    
    // Add to guessed letters
    guessedLetters.insert(letter);
    
    // Check if letter is in the word
    bool found = false;
    for (char c : currentWord) {
        if (c == letter) {
            found = true;
            break;
        }
    }
    
    if (found) {
        correctLetters.insert(letter);
    } else {
        wrongLetters.insert(letter);
    }
    
    return found;
}

bool WordManager::isWordComplete() const {
    for (char c : currentWord) {
        if (correctLetters.find(c) == correctLetters.end()) {
            return false;
        }
    }
    return true;
}

bool WordManager::hasGuessedLetter(char letter) const {
    letter = toUpperCase(letter);
    return guessedLetters.find(letter) != guessedLetters.end();
}

void WordManager::reset() {
    guessedLetters.clear();
    correctLetters.clear();
    wrongLetters.clear();
    currentWord.clear();
}

char WordManager::toUpperCase(char c) const {
    return std::toupper(static_cast<unsigned char>(c));
}

std::string WordManager::toUpperCase(const std::string& str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

bool WordManager::isValidWord(const std::string& word) const {
    if (word.empty() || word.length() < 3 || word.length() > 20) {
        return false;
    }
    
    // Check that word contains only letters
    for (char c : word) {
        if (!std::isalpha(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    
    return true;
}