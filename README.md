# RPAL Interpreter
The RPAL Interpreter is a C++ program designed to interpret the RPAL programming language. This project serves as the final assessment for the CS3513 course at the University of Moratuwa.

 ## Cloning the Repository

To get started, you'll need to clone the project repository. Open your terminal and execute the following command:

    git clone https://github.com/ndranathunga/rpal-interpreter.git

This will create a local copy of the repository on your machine.

## Running the Interpreter

Once you've cloned the repository, you can navigate into the project directory and build/run the interpreter.

    cd rpal-interpreter
    make

The above commands will compile and build the interpreter. If the compilation is successful, you can then execute the interpreter to interpret RPAL programs.

Please ensure you have a C++ compiler and the `make` utility installed on your system.

## Using the Interpreter

    ./rpal20 <input_file>

Replace <input_file.rpal with the actual path to your RPAL program file.

## Visualizing AST and ST

The RPAL Interpreter supports visualization of the Abstract Syntax Tree (AST) and Symbol Table (ST) using Graphviz. You can use the `--visualize` option to generate and visualize these trees. If you provide a specific value (e.g., `ast` or `st`) after `--visualize`, only that tree will be generated. If you use `--visualize` without specifying a value, both trees will be compiled and visualized.

Make sure you have Graphviz installed on your local machine before using this feature.

### Installing Graphviz

Graphviz is required to visualize the trees. You can download Graphviz from the official website: https://www.graphviz.org/

Follow the installation instructions provided on the Graphviz website to install it on your system.

Example commands for visualizing the trees:

<ul>
 <li>
  To visualize both AST and ST:

    ./rpal20 <input_file> --visualize
  
 </li>
 <li>
  To visualize only the AST:

    ./rpal20 <input_file>  --visualize=ast

 </li>
 <li>
  To visualize only the ST:

    ./rpal20 <input_file> --visualize=st

 </li>
</ul>

The generated PNG files for the visualizations will be available in a folder called `Visualizations` within the interpreter directory.
