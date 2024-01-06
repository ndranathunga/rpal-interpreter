#include <gtest/gtest.h>
#include "Tree.h"

class TreeTest : public ::testing::Test {
protected:
    Tree* tree;
    TreeNode* astRoot;
    TreeNode* stRoot;

    void SetUp() override {
        // Setup code called before each test
        tree = &Tree::getInstance();
        astRoot = new TreeNode("ASTRoot");
        stRoot = new TreeNode("STRoot");
    }

    void TearDown() override {
        // Cleanup code called after each test
        // Release the memory allocated for the AST and ST, if not already done by the Tree class
        TreeNode::releaseNodeMemory(astRoot);
        TreeNode::releaseNodeMemory(stRoot);
    }
};

// Test for Singleton instance
TEST_F(TreeTest, SingletonInstance) {
    Tree& ref1 = Tree::getInstance();
    Tree& ref2 = Tree::getInstance();
    ASSERT_EQ(&ref1, &ref2);  // Both references should point to the same instance
}

// Test setting and getting AST root
TEST_F(TreeTest, SetAndGetASTRoot) {
    tree->setASTRoot(astRoot);
    ASSERT_EQ(astRoot, tree->getASTRoot());
}

// Test setting and getting ST root
TEST_F(TreeTest, SetAndGetSTRoot) {
    tree->setSTRoot(stRoot);
    ASSERT_EQ(stRoot, tree->getSTRoot());
}

// Test memory release for AST
TEST_F(TreeTest, ReleaseASTMemory) {
    tree->setASTRoot(astRoot);
    Tree::releaseASTMemory();
    ASSERT_EQ(nullptr, tree->getASTRoot());
    // Ensure astRoot is not dangling
    astRoot = nullptr;
}

// Test memory release for ST
TEST_F(TreeTest, ReleaseSTMemory) {
    tree->setSTRoot(stRoot);
    Tree::releaseSTMemory();
    ASSERT_EQ(nullptr, tree->getSTRoot());
    // Ensure stRoot is not dangling
    stRoot = nullptr;
}

// Test generate ST from AST
TEST_F(TreeTest, GenerateST) {
    // Assuming generateST function is implemented correctly
    // This is a placeholder for when you have a real AST to convert to ST
    tree->setASTRoot(astRoot);
    Tree::generate();
    // Verify that ST has been generated as expected
    ASSERT_NE(nullptr, tree->getSTRoot());
    // Ensure astRoot is not dangling
    astRoot = nullptr;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
