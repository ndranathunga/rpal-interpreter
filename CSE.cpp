//
// Created by nisal on 7/19/2023.
//

#include "CSE.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"


std::vector<std::string> built_in_functions = {"Print", "Order", "Y*", "Conc", "Stem", "Stern", "Isinteger", "Isstring",
                                               "Istuple", "Isempty"};


/*
 * CseNode class
 */
CseNode::CseNode(ObjType node_type, std::string node_value, int cs_index, int env) {
    this->node_type = node_type;
    this->node_value = std::move(node_value);
    this->cs_index = cs_index;
    this->env = env;
}

#pragma clang diagnostic pop

CseNode::CseNode(ObjType node_type, std::string node_value, int cs_index) {
    this->node_type = node_type;
    this->node_value = std::move(node_value);
    this->cs_index = cs_index;
}

CseNode::CseNode(ObjType node_type, std::string node_value) {
    this->node_type = node_type;
    this->node_value = std::move(node_value);
}

CseNode::CseNode(ObjType node_type, int cs_index, std::vector<std::string> bound_variables) {
    is_single_bound_var = false;
    this->node_type = node_type;
    this->cs_index = cs_index;
    this->bound_variables = std::move(bound_variables);
}

CseNode::CseNode(ObjType node_type, int cs_index, std::vector<std::string> bound_variables, int env) {
    is_single_bound_var = false;
    this->node_type = node_type;
    this->node_value = std::move(node_value);
    this->cs_index = cs_index;
    this->env = env;
    this->bound_variables = std::move(bound_variables);
}

CseNode::CseNode(ObjType node_type, std::vector<std::pair<ObjType, std::string>> list_elements) {
    this->node_type = node_type;
    this->list_elements = std::move(list_elements);
}

ObjType CseNode::get_node_type() const {
    return node_type;
}

std::string CseNode::get_node_value() const {
    return node_value;
}

int CseNode::get_env() const {
    return env;
}

int CseNode::get_cs_index() const {
    return cs_index;
}

bool CseNode::get_is_single_bound_var() const {
    return is_single_bound_var;
}

std::vector<std::string> CseNode::get_var_list() const {
    return bound_variables;
}

std::vector<std::pair<ObjType, std::string>> CseNode::get_list_elements() {
    return list_elements;
}

CseNode CseNode::set_env(int env_) {
    this->env = env_;
    return *this;
}

/*
 * ControlStructure class
 */
ControlStructure::ControlStructure(int cs_index) {
    this->cs_index = cs_index;
}

void ControlStructure::add_node(const CseNode &node) {
    nodes.push_back(node);
}

[[maybe_unused]] int ControlStructure::get_cs_index() const {
    return cs_index;
}

[[maybe_unused]] CseNode ControlStructure::get_last_node() const {
    return nodes.back();
}

void ControlStructure::pop_last_node() {
    nodes.pop_back();
}

CseNode ControlStructure::pop_and_return_last_node() {
    CseNode node = nodes.back();
    nodes.pop_back();

    return node;
}

void ControlStructure::push_control_structure(const ControlStructure &cs) {
    for (auto &node: cs.nodes) {
        nodes.push_back(node);
    }
}

void Stack::add_node(const CseNode &node) {
    nodes.push_back(node);
}

[[maybe_unused]] void Stack::pop_last_node() {
    nodes.pop_back();
}

CseNode Stack::pop_and_return_last_node() {
    CseNode node = nodes.back();
    nodes.pop_back();

    return node;
}

[[maybe_unused]] int Stack::length() const {
    return static_cast<int>(nodes.size());
}

/*
 * Env class
 */
Env::Env() {
    parent_env = nullptr;
}

Env::Env(Env *parent_env) {
    this->parent_env = parent_env;
}

void Env::add_variable(const std::string &identifier, const std::pair<ObjType, std::string> &value) {
    variables[identifier] = value;
}

[[maybe_unused]] void Env::add_variables(const std::vector<std::string> &identifiers,
                        const std::vector<std::pair<ObjType, std::string>> &values) {
    for (int i = 0; i < identifiers.size(); i++) {
        variables[identifiers[i]] = values[i];
    }
}

void Env::add_list(const std::string &identifier, const std::vector<std::pair<ObjType, std::string>> &list_elements) {
    lists[identifier] = list_elements;
}

void Env::add_lambda(const std::string &identifier, const CseNode &lambda) {
    is_lambda = true;

    // check the node type and create new object
    if (lambda.get_node_type() == ObjType::LAMBDA) {
        if (lambda.get_is_single_bound_var()) {
            lambdas[identifier] = new CseNode(ObjType::LAMBDA,
                                              lambda.get_node_value(), lambda.get_cs_index(), lambda.get_env());
        } else {
            lambdas[identifier] = new CseNode(ObjType::LAMBDA, lambda.get_cs_index(),
                                              lambda.get_var_list(), lambda.get_env());
        }
    } else if (lambda.get_node_type() == ObjType::EETA) {
        if (lambda.get_is_single_bound_var()) {
            lambdas[identifier] = new CseNode(ObjType::EETA,
                                              lambda.get_node_value(), lambda.get_cs_index(), lambda.get_env());
        } else {
            lambdas[identifier] = new CseNode(ObjType::EETA, lambda.get_cs_index(),
                                              lambda.get_var_list(), lambda.get_env());
        }
    } else {
        throw std::runtime_error("Invalid lambda node type");
    }
}

// NOLINTNEXTLINE
std::pair<ObjType, std::string> Env::get_variable(const std::string &identifier) {
    if (variables.find(identifier) != variables.end()) {
        return variables[identifier];
    } else if (parent_env != nullptr) {
        return parent_env->get_variable(identifier);
    } else {
        throw std::runtime_error("Identifier: " + identifier + " not found");
    }
}

// NOLINTNEXTLINE
CseNode Env::get_lambda(const std::string &identifier) {
    if (lambdas.find(identifier) != lambdas.end()) {
        return *lambdas[identifier];
    } else if (parent_env != nullptr) {
        return parent_env->get_lambda(identifier);
    } else {
        throw std::runtime_error("Identifier: " + identifier + " not found");
    }
}

// NOLINTNEXTLINE
std::vector<std::pair<ObjType, std::string>> Env::get_list(const std::string &identifier) {
    if (lists.find(identifier) != lists.end()) {
        return lists[identifier];
    } else if (parent_env != nullptr) {
        return parent_env->get_list(identifier);
    } else {
        throw std::runtime_error("Identifier: " + identifier + " not found");
    }
}

// NOLINTNEXTLINE
void CSE::create_cs(TreeNode *root, ControlStructure *current_cs, int current_cs_index) {
    ControlStructure *cs;

    if (next_cs == -1) {
        next_cs++;
        cs = new ControlStructure(next_cs++);
        control_structures.push_back(cs);
        current_cs_index = 0;
    } else {
        cs = current_cs;
    }

    if (root->getLabel() == "lambda") {
        CseNode *lambda;
        if (root->getChildren()[0]->getLabel() == ",") {
            std::vector<std::string> vars;
            for (auto &child: root->getChildren()[0]->getChildren()) {
                vars.push_back(child->getValue());
            }
            lambda = new CseNode(ObjType::LAMBDA, next_cs, vars);
        } else {
            std::string var = root->getChildren()[0]->getValue();
            lambda = new CseNode(ObjType::LAMBDA, var, next_cs);
        }

        cs->add_node(*lambda);

        auto *new_cs = new ControlStructure(next_cs);
        control_structures.push_back(new_cs);
        create_cs(root->getChildren()[1], new_cs, next_cs++);
    } else if (root->getLabel() == "tau") {
        auto *tau = new CseNode(ObjType::TAU, std::to_string(root->getChildren().size()));
        cs->add_node(*tau);

        for (auto &child: root->getChildren()) {
            create_cs(child, cs, current_cs_index);
        }
    } else if (root->getLabel() == "->") {
        int then_index = next_cs++;
        int else_index = next_cs++;
        auto *delta_then = new CseNode(ObjType::DELTA, std::to_string(then_index));
        auto *delta_else = new CseNode(ObjType::DELTA, std::to_string(else_index));
        auto *beta = new CseNode(ObjType::BETA, "");

        cs->add_node(*delta_then);
        cs->add_node(*delta_else);
        cs->add_node(*beta);

        auto *then_cs = new ControlStructure(then_index);
        control_structures.push_back(then_cs);
        create_cs(root->getChildren()[1], then_cs, then_index);

        auto *else_cs = new ControlStructure(else_index);
        control_structures.push_back(else_cs);
        create_cs(root->getChildren()[2], else_cs, else_index);

        create_cs(root->getChildren()[0], cs, current_cs_index);
    } else if (isOperator(root->getLabel())) {
        auto *op = new CseNode(ObjType::OPERATOR, root->getLabel());
        cs->add_node(*op);

        for (auto &child: root->getChildren()) {
            create_cs(child, cs, current_cs_index);
        }
    } else if (root->getLabel() == "gamma") {
        auto *gamma = new CseNode(ObjType::GAMMA, "");
        cs->add_node(*gamma);

        for (auto &child: root->getChildren()) {
            create_cs(child, cs, current_cs_index);
        }
    } else if (root->getLabel() == "identifier" || root->getLabel() == "integer" || root->getLabel() == "string") {
        std::string value = root->getValue();
        std::string type = root->getLabel();
        CseNode *leaf;

        if (type == "identifier") {
            leaf = new CseNode(ObjType::IDENTIFIER, value);
        } else if (type == "integer") {
            leaf = new CseNode(ObjType::INTEGER, value);
        } else if (type == "string") {
            leaf = new CseNode(ObjType::STRING, value);
        } else {
            throw std::runtime_error("Invalid leaf type: " + type);
        }

        cs->add_node(*leaf);
    } else {
        throw std::runtime_error("Invalid node type: " + root->getLabel() + "Value: " + root->getValue());
    }
}

void CSE::evaluate() {
    auto *e0 = new CseNode(ObjType::ENV, "0");
    main_control_structure.add_node(*e0);
    stack.add_node(*e0);
    env_stack.push_back(next_env++);
    envs[0] = new Env(nullptr);

    main_control_structure.push_control_structure(*control_structures[0]);

    CseNode top_of_cs = main_control_structure.pop_and_return_last_node();

    while ((top_of_cs.get_node_type() != ObjType::ENV) || (top_of_cs.get_node_value() != "0")) {
        if (top_of_cs.get_node_type() == ObjType::INTEGER || top_of_cs.get_node_type() == ObjType::STRING) {
            stack.add_node(top_of_cs);
            top_of_cs = main_control_structure.pop_and_return_last_node();
        } else if (top_of_cs.get_node_type() == ObjType::IDENTIFIER) {
            // if node value is in built_in_functions add the node to the stack
            if (std::find(built_in_functions.begin(), built_in_functions.end(), top_of_cs.get_node_value()) !=
                built_in_functions.end()) {
                stack.add_node(top_of_cs);
            } else if (top_of_cs.get_node_value() == "nil") {
                stack.add_node(CseNode(ObjType::LIST, std::vector<std::pair<ObjType, std::string>>()));
            } else {
                std::pair<ObjType, std::string> value;
                CseNode value_l;
                std::vector<std::pair<ObjType, std::string>> list;

                try {
                    value = envs[env_stack.back()]->get_variable(top_of_cs.get_node_value());
                    stack.add_node(CseNode(value.first, value.second));
                }
                catch (std::runtime_error &e) {
                    try {
                        value_l = envs[env_stack.back()]->get_lambda(top_of_cs.get_node_value());
                        stack.add_node(value_l);
                    }
                    catch (std::runtime_error &e) {
                        try {
                            list = envs[env_stack.back()]->get_list(top_of_cs.get_node_value());
                            stack.add_node(CseNode(ObjType::LIST, list));
                        }
                        catch (std::runtime_error &e) {
                            throw std::runtime_error("Variable not found: " + top_of_cs.get_node_value());
                        }
                    }
                }
            }

            top_of_cs = main_control_structure.pop_and_return_last_node();
        } else if (top_of_cs.get_node_type() == ObjType::LAMBDA) {
            int current_env = env_stack.back();
            stack.add_node(top_of_cs.set_env(current_env));

            top_of_cs = main_control_structure.pop_and_return_last_node();
        } else if (top_of_cs.get_node_type() == ObjType::GAMMA) {
            CseNode top_of_stack = stack.pop_and_return_last_node();

            if (top_of_stack.get_node_type() == ObjType::LAMBDA) {
                Env *new_env = new Env(envs[top_of_stack.get_env()]);
                envs[next_env++] = new_env;

                CseNode value = stack.pop_and_return_last_node();

                if (value.get_node_type() == ObjType::LAMBDA || value.get_node_type() == ObjType::EETA) {
                    new_env->add_lambda(top_of_stack.get_node_value(), value);
                } else if (value.get_node_type() == ObjType::STRING || value.get_node_type() == ObjType::INTEGER) {
                    new_env->add_variable(top_of_stack.get_node_value(),
                                          std::pair(value.get_node_type(), value.get_node_value()));
                } else if (value.get_node_type() == ObjType::LIST) {
                    new_env->add_list(top_of_stack.get_node_value(), value.get_list_elements());
                } else {
                    throw std::runtime_error("Invalid object for gamma: " + value.get_node_value());
                }

                env_stack.push_back(next_env - 1);
                auto *env_obj = new CseNode(ObjType::ENV, std::to_string(next_env - 1));
                main_control_structure.add_node(*env_obj);
                stack.add_node(*env_obj);
                main_control_structure.push_control_structure(*control_structures[top_of_stack.get_cs_index()]);
            } else if (top_of_stack.get_node_type() == ObjType::IDENTIFIER) {
                // TODO: built-in functions should be handled here
                std::string identifier = top_of_stack.get_node_value();

                if (identifier == "Print") {
                    CseNode value = stack.pop_and_return_last_node();
                    std::vector<std::pair<ObjType, std::string>> list_elements = value.get_list_elements();

                    if (value.get_node_type() == ObjType::LIST) {
                        std::cout << "(";

                        std::vector<int> count_stack;

                        for (int i = 0; i < value.get_list_elements().size(); i++) {
                            if (list_elements[i].first == ObjType::LIST) {
                                count_stack.push_back(std::stoi(list_elements[i].second));
                                std::cout << "(";
                            } else {
                                std::cout << list_elements[i].second;

                                if (!count_stack.empty()) {
                                    // reduce 1 from all elements in count_stack
                                    for (int &count: count_stack) {
                                        count--;
                                    }

                                    if (count_stack[count_stack.size() - 1] == 0) {
                                        if (i != value.get_list_elements().size() - 1)
                                            std::cout << "), ";
                                        else
                                            std::cout << ")";

                                        count_stack.pop_back();
                                    } else {
                                        if (i != value.get_list_elements().size() - 1)
                                            std::cout << ", ";
                                    }
                                } else {
                                    if (i != value.get_list_elements().size() - 1)
                                        std::cout << ", ";
                                }
                            }
                        }
                        std::cout << ")" << std::endl;
                    } else if (value.get_node_type() == ObjType::ENV || value.get_node_value() == "dummy") {
                        std::cout << "dummy";
                    } else if (value.get_node_type() == ObjType::LAMBDA) {
                        std::cout << "[lambda closure: ";
                        std::cout << value.get_node_value() << ": ";
                        std::cout << value.get_cs_index() << "]";
                    } else {
                        std::cout << value.get_node_value();
                    }
                } else if (identifier == "Isinteger") {
                    CseNode value = stack.pop_and_return_last_node();
                    if (value.get_node_type() == ObjType::INTEGER) {
                        stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                    } else {
                        stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                    }
                } else if (identifier == "Isstring") {
                    CseNode value = stack.pop_and_return_last_node();
                    if (value.get_node_type() == ObjType::STRING) {
                        stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                    } else {
                        stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                    }
                } else if (identifier == "Isempty") {
                    CseNode value = stack.pop_and_return_last_node();
                    if (value.get_node_type() == ObjType::LIST) {
                        if (value.get_list_elements().empty()) {
                            stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                        } else {
                            stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                        }
                    } else {
                        throw std::runtime_error("Invalid type for IsEmpty: " + value.get_node_value());
                    }
                } else if (identifier == "Istuple") {
                    CseNode value = stack.pop_and_return_last_node();

                    if (value.get_node_type() == ObjType::LIST) {
                        stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                    } else {
                        stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                    }
                } else if (identifier == "Order") {
                    CseNode value = stack.pop_and_return_last_node();
                    if (value.get_node_type() == ObjType::LIST) {
                        stack.add_node(
                                CseNode(ObjType::INTEGER, std::to_string(value.get_list_elements().size())));
                    } else {
                        throw std::runtime_error("Invalid type for Order: " + value.get_node_value());
                    }
                } else if (identifier == "Conc") {
                    CseNode first_arg = stack.pop_and_return_last_node();
                    CseNode second_arg = stack.pop_and_return_last_node();
                    main_control_structure.pop_last_node();

                    if (first_arg.get_node_type() == ObjType::STRING &&
                        (second_arg.get_node_type() == ObjType::STRING ||
                         second_arg.get_node_type() == ObjType::INTEGER)) {
                        stack.add_node(
                                CseNode(ObjType::STRING, first_arg.get_node_value() + second_arg.get_node_value()));
                    } else {
                        throw std::runtime_error("Invalid type for Conc: " + first_arg.get_node_value());
                    }
                } else if (identifier == "Stem") {
                    CseNode arg = stack.pop_and_return_last_node();

                    if (arg.get_node_type() == ObjType::STRING) {
                        stack.add_node(CseNode(ObjType::STRING, arg.get_node_value().substr(0, 1)));
                    } else {
                        throw std::runtime_error("Invalid type for Stem: " + top_of_stack.get_node_value());
                    }
                } else if (identifier == "Stern") {
                    CseNode arg = stack.pop_and_return_last_node();

                    if (arg.get_node_type() == ObjType::STRING) {
                        stack.add_node(CseNode(ObjType::STRING, arg.get_node_value().substr(1)));
                    } else {
                        throw std::runtime_error("Invalid type for Stern: " + top_of_stack.get_node_value());
                    }
                } else if (identifier == "Y*") {
                    CseNode lambda = stack.pop_and_return_last_node();

                    if (lambda.get_node_type() == ObjType::LAMBDA) {
                        if (lambda.get_is_single_bound_var()) {
                            stack.add_node(CseNode(ObjType::EETA, lambda.get_node_value(), lambda.get_cs_index(),
                                                   lambda.get_env()));
                        } else {
                            stack.add_node(CseNode(ObjType::EETA, lambda.get_cs_index(), lambda.get_var_list(),
                                                   lambda.get_env()));
                        }
                    } else {
                        throw std::runtime_error("Invalid type for Y*: " + lambda.get_node_value());
                    }
                }
            } else if (top_of_stack.get_node_type() == ObjType::EETA) {
                stack.add_node(top_of_stack);

                if (top_of_stack.get_is_single_bound_var()) {
                    stack.add_node(CseNode(ObjType::LAMBDA, top_of_stack.get_node_value(), top_of_stack.get_cs_index(),
                                           top_of_stack.get_env()));
                } else {
                    stack.add_node(CseNode(ObjType::LAMBDA, top_of_stack.get_cs_index(), top_of_stack.get_var_list(),
                                           top_of_stack.get_env()));
                }

                main_control_structure.add_node(CseNode(ObjType::GAMMA, ""));
                main_control_structure.add_node(CseNode(ObjType::GAMMA, ""));
            } else if (top_of_stack.get_node_type() == ObjType::LIST) {
                CseNode second_arg = stack.pop_and_return_last_node();

                if (second_arg.get_node_type() == ObjType::INTEGER) {
                    int index = std::stoi(second_arg.get_node_value());
                    if (index < 0 || index > top_of_stack.get_list_elements().size()) {
                        throw std::runtime_error("Index out of bounds: " + second_arg.get_node_value());
                    } else {
                        std::vector<std::pair<ObjType, std::string>> list_elements = top_of_stack.get_list_elements();

                        int i = 1;
                        std::pair<ObjType, std::string> element;

                        for (auto &list_element: list_elements) {
                            if (i == index) {
                                element = list_element;
                                break;
                            }
                            i++;
                        }

                        stack.add_node(CseNode(ObjType::INTEGER, element.second));
                    }
                } else {
                    throw std::runtime_error("Invalid type for Index: " + second_arg.get_node_value());
                }
            }

            top_of_cs = main_control_structure.pop_and_return_last_node();
        } else if (top_of_cs.get_node_type() == ObjType::ENV) {
            std::vector<CseNode> env_nodes = {};

            CseNode st_node = stack.pop_and_return_last_node();

            while (st_node.get_node_type() != ObjType::ENV) {
                env_nodes.push_back(st_node);
                st_node = stack.pop_and_return_last_node();
            }

            // push back the stack from vector
            for (auto it = env_nodes.rbegin(); it != env_nodes.rend(); ++it) {
                stack.add_node(*it);
            }

            env_stack.pop_back();

            top_of_cs = main_control_structure.pop_and_return_last_node();
        } else if (top_of_cs.get_node_type() == ObjType::OPERATOR) {
            std::string operator_ = top_of_cs.get_node_value();

            CseNode first = stack.pop_and_return_last_node();
            CseNode second = stack.pop_and_return_last_node();

            if (operator_ == "+") {
                stack.add_node(CseNode(ObjType::INTEGER, std::to_string(
                        std::stoi(first.get_node_value()) + std::stoi(second.get_node_value()))));
            } else if (operator_ == "-") {
                stack.add_node(CseNode(ObjType::INTEGER, std::to_string(
                        std::stoi(first.get_node_value()) - std::stoi(second.get_node_value()))));
            } else if (operator_ == "/") {
                stack.add_node(CseNode(ObjType::INTEGER, std::to_string(
                        std::stoi(first.get_node_value()) / std::stoi(second.get_node_value()))));
            } else if (operator_ == "*") {
                stack.add_node(CseNode(ObjType::INTEGER, std::to_string(
                        std::stoi(first.get_node_value()) * std::stoi(second.get_node_value()))));
            } else if (operator_ == "neg") {
                stack.add_node(second);
                stack.add_node(CseNode(ObjType::INTEGER, std::to_string(-std::stoi(first.get_node_value()))));
            } else if (operator_ == "not") {
                stack.add_node(second);
                if (first.get_node_value() == "true") {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                }
            } else if (operator_ == "eq") {
                if (first.get_node_value() == second.get_node_value()) {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                }
            } else if (operator_ == "gr") {
                if (std::stoi(first.get_node_value()) > std::stoi(second.get_node_value())) {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                }
            } else if (operator_ == "ge") {
                if (std::stoi(first.get_node_value()) >= std::stoi(second.get_node_value())) {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                }
            } else if (operator_ == "ls") {
                if (std::stoi(first.get_node_value()) < std::stoi(second.get_node_value())) {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                }
            } else if (operator_ == "le") {
                if (std::stoi(first.get_node_value()) <= std::stoi(second.get_node_value())) {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                }
            } else if (operator_ == "ne") {
                if (first.get_node_value() != second.get_node_value()) {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                }
            } else if (operator_ == "aug") {
                if (first.get_node_type() == ObjType::LIST) {
                    if (second.get_node_type() == ObjType::LIST) {
                        std::vector<std::pair<ObjType, std::string>> elements = first.get_list_elements();
                        std::vector<std::pair<ObjType, std::string>> elements_2 = second.get_list_elements();

                        elements.emplace_back(ObjType::LIST, std::to_string(elements_2.size()));

                        for (auto &element: elements_2) {
                            elements.push_back(element);
                        }

                        stack.add_node(CseNode(ObjType::LIST, elements));
                    } else if (second.get_node_type() == ObjType::INTEGER ||
                               second.get_node_type() == ObjType::BOOLEAN ||
                               second.get_node_type() == ObjType::STRING) {
                        std::vector<std::pair<ObjType, std::string>> elements = first.get_list_elements();

                        elements.emplace_back(second.get_node_type(), second.get_node_value());
                        stack.add_node(CseNode(ObjType::LIST, elements));
                    } else {
                        throw std::runtime_error("Invalid type for aug: " + second.get_node_value());
                    }
                } else {
                    throw std::runtime_error("Invalid type for aug: " + first.get_node_value());
                }
            } else if (operator_ == "or") {
                if (first.get_node_value() == "true" || second.get_node_value() == "true") {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "true"));
                } else {
                    stack.add_node(CseNode(ObjType::BOOLEAN, "false"));
                }
            } else {
                throw std::runtime_error("Invalid operator: " + operator_);
            }

            top_of_cs = main_control_structure.pop_and_return_last_node();
        } else if (top_of_cs.get_node_type() == ObjType::TAU) {
            std::vector<std::pair<ObjType, std::string>> tau_elements;
            int tau_size = std::stoi(top_of_cs.get_node_value());

            for (int i = 0; i < tau_size; i++) {
                CseNode node = stack.pop_and_return_last_node();

                if (node.get_node_type() == ObjType::LIST) {
                    std::vector<std::pair<ObjType, std::string>> elements = node.get_list_elements();
                    tau_elements.emplace_back(ObjType::LIST, std::to_string(elements.size()));

                    for (auto &element: elements) {
                        tau_elements.push_back(element);
                    }
                } else {
                    tau_elements.emplace_back(node.get_node_type(),
                                              node.get_node_value());
                }
            }

            stack.add_node(CseNode(ObjType::LIST, tau_elements));

            top_of_cs = main_control_structure.pop_and_return_last_node();
        } else if (top_of_cs.get_node_type() == ObjType::BETA) {
            CseNode node = stack.pop_and_return_last_node();

            if (node.get_node_type() == ObjType::BOOLEAN) {
                if (node.get_node_value() == "true") {
                    main_control_structure.pop_last_node();
                    CseNode true_node = main_control_structure.pop_and_return_last_node();

                    if (true_node.get_node_type() == ObjType::DELTA) {
                        main_control_structure.push_control_structure(
                                *control_structures[std::stoi(true_node.get_node_value())]);
                    } else {
                        throw std::runtime_error("Invalid type for beta: " + true_node.get_node_value());
                    }
                } else {
                    CseNode false_node = main_control_structure.pop_and_return_last_node();
                    main_control_structure.pop_last_node();

                    if (false_node.get_node_type() == ObjType::DELTA) {
                        main_control_structure.push_control_structure(
                                *control_structures[std::stoi(false_node.get_node_value())]);
                    } else {
                        throw std::runtime_error("Invalid type for beta: " + false_node.get_node_value());
                    }
                }
            } else if (node.get_node_type() == ObjType::INTEGER) {
                if (node.get_node_value() != "0") {
                    main_control_structure.pop_last_node();
                    CseNode true_node = main_control_structure.pop_and_return_last_node();

                    if (true_node.get_node_type() == ObjType::DELTA) {
                        main_control_structure.push_control_structure(
                                *control_structures[std::stoi(true_node.get_node_value())]);
                    } else {
                        throw std::runtime_error("Invalid type for beta: " + true_node.get_node_value());
                    }
                } else {
                    CseNode false_node = main_control_structure.pop_and_return_last_node();
                    main_control_structure.pop_last_node();

                    if (false_node.get_node_type() == ObjType::DELTA) {
                        main_control_structure.push_control_structure(
                                *control_structures[std::stoi(false_node.get_node_value())]);
                    } else {
                        throw std::runtime_error("Invalid type for beta: " + false_node.get_node_value());
                    }
                }
            } else {
                throw std::runtime_error("Invalid type for beta: " + node.get_node_value());
            }

            top_of_cs = main_control_structure.pop_and_return_last_node();
        }
    }
}

bool isOperator(const std::string &label) {
    std::vector<std::string> operators_ = {"+", "-", "/", "*", "aug", "neg", "not", "eq", "gr", "ge", "ls", "le", "ne", "or"};

    auto it = std::find(operators_.begin(), operators_.end(), label);
    return it != operators_.end();
}
