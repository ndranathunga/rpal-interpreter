//
// Created by nisal on 7/19/2023.
//

#ifndef RPAL_FINAL_TOKENSTORAGE_H
#define RPAL_FINAL_TOKENSTORAGE_H


#include "Lexer.h"
#include <vector>

/**
 * The TokenStorage class is responsible for storing and managing tokens during parsing.
 * It implements the singleton pattern to ensure only one instance exists throughout the program.
 */
class TokenStorage
{
private:
    static TokenStorage instance; // Singleton instance
    std::vector<Token> tokens;    // Vector to store tokens
    int currentPosition{};          // Current position in the tokens vector
    Lexer *lexer{};                 // Pointer to the lexer

    // Private constructor to prevent instantiation
    TokenStorage() {}

    // Private destructor to prevent deletion of the instance
    ~TokenStorage() = default;

    /**
     * Retrieves tokens from the lexer and stores them in the tokens vector until the end of file token is encountered.
     */
    void setTokens();

public:
    // Private copy constructor and assignment operator to prevent copying
    TokenStorage(const TokenStorage &) = delete;

    TokenStorage &operator=(const TokenStorage &) = delete;

    /**
     * Returns the instance of the TokenStorage class.
     * @return The singleton instance of the TokenStorage class.
     */
    static TokenStorage &getInstance();

    /**
     * Sets the lexer_ and initializes the tokens vector.
     * @param lexer_ The lexer_ object to set.
     */
    void setLexer(Lexer &lexer_);

    /**
     * Returns a reference to the current token at the top of the tokens vector.
     * @return A reference to the current token.
     */
    Token &top();

    /**
     * Removes and returns the current token at the top of the tokens vector.
     * @return The current token.
     */
    Token &pop();

    /**
     * Resets the current position to the beginning of the tokens vector.
     */
    [[maybe_unused]] void reset();

    /**
     * Clears the tokens vector and sets the lexer to nullptr.
     * This method should be called when the TokenStorage instance is no longer needed.
     */
    static void destroyInstance();
};


#endif //RPAL_FINAL_TOKENSTORAGE_H
