#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "generation.hpp"
#include "parser.hpp"
#include "tokenization.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Incorrect number of arguments." << std::endl;
        std::cerr << "Usage: bdbc <file.dream>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string content;
    {
        std::stringstream content_stream;
        std::ifstream input(argv[1]);
        content_stream << input.rdbuf();
        content = content_stream.str();
    }

    Tokenizer tokenizer(std::move(content));
    std::vector<Token> tokens = tokenizer.tokenize();

    Parser parser(std::move(tokens));
    std::optional<node::S> tree = parser.parse();

    if (!tree.has_value())
    {
        std::cerr << "Unable to parse statement!" << std::endl;
        exit(EXIT_FAILURE);
    }

    Generator generator(tree.value());

    {
        std::ofstream file("build/out.asm");
        file << generator.generate();
    }

    system("nasm -felf64 build/out.asm");
    system("ld -o build/out build/out.o");

    return EXIT_SUCCESS;
}