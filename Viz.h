//
// Created by nisal on 7/20/2023.
//

#ifndef RPAL_FINAL_VIZ_H
#define RPAL_FINAL_VIZ_H

//
// Created by nisal on 7/19/2023.
//

#include <iostream>
#include <unordered_map>
#include <fstream>

#include "Token.h"
#include "TreeNode.h"

// Map the enum values to their string representations
std::unordered_map<token_type, std::string> token_typeNames = {
        {token_type::IDENTIFIER, "IDENTIFIER"},
        {token_type::INTEGER, "INTEGER"},
        {token_type::STRING, "STRING"},
        {token_type::OPERATOR, "OPERATOR"},
        {token_type::DELIMITER, "DELIMITER"},
        {token_type::KEYWORD, "KEYWORD"},
        {token_type::END_OF_FILE, "END_OF_FILE"}};

// Function to get the string representation of the token_type
[[maybe_unused]] std::string gettoken_typeName(token_type type)
{
    if (token_typeNames.count(type) > 0)
    {
        return token_typeNames[type];
    }
    return "UNKNOWN";
}

// Function to check if Graphviz is installed
bool isGraphvizInstalled()
{
// Redirect stderr to /dev/null or nul to discard error message
#ifdef _WIN32
    const char *redirectCommand = "2>nul";
#else
    const char *redirectCommand = "2>/dev/null";
#endif

    std::string command = "dot -V ";
    command += redirectCommand;

    int exitCode = system(command.c_str());
    return (exitCode == 0);
}

// Function to print a warning message in yellow color with download link to Graphviz
void printYellowWarning(const std::string &message)
{
    std::cout << "\033[1;33m" << message << "\033[0m";
}

// Function to print the warning message if Graphviz is not installed
void printGraphvizWarning()
{
    std::string warningMessage = "WARNING: Graphviz is not installed on this computer.";
    warningMessage += " Download Graphviz from: ";

    printYellowWarning(warningMessage);

    std::cout << "https://graphviz.org/download/\n"
              << std::endl;
}

/**
 * Helper function to generate the dot file contents recursively.
 *
 * @param node The current TreeNode being processed.
 * @param file The ofstream object for writing the dot file.
 * @param parent The parent node ID (default: -1).
 * @return The ID of the current node.
 */
// NOLINTNEXTLINE
int generateDotFileHelper(TreeNode *node, std::ofstream &file, int parent = -1, int nodeCount = 0)
{
    int currentNode = nodeCount;

    // Increment nodeCount for the next call
    int nextNodeCount = nodeCount + 1;

    // Determine colors and fill based on node values
    std::string labelColor = "darkblue";
    std::string valueColor = "darkgreen";
    std::string fillColor = (node->getValue() == " " || node->getValue().empty()) ? "#CCCCCC" : "#FFFFFF";

    // Escape label characters if necessary
    std::string escapedLabel = node->getLabel();
    size_t pos = escapedLabel.find('>');
    while (pos != std::string::npos)
    {
        escapedLabel.replace(pos, 1, "&gt;");
        pos = escapedLabel.find('>', pos + 4);
    }

    // Prepare label and value strings for the dot file
    std::string labelStr = (escapedLabel.empty()) ? "&nbsp;" : escapedLabel;
    std::string valueStr = (node->getValue().empty()) ? "&nbsp;" : node->getValue();

    // Write the node details to the dot file
    file << "    node" << currentNode << " [label=<";
    file << "<font color=\"" << labelColor << "\">" << labelStr << "</font><br/>";
    file << "<font color=\"" << valueColor << "\">" << valueStr << "</font>";
    file << ">, style=filled, fillcolor=\"" << fillColor << "\"];\n";

    // Connect the current node to its parent (if not the root)
    if (currentNode != 0 && parent != -1)
    {
        file << "    node" << parent << " -> node" << currentNode << ";\n";
    }

    // Recursively generate dot file contents for each child node
    for (TreeNode *child : node->getChildren())
    {
        nextNodeCount = generateDotFileHelper(child, file, currentNode, nextNodeCount);
    }

    return nextNodeCount;
}

/**
 * Generates a dot file representing the AST.
 *
 * @param root The root node of the AST.
 */
void generateDotFile(TreeNode *root, const std::string& filename)
{
    std::string f_name = std::string("Visualizations\\") + filename;
    std::ofstream file(f_name);
    if (file.is_open())
    {
        file << "digraph Tree {\n";
        generateDotFileHelper(root, file, 0);
        file << "}\n";
        file.close();
    }
}


#endif //RPAL_FINAL_VIZ_H
