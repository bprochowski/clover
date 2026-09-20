cmake_minimum_required(VERSION 3.20)
project(clover CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(clover
    src/main.cpp
    
    src/Lexer/Lexer.cpp

    src/Support/Source.cpp
    src/Support/SourceReader.cpp
    src/Support/SourceCursor.cpp
)

target_include_directories(clover PRIVATE src)
