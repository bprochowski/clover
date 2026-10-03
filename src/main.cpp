#include <fstream>
#include <iostream>

#include "CodeGen/AsmGenerator.hpp"

#include "Support/Source.hpp"
#include "Support/SourceCursor.hpp"
#include "Support/SourceReader.hpp"

#include "Lexer/Lexer.hpp"
#include "Parser/Parser.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [source-file]" << std::endl;
        return 1;
    }

    auto const source_text = source_reader::load(argv[1]);
    if (!source_text.has_value()) {
        std::cout << source_text.error() << std::endl;
        return 1;
    }

    Source source(source_text.value());
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    if (!tokens.has_value()) {
        for (auto const& error : tokens.error()) {
            std::cout << error << std::endl;
        }

        exit(1);
    }

    Parser parser(tokens.value());
    auto fn_node = parser.build_ast();

    auto const generated_asm = AsmGenerator { }.emit(fn_node);

    std::ofstream out_file("clo.asm");
    if (!out_file.is_open()) {
        std::cout << "Filed to write .asm file" << std::endl;
        exit(1);
    }

    out_file << ".intel_syntax noprefix" << std::endl;
    out_file << generated_asm;
    out_file.close();

    return 0;
}