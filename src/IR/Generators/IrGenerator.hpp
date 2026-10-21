#pragma once

#include <Ast/BinaryExpression.hpp>
#include <Ast/Function.hpp>
#include <Ast/Identifier.hpp>
#include <Ast/Return.hpp>

#include <IR/Define.hpp>
#include <IR/Function.hpp>
#include <IR/Store.hpp>

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
        current_bb_().add_instruction(ir::Return(this->operator()(node.value())));
    }

    void operator()(FunctionBody const& node)
    {
        for (auto const& stmt : node) {
            std::visit(*this, stmt);
        }
    }
    void operator()(ast::Variable const& node)
    {
        current_bb_().add_instruction(ir::Define { node.name() });
        if (node.has_initializer()) {
            auto value = this->operator()(node.initializer());
            current_bb_().add_instruction(ir::Store { node.name(), value });
        }
    }

    void operator()(ast::If const& node)
    {
        ++if_lable_id_;
        auto condition = this->operator()(node.condition());
        auto lable_id = std::to_string(if_lable_id_);
        auto then_lable = std::format("if.then.{}", lable_id);
        auto else_lable = std::format("if.else.{}", lable_id);
        auto end_lable = std::format("if.end.{}", lable_id);

        auto branching_end = node.else_block().size() > 0 ? else_lable : end_lable;
        current_bb_().add_instruction(ir::Branch { condition, then_lable, branching_end });

        init_bb_(then_lable);
        this->operator()(node.then_block());

        if (!current_bb_().is_terminated()) {
            current_bb_().add_instruction(ir::Jump(end_lable));
        }

        if (node.else_block().size() > 0) {
            init_bb_(else_lable);
            this->operator()(node.else_block());
            if (!current_bb_().is_terminated()) {
                current_bb_().add_instruction(ir::Jump(end_lable));
            }
        }

        init_bb_(end_lable);
    }

    void operator()(ast::While const& node)
    {
        ++while_lable_id_;
        auto lable_id = std::to_string(while_lable_id_);
        auto cond_lable = std::format("while.cond.{}", lable_id);
        auto body_lable = std::format("while.body.{}", lable_id);
        auto end_lable = std::format("while.end.{}", lable_id);

        current_bb_().add_instruction(ir::Jump { cond_lable });

        init_bb_(cond_lable);
        auto condition = this->operator()(node.condition());
        current_bb_().add_instruction(ir::Branch { condition, body_lable, end_lable });

        init_bb_(body_lable);
        this->operator()(node.body());

        if (!current_bb_().is_terminated()) {
            current_bb_().add_instruction(ir::Jump(cond_lable));
        }

        init_bb_(end_lable);
    }

    ir::Function operator()(ast::Function const& node)
    {
        init_bb_("fn_entry");
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
        current_bb_().add_instruction(ir::Load { dst, node.name() });
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
        current_bb_().add_instruction(ir::BinaryOperation { to_bin_op_kind(node.op()), dst, lhs, rhs });
        return dst;
    }

private:
    int virtual_var_id_ = -1;
    int if_lable_id_ = -1;
    int while_lable_id_ = -1;
    void bump_() { ++virtual_var_id_; }
    std::vector<ir::BasicBlock> basic_block_;
    int bb_index_ { -1 };
    void init_bb_(std::string_view const lable)
    {
        ++bb_index_;
        basic_block_.emplace_back(lable);
    }
    ir::BasicBlock& current_bb_() { return basic_block_[bb_index_]; }
};