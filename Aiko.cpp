#include "Aiko.h"

#include <vector>

int main(int argc, char** argv) {
    std::vector<std::string> filesToCompile;
    
    for (int i = 0; i < argc; i++) {
        std::string arg = std::string(argv[i]);

        if (EndsWith(arg, ".aiko")) {
            filesToCompile.push_back(arg);
        } else if (arg.rfind("/", 0) == 0) {
            ParseParameter(arg.substr(1, arg.length()));
        }
    }

    printf("Starting Process\n\n");
    
    for(std::string path : filesToCompile)  {
        printf("%s\n", path.c_str());
        std::ifstream input_file(path);

        if (!input_file.is_open()) {
            printf("Unable to read file. Skipping..\n\n");
            continue;
        }
        std::string src = std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
        
        Lexer lexer(src);

        for (Token t = lexer.NextToken(); t.type != TokenType::EndOfFile; t = lexer.NextToken()) {
            printf("Tk \"%s\" %i (Type: %i)\n", t.value.c_str(), t.line, t.type);
        }
        printf("\n");
    }

    return 0;
}

void ParseParameter(std::string arg) {
    printf("option %s\n", arg.c_str());
}