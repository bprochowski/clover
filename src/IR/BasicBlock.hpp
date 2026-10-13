#pragma once

#include <variant>
#include <vector>

#include "BinaryOperation.hpp"
#include "Load.hpp"
#include "Return.hpp"

namespace ir {
using Instruction = std::variant<BinaryOperation, Load, Return>;
class BasicBlock {
public:
    std::vector<Instruction> get_instructions() const
    {
        return instructions_;
    };

    void add_instruction(Instruction const& instruction)
    {
        instructions_.push_back(instruction);
    }

private:
    std::vector<Instruction> instructions_;
};
}