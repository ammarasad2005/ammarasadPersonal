#pragma once
#include <string>
#include <vector>
#include <set>

class WordManager {
public:
    WordManager();
    ~WordManager() = default;
    
    // File operations
    bool loadWordsFromFile(const std::string& filename);
    bool hasWords() const { return !words.empty(); }
    
    // Word selection
    void selectRandomWord();
    const std::string& getCurrentWord() const { return currentWord; }
    std::string getDisplayWord() const;
    
    // Game logic
    bool guessLetter(char letter);
    bool isWordComplete() const;
    bool hasGuessedLetter(char letter) const;
    
    // Getters
    const std::set<char>& getGuessedLetters() const { return guessedLetters; }
    const std::set<char>& getCorrectLetters() const { return correctLetters; }
    const std::set<char>& getWrongLetters() const { return wrongLetters; }
    int getWordLength() const { return currentWord.length(); }
    
    // Reset for new game
    void reset();
    
    // Statistics
    size_t getTotalWordsLoaded() const { return words.size(); }

private:
    std::vector<std::string> words;
    std::string currentWord;
    
    std::set<char> guessedLetters;
    std::set<char> correctLetters;
    std::set<char> wrongLetters;
    
    // Helper methods
    char toUpperCase(char c) const;
    std::string toUpperCase(const std::string& str) const;
    bool isValidWord(const std::string& word) const;
};