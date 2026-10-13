#pragma once

#include <Ast/BinaryExpression.hpp>
#include <Ast/Function.hpp>
#include <Ast/Identifier.hpp>
#include <Ast/Return.hpp>

#include <IR/Function.hpp>

#include <iostream>

ir::BinaryOperation::Kind to_bin_op_kind(BinaryExpression::Operator op)
{
    switch (op) {
    case BinaryExpression::Operator::plus:
        return ir::BinaryOperation::Kind::add;
    case BinaryExpression::Operator::minus:
        return ir::BinaryOperation::Kind::sub;
    case BinaryExpression::Operator::slash:
        return ir::BinaryOperation::Kind::div;
    case BinaryExpression::Operator::asterisk:
        return ir::BinaryOperation::Kind::mul;
    }
}

class IrGenerator {
public:
    void operator()(ast::Return const& node)
    {
        basic_block_.add_instruction(ir::Return(this->operator()(node.value())));
    }

    ir::Function operator()(ast::Function const& node)
    {
        this->operator()(node.body());
        return ir::Function(node.name(), basic_block_);
    }

    ir::Value operator()(Expression const& node)
    {
        return std::visit(*this, node);
    }

    ir::Value operator()(Identifier const& node)
    {
        this->bump_();
        ir::VirtualVar dst { std::to_string(this->virtual_var_id_) };
        basic_block_.add_instruction(ir::Load { dst, node.name() });
        return dst;
    }

    ir::Value operator()(Integer const& node)
    {
        return ir::Constant { node.value() };
    }

    ir::Value operator()(BinaryExpression const& node)
    {
        auto lhs = this->operator()(node.lhs());
        auto rhs = this->operator()(node.rhs());
        this->bump_();
        ir::VirtualVar dst { std::to_string(this->virtual_var_id_) };
        basic_block_.add_instruction(ir::BinaryOperation { to_bin_op_kind(node.op()), dst, lhs, rhs });
        return dst;
    }

private:
    int virtual_var_id_ = -1;
    void bump_() { ++virtual_var_id_; }
    ir::BasicBlock basic_block_;
};