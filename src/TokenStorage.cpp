//
// Created by nisal on 7/19/2023.
//

#include "TokenStorage.h"

void TokenStorage::setTokens() {
    Token token;
    do
    {
        token = lexer->getNextToken();
        tokens.push_back(token);
    } while (token.type != token_type::END_OF_FILE);
}

TokenStorage &TokenStorage::getInstance() {
    return instance;
}

void TokenStorage::setLexer(Lexer &lexer_) {
    this->lexer = &lexer_;
    setTokens();
    currentPosition = 0;
}

Token &TokenStorage::top() {
    return tokens[currentPosition];
}

Token &TokenStorage::pop() {
    return tokens[currentPosition++];
}

[[maybe_unused]] void TokenStorage::reset() {
    currentPosition = 0;
}

void TokenStorage::destroyInstance() {
    instance.lexer = nullptr;
    instance.tokens.clear();
}


TokenStorage TokenStorage::instance; // Initialize the static instance
