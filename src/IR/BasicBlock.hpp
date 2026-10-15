#pragma once

#include <variant>
#include <vector>

#include "BinaryOperation.hpp"
#include "Branch.hpp"
#include "Define.hpp"
#include "Load.hpp"
#include "Return.hpp"
#include "Store.hpp"

namespace ir {
using Instruction = std::variant<BinaryOperation, Load, Return, Store, Define, Branch>;
class BasicBlock {
public:
    BasicBlock(std::string_view const lable)
        : lable_(lable)
    {
    }

    std::string get_lable() const
    {
        return lable_;
    }

    std::vector<Instruction> get_instructions() const
    {
        return instructions_;
    };

    void add_instruction(Instruction const& instruction)
    {
        instructions_.push_back(instruction);
    }

private:
    std::string lable_;
    std::vector<Instruction> instructions_;
};
}