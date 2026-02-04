/**
 * \file cmdline.cpp
 * \brief Implementation of command-line argument handling.
 *
 * This file implements the logic for parsing arguments. It includes the
 * Catch2 runner configuration to enable testing via the `--test` flag.
 */

#define CATCH_CONFIG_RUNNER // Tells Catch2 to use our custom main
#include "catch.h"
#include "cmdline.h"
#include <iostream>
#include <string>
#include <cstdlib>

void use_arguments(int argc, char **argv) {
    bool test_seen = false;

    // Start from 1 to skip the program name (argv[0])
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--help") {
            std::cout << "Allowed arguments:" << std::endl;
            std::cout << "  --help : Show this help text." << std::endl;
            std::cout << "  --test : Run tests." << std::endl;
            exit(0);
        }
        else if (arg == "--test") {
            if (test_seen) {
                std::cerr << "Error: '--test' seen more than once." << std::endl;
                exit(1);
            }
            
            // Run Catch2 tests. Returns 0 on success, non-zero on failure.
            if (Catch::Session().run() != 0) {
                std::cerr << "Tests failed!" << std::endl;
                exit(1);
            }
            
            // If tests passed
            // std::cout << "Tests passed" << std::endl;
            test_seen = true;
        }
        else {
            std::cerr << "Error: Unknown argument '" << arg << "'." << std::endl;
            exit(1);
        }
    }
}
