cmake_minimum_required(VERSION 3.20)
project(clover CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(clover
    src/main.cpp

    src/Ast/Function.cpp
    src/Ast/Return.cpp

    src/CodeGen/AsmGenerator.cpp
    
    src/Lexer/Lexer.cpp
    src/Lexer/Token.cpp

    src/Parser/Parser.cpp

    src/Support/Source.cpp
    src/Support/SourceReader.cpp
    src/Support/SourceCursor.cpp
    src/Support/TokenCursor.cpp
)

target_include_directories(clover PRIVATE src)
