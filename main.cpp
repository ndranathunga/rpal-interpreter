#include <string>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <algorithm>

#include "Parser.h"
#include "Token.h"

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
std::string gettoken_typeName(token_type type)
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
int generateDotFileHelper(TreeNode *node, std::ofstream &file, int parent = -1, int nodeCount = 0)
{
    int currentNode = nodeCount;

    // Increment nodeCount for the next call
    int nextNodeCount = nodeCount + 1;

    // Determine colors and fill based on node values
    std::string labelColor = "darkblue";
    std::string valueColor = "darkgreen";
    std::string fillColor = (node->getValue() == " " || node->getValue() == "") ? "#CCCCCC" : "#FFFFFF";

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
    std::string valueStr = (node->getValue() == "") ? "&nbsp;" : node->getValue();

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
void generateDotFile(TreeNode *root, std::string filename)
{
    // std::ofstream file("Visualizations/tree.dot");
    // std::string f_name = std::string("D:\\Files\\Academics\\Semester 04\\PL\\RPAL_CLION\\Visualizations\\") + filename;
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: .\\rpal20 input_file [-visualize=VALUE]" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    std::ifstream file(filename);

    if (!file.is_open())
    {
        std::cout << "Unable to open file: " << filename << std::endl;
        return 1;
    }

    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Check if the "-visualize" argument is provided
    std::string visualizeArg;
    bool visualizeAst = false;
    bool visualizeSt = false;

    for (int i = 2; i < argc; ++i)
    {
        std::string arg(argv[i]);

        if (arg == "-visualize")
        {
            visualizeAst = true;
            visualizeSt = true;
        }
        else if (arg == "-visualize=ast")
        {
            visualizeAst = true;
        }
        else if (arg == "-visualize=st")
        {
            visualizeSt = true;
        }
    }

    if (!isGraphvizInstalled())
    {
        printGraphvizWarning();
        visualizeAst = false;
        visualizeSt = false;
    }

    Lexer lexer(input);

    TokenStorage &tokenStorage = TokenStorage::getInstance();
    tokenStorage.setLexer(lexer);

    // Token token;
    // do
    // {
    //     token = TokenStorage::getInstance().pop();
    //     std::cout << "Type: " << gettoken_typeName(token.type) << ", Value: " << token.value << std::endl;
    // } while (token.type != token_type::END_OF_FILE);

    // TokenStorage::getInstance().reset();

    Parser::parse();
    TokenStorage::destroyInstance();

    TreeNode *root = Tree::getInstance().getASTRoot();

    if (visualizeAst)
    {
        // Generate the DOT file
        generateDotFile(root, "ast.dot");

        // Tree::releaseASTMemory();

        // // Use Graphviz to generate the graph
        // system("dot -Tpng Visualizations\\tree.dot -o Visualizations\\tree.png");

        // std::string dotFilePath = "\"D:\\Files\\Academics\\Semester 04\\PL\\RPAL_CLION\\Visualizations\\ast.dot\"";
        // std::string outputFilePath = "\"D:\\Files\\Academics\\Semester 04\\PL\\RPAL_CLION\\Visualizations\\ast.png\"";

        std::string dotFilePath = "Visualizations\\ast.dot";
        std::string outputFilePath = "Visualizations\\ast.png";

        std::string command = "dot -Tpng -Gdpi=150 " + dotFilePath + " -o " + outputFilePath;
        system(command.c_str());

        // tree.png folder path message
        std::cout << "The ast.png file is located in the Visualizations folder." << std::endl;
    }

    Tree::generate();
    TreeNode *st_root = Tree::getInstance().getSTRoot();

    if (visualizeSt)
    {
        generateDotFile(st_root, "st.dot");

        // std::string dotFilePath_st = "\"D:\\Files\\Academics\\Semester 04\\PL\\RPAL_CLION\\Visualizations\\st.dot\"";
        // std::string outputFilePath_st = "\"D:\\Files\\Academics\\Semester 04\\PL\\RPAL_CLION\\Visualizations\\st.png\"";

        std::string dotFilePath_st = "Visualizations\\st.dot";
        std::string outputFilePath_st = "Visualizations\\st.png";

        std::string command_st = "dot -Tpng -Gdpi=150 " + dotFilePath_st + " -o " + outputFilePath_st;
        system(command_st.c_str());

        std::cout << "The st.png file is located in the Visualizations folder." << std::endl;
    }

    // open the image
    // system("Visualizations\\tree.png");

    // // delete the DOT file
    // system("del Visualizations\\tree.dot");
    // // delete the image after it is closed
    // system("del Visualizations\\tree.png");

    return 0;
}