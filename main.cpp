#include <fstream>
#include <iostream>

#include "./services/syntax.highlighter.service.h"

int main(int argc, char* argv[]) {
    std::ifstream inputFile(
        "C:\\Users\\scien\\Desktop\\nekoasm-syntax-highlight\\in.asm");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the file!" << std::endl;
        return 1;
    }

    std::string line;
    std::string full_line;
    std::cout << "File Content: " << std::endl;
    while (getline(inputFile, line)) {
        full_line += line + "\n";
    }

    inputFile.close();
    auto serv = new SyntaxHighlightService(full_line);
    auto res  = serv->InitLexAnalyze();
    printf("%s", res.c_str());
}
