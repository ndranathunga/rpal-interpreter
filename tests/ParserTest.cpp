#include <gtest/gtest.h>
#include "Parser.h"

class MockLexer : public Lexer
{
public:
    explicit MockLexer(const std::vector<Token> &tokens)
        : Lexer(""),
          tokens_(tokens),
          index_(0) {}

    Token getNextToken() override
    {
        if (index_ < tokens_.size())
        {
            return tokens_[index_++];
        }
        return {token_type::END_OF_FILE, ""};
    }

private:
    std::vector<Token> tokens_;
    size_t index_;
};

// Helper function to set up the token storage with the mock lexer
void SetupTokenStorageWithMockLexer(const std::vector<Token> &tokens)
{
    static MockLexer mockLexer(tokens);
    TokenStorage::getInstance().setLexer(mockLexer);
}

class ParserTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        Parser::nodeStack.clear(); // Clear the node stack before each test
    }

    void TearDown() override
    {
        // Clean up the node stack after each test to prevent memory leaks
        for (TreeNode *node : Parser::nodeStack)
        {
            TreeNode::releaseNodeMemory(node);
        }
        Parser::nodeStack.clear();
    }
};

TEST_F(ParserTest, ParseIdentifier)
{
    std::vector<Token> tokens = {
        {token_type::IDENTIFIER, "x"},
        {token_type::END_OF_FILE, ""}};
    SetupTokenStorageWithMockLexer(tokens);
    ASSERT_NO_THROW(Parser::parse());
    ASSERT_EQ(1, Parser::nodeStack.size());
    ASSERT_EQ("identifier", Parser::nodeStack.back()->getLabel());
    ASSERT_EQ("x", Parser::nodeStack.back()->getValue());
}

// More tests for other grammar rules...

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
