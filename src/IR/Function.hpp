#pragma once

#include "BasicBlock.hpp"

namespace ir {
class Function {
public:
    Function(std::string_view const name, std::vector<BasicBlock> const& basic_block)
        : name_(name)
        , body_(basic_block)
    {
    }

    std::string name() const
    {
        return name_;
    }

    std::vector<BasicBlock> body() const
    {
        return body_;
    }

private:
    std::string name_;
    std::vector<BasicBlock> body_;
};
}