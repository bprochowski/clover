#pragma once

#include <Ast/BinaryExpression.hpp>
#include <Ast/Function.hpp>
#include <Ast/Identifier.hpp>
#include <Ast/Integer.hpp>
#include <Ast/Return.hpp>

#include "Environment.hpp"
#include "Type.hpp"

#include <variant>

class TypeChecker {
public:
    void operator()(ast::Return const& node)
    {
        auto const type = this->operator()(node.value());
        if (type != return_type_) {
            report_error_("Returned value type is not matching the function return type");
        }
    }

    void operator()(StatementList const& node)
    {
        for (auto const& stmt : node) {
            std::visit(*this, stmt);
        }
    }

    void operator()(ast::Variable const& node)
    {
        auto const var = env_.lookup(node.name());
        if (var.has_value()) {
            report_error_("Variable redefinition");
            return;
        }

        auto declared_type = node.declared_type();
        if (declared_type.has_value()) {
            auto const t = env_.lookup(declared_type.value());
            if (!t.has_value() || t.value().kind != SymbolCtx::Kind::type) {
                report_error_("Variable of unknown type");
                return;
            }

            if (node.has_initializer()) {
                auto const type = this->operator()(node.initializer());
                if (type != t.value().type) {
                    report_error_("Initializer type is not matching the declared type");
                    return;
                }

                if (!env_.declare(node.name(), { SymbolCtx::Kind::variable, type })) {
                    report_error_("Redeclaration of variable");
                }
            } else {
                if (!env_.declare(node.name(), { SymbolCtx::Kind::variable, t.value().type })) {
                    report_error_("Redeclaration of variable");
                }
            }
        } else {
            if (!node.has_initializer()) {
                report_error_("Cennot doduce variable type");
            }

            auto const type = this->operator()(node.initializer());
            if (!env_.declare(node.name(), { SymbolCtx::Kind::variable, type })) {
                report_error_("Variable redeclaration");
            }
        }
    }

    void operator()(ast::If const& node)
    {
        this->operator()(node.condition());

        env_.open_scope();
        this->operator()(node.then_block());
        env_.close_scope();

        env_.open_scope();
        this->operator()(node.else_block());
        env_.close_scope();
    }

    void operator()(ast::While const& node)
    {
        this->operator()(node.condition());

        env_.open_scope();
        this->operator()(node.body());
        env_.close_scope();
    }

    void operator()(ast::Function const& node)
    {
        auto const t = env_.lookup(node.return_type());
        if (!t.has_value() || t.value().kind != SymbolCtx::Kind::type) {
            report_error_("Function of unknown return type");
            return;
        }
        return_type_ = t.value().type;

        env_.open_scope();
        this->operator()(node.body());
        env_.close_scope();
    }

    Type operator()(Expression const& node)
    {
        return std::visit(*this, node);
    }

    Type operator()(Identifier const& node)
    {
        auto type = env_.lookup(node.name());
        if (!type.has_value()) {
            report_error_("Undefined variable");
        }

        if (type.value().kind == SymbolCtx::Kind::type) {
            report_error_("used type not variable");
        }

        return type.value().type;
    }

    Type operator()(Integer const&)
    {
        return Type::integer;
    }

    Type operator()(BinaryExpression const& node)
    {
        auto const lhs_type = this->operator()(node.lhs());
        auto const rhs_type = this->operator()(node.rhs());

        switch (node.op()) {
        case BinaryExpression::Operator::plus:
        case BinaryExpression::Operator::minus:
        case BinaryExpression::Operator::slash:
        case BinaryExpression::Operator::asterisk: {
            if (lhs_type != Type::integer || rhs_type != Type::integer) {
                report_error_("Type mismatch");
            }
            return Type::integer;
        }
        }
    }

private:
    void report_error_(std::string_view const error)
    {
        std::cout << error << std::endl;
        exit(1);
    }
    Environment env_;
    std::optional<Type> return_type_;
};
