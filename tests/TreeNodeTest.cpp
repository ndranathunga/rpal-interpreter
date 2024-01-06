#include <gtest/gtest.h>
#include "TreeNode.h"

class TreeNodeTest : public ::testing::Test {
protected:
    TreeNode* node;

    void SetUp() override {
        // Set up code called before each test
        // Creating a TreeNode with a label for testing
        node = new TreeNode("root");
    }

    void TearDown() override {
        // Clean up code called after each test
        // Release the memory allocated for the node
        TreeNode::releaseNodeMemory(node);
    }
};

// Test for constructing a TreeNode and getting its label
TEST_F(TreeNodeTest, NodeInitialization) {
    ASSERT_EQ("root", node->getLabel());
}

// Test for adding and getting children
TEST_F(TreeNodeTest, AddAndGetChildren) {
    TreeNode* child1 = new TreeNode("child1");
    TreeNode* child2 = new TreeNode("child2");
    
    node->addChild(child1);
    node->addChild(child2);

    ASSERT_EQ(2, node->getNumChildren());
    ASSERT_EQ("child1", node->getChildren()[0]->getLabel());
    ASSERT_EQ("child2", node->getChildren()[1]->getLabel());
}

// Test for reversing children
TEST_F(TreeNodeTest, ReverseChildren) {
    TreeNode* child1 = new TreeNode("child1");
    TreeNode* child2 = new TreeNode("child2");
    
    node->addChild(child1);
    node->addChild(child2);
    node->reverseChildren();

    ASSERT_EQ("child2", node->getChildren()[0]->getLabel());
    ASSERT_EQ("child1", node->getChildren()[1]->getLabel());
}

// Test for removing a child
TEST_F(TreeNodeTest, RemoveChild) {
    TreeNode* child1 = new TreeNode("child1");
    TreeNode* child2 = new TreeNode("child2");
    
    node->addChild(child1);
    node->addChild(child2);
    node->removeChild(0, true);  // Remove the first child

    ASSERT_EQ(1, node->getNumChildren());
    ASSERT_EQ("child2", node->getChildren()[0]->getLabel());
}

// Test for LeafNode-specific behavior
TEST_F(TreeNodeTest, LeafNodeBehavior) {
    LeafNode* leaf = new LeafNode("leaf", "value");

    ASSERT_EQ("leaf", leaf->getLabel());
    ASSERT_EQ("value", leaf->getValue());

    // Ensure addChild is not callable on LeafNode
    // Can't be tested at compile time since it's a compile error, so it's commented out
    // leaf->addChild(new TreeNode("shouldFail"));

    TreeNode::releaseNodeMemory(leaf);
}

// Test for memory release
TEST_F(TreeNodeTest, MemoryRelease) {
    TreeNode* child1 = new TreeNode("child1");
    node->addChild(child1);

    // No need to assert here, just ensure no memory leaks are detected
}

// More tests can be added for InternalNode if it has unique behaviors

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
