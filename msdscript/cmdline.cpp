#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "cmdline.h"
#include <iostream>
#include <string>
#include <cstdlib>

void use_arguments(int argc, char **argv) {
    bool test_seen = false;

    // 从 1 开始，因为 argv[0] 是程序名
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
            
            if(Catch::Session().run() != 0){
                exit(1) ;
            }
            test_seen = true ;
        } 
        else {
            std::cerr << "Error: Unknown argument '" << arg << "'." << std::endl;
            exit(1);
        }
    }
}
