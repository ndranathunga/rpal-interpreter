//
// Created by nisal on 7/19/2023.
//

#ifndef RPAL_FINAL_LEXER_H
#define RPAL_FINAL_LEXER_H


#include <string>
#include <sstream>
#include <iostream>
//#include <unordered_set>

#include "Token.h"

#include <unordered_set>
#include <utility>
//#include <sstream>
//#include <iostream>


/**
 * @brief The Lexer class tokenizes the input string.
 */
class Lexer
{
public:
    /**
     * @brief Constructs a Lexer object with the given input string.
     * @param input The input string to tokenize.
     */
    [[maybe_unused]] explicit Lexer(std::string input) : input(std::move(input)), currentPosition(0) {}

    /**
     * @brief Retrieves the next token from the input string.
     * @return The next token.
     */
    Token getNextToken();

private:
    /**
     * @brief Skips whitespace characters in the input string.
     */
    void skipWhitespace();

    /**
     * @brief Checks if the given character is an operator symbol.
     * @param c The character to check.
     * @return True if the character is an operator symbol, false otherwise.
     */
    static bool isOperatorSymbol(char c);

private:
    std::string input;
    size_t currentPosition;
};

#endif //RPAL_FINAL_LEXER_H
