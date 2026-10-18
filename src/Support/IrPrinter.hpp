#pragma once

#include <iostream>
#include <string>
#include <variant>

#include "IR/Function.hpp"

inline char const* bin_op_name(ir::BinaryOperation::Kind const kind)
{
    switch (kind) {
    case ir::BinaryOperation::Kind::add:
        return "add";
    case ir::BinaryOperation::Kind::sub:
        return "sub";
    case ir::BinaryOperation::Kind::mul:
        return "mul";
    case ir::BinaryOperation::Kind::div:
        return "div";
    }

    return "?";
}

struct IrPrinter {
    std::string operator()(ir::Constant const& value) const
    {
        return value.value;
    }

    std::string operator()(ir::VirtualVar const& var) const
    {
        return "%" + var.id;
    }

    void operator()(ir::Branch const& inst) const
    {
        std::cout << "  br_if " << std::visit(*this, inst.condition) << ", " << inst.then_ << ", " << inst.else_ << std::endl;
    }

    void operator()(ir::BinaryOperation const& inst) const
    {
        std::cout << "  " << (*this)(inst.get_destination()) << " = " << bin_op_name(inst.get_kind()) << " "
                  << std::visit(*this, inst.lhs()) << ", " << std::visit(*this, inst.rhs()) << std::endl;
    }

    void operator()(ir::Load const& inst) const
    {
        std::cout << "  " << (*this)(inst.get_destination()) << " = load " << inst.get_name() << std::endl;
    }

    void operator()(ir::Store const& inst) const
    {
        std::cout << "  store " << inst.get_name() << ", " << std::visit(*this, inst.value()) << std::endl;
    }

    void operator()(ir::Define const& inst) const
    {
        std::cout << "  define " << inst.get_name() << std::endl;
    }

    void operator()(ir::Return const& inst) const
    {
        std::cout << "  ret " << std::visit(*this, inst.value()) << std::endl;
    }

    void operator()(ir::Function const& fn) const
    {
        std::cout << "fn " << fn.name() << ":" << std::endl;
        for (auto const& basic_block : fn.body()) {
            std::cout << basic_block.get_lable() << ":" << std::endl;
            for (auto const& inst : basic_block.get_instructions()) {
                std::visit(*this, inst);
            }
        }
    }

    void operator()(ir::Jump const& jump) const
    {
        std::cout << "  jmp " << jump.lable() << std::endl;
    }
};
