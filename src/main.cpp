#include <iostream>

#include "Support/SourceCursor.hpp"
#include "Support/SourceReader.hpp"

#include "Lexer/Lexer.hpp"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " [source-file]" << std::endl;
        return 1;
    }

    auto const source = source_reader::load(argv[1]);
    if (!source.has_value()) {
        std::cout << source.error() << std::endl;
        return 1;
    }

    SourceCursor source_cursor(source.value());
    Lexer lexer(source_cursor);

    return 0;
}