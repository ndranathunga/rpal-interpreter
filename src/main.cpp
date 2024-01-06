#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "Parser.h"
#include "CSE.h"
#include "Viz.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || std::string(argv[1]) == "-visualize")
    {
        std::cout << "\033[1;31mERROR: \033[0m"
                  << "Usage: .\\rpal20 input_file [-visualize=VALUE]"
                  << "\n"
                  << std::endl;
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

    if (!isGraphvizInstalled() && (visualizeAst || visualizeSt))
    {
        printGraphvizWarning();
        visualizeAst = false;
        visualizeSt = false;
    }

    if (isGraphvizInstalled() && (visualizeSt || visualizeAst)) {
        if (!std::filesystem::exists("Visualizations")) {
            std::filesystem::create_directory("Visualizations");
        }
    }

    Lexer lexer(input);

    TokenStorage &tokenStorage = TokenStorage::getInstance();
    tokenStorage.setLexer(lexer);

//     Token token;
//     do
//     {
//         token = TokenStorage::getInstance().pop();
//         std::cout << "Type: " << gettoken_typeName(token.type) << ", Value: " << token.value << std::endl;
//         // std::cout << token.value << std::endl;
//     } while (token.type != token_type::END_OF_FILE);
//
//     TokenStorage::getInstance().reset();

    Parser::parse();
    TokenStorage::destroyInstance();

    TreeNode *root = Tree::getInstance().getASTRoot();

    if (visualizeAst)
    {
        // Generate the DOT file
        generateDotFile(root, "ast.dot");

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

        std::string dotFilePath_st = "Visualizations\\st.dot";
        std::string outputFilePath_st = "Visualizations\\st.png";

        std::string command_st = "dot -Tpng -Gdpi=150 " + dotFilePath_st + " -o " + outputFilePath_st;
        system(command_st.c_str());

        std::cout << "The st.png file is located in the Visualizations folder." << std::endl;
    }

    CSE cse = CSE();
    cse.create_cs(Tree::getInstance().getSTRoot());
    cse.evaluate();

    std::cout << std::endl;

    return 0;
}