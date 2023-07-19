//
// Created by nisal on 7/19/2023.
//

#ifndef RPAL_FINAL_CSE_H
#define RPAL_FINAL_CSE_H


#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

#include "Tree.h"

// enum of node types for CSE machine
enum class ObjType : int {
    LAMBDA,
    IDENTIFIER,
    INTEGER,
    STRING,
    GAMMA,
    OPERATOR,
    BETA,
    EETA,
    DELTA,
    TAU,
    ENV,
    LIST,
    BOOLEAN
};

/**
 * Check if the given label is an operator.
 *
 * @param label The label to check.
 * @return True if the label is an operator, false otherwise.
 */
bool isOperator(const std::string &label);


#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"

// NOLINTNEXTLINE
class CseNode {
private:
    // General node properties
    ObjType node_type;
    std::string node_value;

    // CseNode properties for lambda and eeta nodes
    int env{};
    int cs_index{}; // for delta, tau, eeta, lambda nodes
    std::vector<std::string> bound_variables;
    std::vector<std::pair<ObjType, std::string>> list_elements;
    bool is_single_bound_var = true;

public:
    CseNode() = default; // NOLINT(cppcoreguidelines-pro-type-member-init)

    // Constructor for lambda (in stack) and eeta nodes
    CseNode(ObjType node_type, std::string node_value, int cs_index, int env);

    // Constructor for lambda (in control structure) nodes
    CseNode(ObjType node_type, std::string node_value, int cs_index);

    // Constructor for other nodes
    CseNode(ObjType node_type, std::string node_value);

    // Constructor for lambda (in cs) nodes with bound variables
    CseNode(ObjType node_type, int cs_index, std::vector<std::string> bound_variables);

    // Constructor for lambda (in stack) nodes with bound variables
    CseNode(ObjType node_type, int cs_index, std::vector<std::string> bound_variables, int env);

    CseNode(ObjType node_type, std::vector<std::pair<ObjType, std::string>> list_elements);

    // Getters
    [[nodiscard]] ObjType get_node_type() const;

    [[nodiscard]] std::string get_node_value() const;

    [[nodiscard]] int get_env() const;

    [[nodiscard]] int get_cs_index() const;

    [[nodiscard]] bool get_is_single_bound_var() const;

    [[nodiscard]] std::vector<std::string> get_var_list() const;

    std::vector<std::pair<ObjType, std::string>> get_list_elements();

    CseNode set_env(int env_);
};

#pragma clang diagnostic pop


class ControlStructure {
private:
    int cs_index;
    std::vector<CseNode> nodes;

public:
    // Constructor with empty nodes
    explicit ControlStructure(int cs_index);

    // add node to control structure
    void add_node(const CseNode &node);

    // Getters
    [[maybe_unused]] [[nodiscard]] int get_cs_index() const;

    // return the last node in the control structure
    [[maybe_unused]] [[nodiscard]] CseNode get_last_node() const;

    // pop the last node in the control structure
    void pop_last_node();

    // pop and return the last node in the control structure
    CseNode pop_and_return_last_node();

    // push another control structure to the current control structure
    void push_control_structure(const ControlStructure &cs);
};

class Stack {
private:
    std::vector<CseNode> nodes;

public:
    // constructor with empty nodes
    Stack() = default;

    // add node to stack
    void add_node(const CseNode &node);

    // pop the last node in the stack
    [[maybe_unused]] void pop_last_node();

    // pop and return the last node in the stack
    CseNode pop_and_return_last_node();

    // length of the stack
    [[maybe_unused]] [[nodiscard]] int length() const;
};

class Env {
private:
    std::unordered_map<std::string, std::pair<ObjType, std::string>> variables;
    std::unordered_map<std::string, CseNode *> lambdas;
    std::unordered_map<std::string, std::vector<std::pair<ObjType, std::string>>> lists;
    [[maybe_unused]] bool is_lambda = false;
    Env *parent_env;

public:
    // constructor with empty variables and lambdas
    Env();

    // constructor with empty variables and lambdas
    explicit Env(Env *parent_env);

    // add variable to environment
    void add_variable(const std::string &identifier, const std::pair<ObjType, std::string> &value);

    [[maybe_unused]] void add_variables(const std::vector<std::string> &identifiers,
                       const std::vector<std::pair<ObjType, std::string>> &values);

    void add_list(const std::string &identifier, const std::vector<std::pair<ObjType, std::string>> &list_elements);

    // add lambda to environment
    void add_lambda(const std::string &identifier, const CseNode &lambda);

    // get variable from environment
    std::pair<ObjType, std::string> get_variable(const std::string &identifier);

    // get lambda from environment
    CseNode get_lambda(const std::string &identifier);

    // get list from environment
    std::vector<std::pair<ObjType, std::string>> get_list(const std::string &identifier);
};

class CSE {
private:
    int next_env = 0;
    int next_cs = -1;

    std::vector<ControlStructure *> control_structures;
    ControlStructure main_control_structure = ControlStructure(-1);
    Stack stack = Stack();
    std::vector<int> env_stack = std::vector<int>();
    std::unordered_map<int, Env *> envs = std::unordered_map<int, Env *>();

public:
    CSE() = default;

    /**
     * Create control structures for the RPAL program represented by the given Abstract Syntax Tree (AST).
     *
     * @param root: The root node of the AST.
     * @param current_cs: The current control structure being constructed.
     * @param current_cs_index: The index of the current control structure in the control_structures vector.
     */
    void create_cs(TreeNode *root, ControlStructure *current_cs = nullptr, int current_cs_index = -1);

    /**
     * Evaluate the main control structure.
     * This function implements the RPAL evaluation algorithm for the main control structure.
     */
    void evaluate();
};

#endif //RPAL_FINAL_CSE_H
