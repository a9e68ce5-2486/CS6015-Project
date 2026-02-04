/**
 * \file main.cpp
 * \brief Entry point for the MSDScript program.
 *
 * \mainpage MSDScript Documentation
 *
 * \section intro_sec Introduction
 *
 * Welcome to the MSDScript project documentation.
 * MSDScript is a mathematical expression interpreter capable of parsing,
 * evaluating, and manipulating mathematical formulas involving addition,
 * multiplication, and variables.
 *
 * \section features_sec Features
 *
 * - **Expression Evaluation**: Calculate the integer value of expressions.
 * - **Variable Substitution**: Replace variables with other expressions.
 * - **Pretty Printing**: Format expressions with minimal parentheses based on precedence.
 * - **Command Line Interface**: Support for testing and help commands.
 *
 * \section usage_sec Usage
 *
 * Run the program from the command line:
 * - `./msdscript --help` : Display help message.
 * - `./msdscript --test` : Run the test suite.
 *
 * \author MSDScript Student
 * \date 2024
 */

#include "cmdline.h"

/**
 * \brief Main entry point of the application.
 *
 * Delegates argument processing to use_arguments().
 *
 * \param argc Argument count.
 * \param argv Argument vector.
 * \return 0 on successful execution.
 */
int main(int argc, char **argv) {
    use_arguments(argc, argv);
    return 0;
}
