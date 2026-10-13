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

    void operator()(ir::BinaryOperation const& inst) const
    {
        std::cout << "  " << (*this)(inst.get_destination()) << " = " << bin_op_name(inst.get_kind()) << " "
                  << std::visit(*this, inst.lhs()) << ", " << std::visit(*this, inst.rhs()) << std::endl;
    }

    void operator()(ir::Load const& inst) const
    {
        std::cout << "  " << (*this)(inst.get_destination()) << " = load " << inst.get_name() << std::endl;
    }

    void operator()(ir::Return const& inst) const
    {
        std::cout << "  ret " << std::visit(*this, inst.value()) << std::endl;
    }

    void operator()(ir::Function const& fn) const
    {
        std::cout << "fn " << fn.name() << ":" << std::endl;
        for (auto const& inst : fn.body().get_instructions()) {
            std::visit(*this, inst);
        }
    }
};
