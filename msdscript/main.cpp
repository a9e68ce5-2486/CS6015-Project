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
#include "env.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>

static std::string read_program_from_stdin() {
    if (isatty(STDIN_FILENO)) {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    return buffer.str();
}

/**
 * \brief Main entry point.
 * Parses input from stdin based on the selected mode.
 */
int main(int argc, char **argv) {
    try {
        run_mode_t mode = use_arguments(argc, argv);

        if (mode == do_nothing) {
            return 0;
        }

        std::string input = read_program_from_stdin();
        PTR(Expr) e = parse_str(input);

        if (mode == do_interp) {
            std::cout << e->interp(Env::empty)->to_string() << std::endl;
        }
        else if (mode == do_print) {
            e->printExp(std::cout);
            std::cout << std::endl;
        }
        else if (mode == do_pretty_print) {
            std::cout << e->to_pretty_string() << std::endl;
        }

        return 0;
    } catch (std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}
