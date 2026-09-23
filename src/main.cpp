#include <iostream>

#include "Support/Source.hpp"
#include "Support/SourceCursor.hpp"
#include "Support/SourceReader.hpp"

#include "Lexer/Lexer.hpp"

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

    return 0;
}