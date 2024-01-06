//
// Created by nisal on 7/19/2023.
//

#ifndef RPAL_FINAL_PARSER_H
#define RPAL_FINAL_PARSER_H


#include "Token.h"
#include "Lexer.h"
#include "TokenStorage.h"
#include "Tree.h"
#include "TreeNode.h"

//#include <vector>


void E();
void Ew();
void T();
void Ta();
void Tc();
void B();
void Bt();
void Bs();
void Bp();
void A();
void At();
void Af();
void Ap();
void R();
void Rn();
void D();
void Da();
void Dr();
void Db();
void Vb();
void Vl();


/**
 * Constructs a new TreeNode with the specified label, number of children, leaf status, and value.
 * Adds the constructed node to the nodeStack.
 * @param label The label of the node.
 * @param num The number of children the node will have.
 * @param isLeaf A boolean indicating whether the node is a leaf node or not.
 * @param value The value associated with the node (only applicable for leaf nodes).
 */
void build_tree(const std::string &label, const int &num, bool isLeaf, const std::string &value = "");


/**
 * The Parser class is responsible for parsing a sequence of tokens and constructing the Abstract Syntax Tree (AST).
 */
class Parser
{
public:
    static std::vector<TreeNode *> nodeStack;

    /**
     * Parses the input tokens and constructs the Abstract Syntax Tree (AST).
     */
    static void parse();
};

#endif //RPAL_FINAL_PARSER_H
