#include "AsmGenerator.hpp"

#include <format>

namespace {
std::string wrap_if_main(std::string const& fn_name)
{
    if (fn_name == "main") {
        return std::format("_{}", fn_name); // main function name expected by linker
    }

    return fn_name;
}
}

std::string AsmGenerator::emit(ast::Function const& fn)
{
    auto fn_name = wrap_if_main(fn.name());

    return std::format(
        ".global {}\n\n"
        "{}:\n"
        "    push rbp\n"
        "    mov  rbp, rsp\n"
        "{}\n",
        fn_name, fn_name, emit(fn.body()));
}

std::string AsmGenerator::emit(ast::Return const& return_stmt)
{
    return std::format(
        "    mov  eax, {}\n"
        "    pop  rbp\n"
        "    ret\n",
        return_stmt.value());
}