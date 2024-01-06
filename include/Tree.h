//
// Created by nisal on 7/19/2023.
//

#ifndef RPAL_FINAL_TREE_H
#define RPAL_FINAL_TREE_H


#include "TreeNode.h"


/**
 * Generates the Syntax Tree (ST) by modifying the given tree structure.
 *
 * @param currentNode The current node being processed.
 * @param parentNode The parent node of the current node.
 */
void generateST(TreeNode *currentNode, TreeNode *parentNode);


/**
 * @brief Represents the Tree for a program.
 *
 * The Tree class stores the root nodes of the AST and ST and provides
 * access to them. It follows the Singleton design pattern to ensure
 * that only one instance of the Tree exists.
 */
class Tree
{
private:
    static Tree *tree;           // The singleton instance of the Tree
    TreeNode *astRoot = nullptr; // The root node of the Abstract Syntax Tree (AST)
    TreeNode *stRoot = nullptr;  // The root node of the Standardized Tree (ST)

    Tree() {} // Private constructor

    ~Tree() = default; // Private destructor

public:
    Tree &operator=(const Tree &) = delete; // Disable assignment operator

    Tree(const Tree &) = delete; // Disable copy constructor

    /**
     * @brief Returns the singleton instance of the Tree.
     * @return The reference to the Tree instance.
     */
    static Tree &getInstance();

    /**
     * @brief Sets the root node of the Abstract Syntax Tree (AST).
     * @param r The root node to set.
     */
    void setASTRoot(TreeNode *r);

    /**
     * @brief Retrieves the root node of the Abstract Syntax Tree (AST).
     * @return The root node of the AST.
     */
    TreeNode *getASTRoot();

    /**
     * @brief Sets the root node of the Standardized Tree (ST).
     * @param r The root node to set.
     */
    void setSTRoot(TreeNode *r);

    /**
     * @brief Retrieves the root node of the Standardized Tree (ST).
     * @return The root node of the ST.
     */
    TreeNode *getSTRoot();

    /**
     * @brief Releases the memory occupied by the Abstract Syntax Tree (AST).
     *
     * This function calls the static releaseNodeMemory() function
     * of the TreeNode class to release the memory of all AST nodes.
     * It should be called when the AST is no longer needed to avoid memory leaks.
     */
    static void releaseASTMemory();

    /**
     * @brief Releases the memory occupied by the Standardized Tree (ST).
     *
     * This function calls the static releaseNodeMemory() function
     * of the TreeNode class to release the memory of all ST nodes.
     * It should be called when the ST is no longer needed to avoid memory leaks.
     */
    [[maybe_unused]] static void releaseSTMemory();

    /**
     * @brief Generates the Standardized Tree (ST) from the Abstract Syntax Tree (AST).
     *
     * This function calls the generateST() function to generate the ST from the AST.
     * It should be called when the AST is no longer needed to avoid memory leaks.
     */
    static void generate();
};

#endif //RPAL_FINAL_TREE_H
