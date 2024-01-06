#include <gtest/gtest.h>
#include "Lexer.h"

class LexerTest : public ::testing::Test {
protected:
    Lexer* lexer;

    void SetUp() override {
        // Setup code common to all tests
    }

    void TearDown() override {
        delete lexer;  // Clean up after each test
    }
};

// Test for recognizing keywords
TEST_F(LexerTest, RecognizesKeywords) {
    lexer = new Lexer("let");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::KEYWORD, token.type);
    ASSERT_EQ("let", token.value);
}

// Test for recognizing identifiers
TEST_F(LexerTest, RecognizesIdentifiers) {
    lexer = new Lexer("variable1");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::IDENTIFIER, token.type);
    ASSERT_EQ("variable1", token.value);
}

// Test for recognizing integers
TEST_F(LexerTest, RecognizesIntegers) {
    lexer = new Lexer("12345");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::INTEGER, token.type);
    ASSERT_EQ("12345", token.value);
}

// Test for recognizing operators
TEST_F(LexerTest, RecognizesOperators) {
    lexer = new Lexer("+");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::OPERATOR, token.type);
    ASSERT_EQ("+", token.value);
}

// Test for recognizing strings
TEST_F(LexerTest, RecognizesStrings) {
    lexer = new Lexer("\"hello world\"");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::STRING, token.type);
    ASSERT_EQ("hello world", token.value);  // Assuming the Lexer unescapes the string
}

// Test for handling whitespace
TEST_F(LexerTest, HandlesWhitespace) {
    lexer = new Lexer("    abc");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::IDENTIFIER, token.type);
    ASSERT_EQ("abc", token.value);
}

// Test for handling end of file
TEST_F(LexerTest, HandlesEOF) {
    lexer = new Lexer("");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::END_OF_FILE, token.type);
}

// Test for handling unknown tokens
TEST_F(LexerTest, HandlesUnknown) {
    lexer = new Lexer("?");
    Token token = lexer->getNextToken();
    ASSERT_EQ(token_type::END_OF_FILE, token.type); // Adjust according to actual behavior for unknown tokens
}

// Main function running all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
