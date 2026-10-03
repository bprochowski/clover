#pragma once

#include "Ast/Function.hpp"
#include "Ast/Return.hpp"

#include <filesystem>

class AsmGenerator {
public:
    std::string emit(ast::Function const& fn);
    std::string emit(ast::Return const& return_stmt);
};