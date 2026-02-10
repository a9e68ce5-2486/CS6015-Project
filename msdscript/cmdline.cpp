/**
 * \file cmdline.cpp
 * \brief Implementation of command-line argument handling.
 */

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "cmdline.h"
#include <iostream>
#include <string>
#include <cstdlib>

run_mode_t use_arguments(int argc, char **argv) {
    bool test_seen = false;
    run_mode_t mode = do_nothing;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--help") {
            std::cout << "Usage: ./msdscript [--test] [--interp] [--print] [--pretty-print]" << std::endl;
            exit(0);
        }
        else if (arg == "--test") {
            if (test_seen) {
                std::cerr << "Error: '--test' seen more than once." << std::endl;
                exit(1);
            }
            // Run tests and exit
            if (Catch::Session().run() != 0) {
                exit(1);
            }
            test_seen = true;
            exit(0);
        }
        else if (arg == "--interp") {
            mode = do_interp;
        }
        else if (arg == "--print") {
            mode = do_print;
        }
        else if (arg == "--pretty-print") {
            mode = do_pretty_print;
        }
        else {
            std::cerr << "Error: Unknown argument '" << arg << "'." << std::endl;
            exit(1);
        }
    }
    
    return mode;
}
