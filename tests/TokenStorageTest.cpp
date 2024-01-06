#include <gtest/gtest.h>
#include "TokenStorage.h"
#include "Lexer.h"

class MockLexer : public Lexer {
public:
    MockLexer(std::string input) : Lexer(std::move(input)) {}

    Token getNextToken() override {
        if (getInput().empty()) {
            return {token_type::END_OF_FILE, ""};
        }

        // Simulate getting the next token from the input
        char ch = getInput()[0];
        // You need to cast away constness here because you're changing the string
        // This is generally not good practice and is only suitable here because we know the context
        std::string& modifiableInput = const_cast<std::string&>(getInput());
        modifiableInput.erase(0, 1);
        return {token_type::IDENTIFIER, std::string(1, ch)}; // Simplified for demonstration
    }
};


class TokenStorageTest : public ::testing::Test {
protected:
    MockLexer* lexer;
    TokenStorage* storage;

    void SetUp() override {
        lexer = new MockLexer("abcd"); // Mock input
        storage = &TokenStorage::getInstance();
        storage->setLexer(*lexer);
    }

    void TearDown() override {
        TokenStorage::destroyInstance();  // Cleanup the singleton instance
        delete lexer;
    }
};

// Test for Singleton instance
TEST_F(TokenStorageTest, SingletonInstance) {
    TokenStorage& ref1 = TokenStorage::getInstance();
    TokenStorage& ref2 = TokenStorage::getInstance();
    ASSERT_EQ(&ref1, &ref2);  // Both references should point to the same instance
}

// Test pushing and popping tokens
TEST_F(TokenStorageTest, PushPopToken) {
    ASSERT_EQ("a", storage->top().value); // The top should be the first token
    storage->pop();  // Remove the top
    ASSERT_EQ("b", storage->top().value); // Now the top should be the second token
}

// Test for handling end of file token
TEST_F(TokenStorageTest, HandlesEndOfFileToken) {
    // Simulate reading all tokens until EOF
    while (storage->top().type != token_type::END_OF_FILE) {
        storage->pop();
    }
    ASSERT_EQ(token_type::END_OF_FILE, storage->top().type);  // Should reach EOF token
}

// Test resetting the position
TEST_F(TokenStorageTest, ResetPosition) {
    storage->pop();  // Move the position forward
    storage->reset();  // Reset the position
    ASSERT_EQ("a", storage->top().value);  // Should be back at the first token
}

// Test destroying the instance
TEST_F(TokenStorageTest, DestroyInstance) {
    storage->pop();  // Modify the instance state
    TokenStorage::destroyInstance();
    storage = &TokenStorage::getInstance();  // Get the instance again
    ASSERT_EQ("a", storage->top().value);  // The state should be reset
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
