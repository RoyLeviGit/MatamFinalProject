#include <iostream>
#include <fstream>
#include "Parser.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        Calculator calculator;
        Parser parser(std::cin, std::cout, calculator, Parser::prompt);
        try {
            parser.parse();
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 0;
        } catch (...) {
            std::cerr << "Error: unknown error" << std::endl;
        }
    } else if (argc == 3) {
        std::string fileName = argv[1];
        std::ifstream inputFile(argv[1]);
        if (!inputFile) {
            std::cerr << "Error: cannot open file " << argv[1] << std::endl;
            return 0;
        }
        std::ofstream outputFile(argv[2]);
        if (!outputFile) {
            std::cerr << "Error: cannot open file " << argv[2] << std::endl;
            return 0;
        }
        Calculator calculator;
        Parser parser(inputFile, outputFile, calculator, Parser::batch);
        try {
            parser.parse();
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 0;
        } catch (...) {
            std::cerr << "Error: unknown error" << std::endl;
        }
    } else {
        // expected 1 or 3 arguments: the program name, [the source path] and [the destination path]
        std::cerr << "Error: invalid usage. Usage: " << argv[0] << "SOURCE DESTINATION" << std::endl;
    }
    return 0;
}
