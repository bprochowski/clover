#pragma once

#include "BasicBlock.hpp"

namespace ir {
class Function {
public:
    Function(std::string_view const name, BasicBlock const& basic_block)
        : name_(name)
        , body_(basic_block)
    {
    }

    std::string name() const
    {
        return name_;
    }

    BasicBlock body() const
    {
        return body_;
    }

private:
    std::string name_;
    BasicBlock body_;
};
}