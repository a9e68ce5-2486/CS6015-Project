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
 * multiplication, variables, and let-bindings.
 *
 * \section usage_sec Usage
 *
 * Run the program from the command line:
 * - `./msdscript --test` : Run the test suite.
 * - `./msdscript --interp` : Parse and evaluate an expression from stdin.
 * - `./msdscript --print` : Parse and print (strict) an expression from stdin.
 * - `./msdscript --pretty-print` : Parse and pretty-print an expression from stdin.
 *
 * \author MSDScript Student
 * \date 2026
 */

#include "cmdline.h"
#include "expr.h"
#include "parse.h"
#include <iostream>

/**
 * \brief Main entry point.
 * Parses input from stdin based on the selected mode.
 */
int main(int argc, char **argv) {
    run_mode_t mode = use_arguments(argc, argv);

    // If no specific mode is set (and use_arguments didn't exit), do nothing.
    if (mode == do_nothing) {
        return 0;
    }

    try {
        Expr* e = parse(std::cin);

        if (mode == do_interp) {
            std::cout << e->interp() << std::endl;
        }
        else if (mode == do_print) {
            e->printExp(std::cout);
            std::cout << std::endl;
        }
        else if (mode == do_pretty_print) {
            std::cout << e->to_pretty_string() << std::endl;
        }

        // Cleanup (optional but good practice)
        // delete e;
        return 0;

    } catch (std::runtime_error &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
}
