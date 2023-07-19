//
// Created by nisal on 7/19/2023.
//

#include "TreeNode.h"

#include <utility>

TreeNode::TreeNode(std::string l) : label(std::move(l)) {
    children = std::vector<TreeNode *>();
}

void TreeNode::addChild(TreeNode *child) {
    children.push_back(child);
}

void TreeNode::reverseChildren() {
    std::reverse(children.begin(), children.end());
}

void TreeNode::removeChild(int index, bool deleteNode) {
    if (index < 0 || index >= children.size())
    {
        throw std::out_of_range("Index out of range");
    }

    if (deleteNode)
    {
        releaseNodeMemory(children[index]);
    }

    children.erase(children.begin() + index);
}

int TreeNode::getNumChildren() {
    return static_cast<int>(children.size());
}

std::string TreeNode::getLabel() {
    return label;
}

std::vector<TreeNode *> &TreeNode::getChildren() {
    return children;
}

std::string TreeNode::getValue() {
    return value;
}

void TreeNode::setValue(std::string v) {
    value = std::move(v);
}

// NOLINTNEXTLINE
void TreeNode::releaseNodeMemory(TreeNode *node) {
    if (node == nullptr)
        return;

    for (TreeNode *child : node->getChildren())
    {
        releaseNodeMemory(child);
    }

    delete node;
}
