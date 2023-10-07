#pragma once

#include <sstream>
#include <unordered_map>

#include "parser.hpp"

class Generator
{
public:
    inline explicit Generator(node::S root) : m_root(std::move(root))
    {
    }

    void generate_expr(const node::Expr &expr)
    {
        struct ExprVisitor
        {
            Generator *gen;

            void operator()(const node::ExprIntLit &expr_int_lit) const
            {
                gen->m_output << "  mov rax, " << expr_int_lit.value.value.value() << "\n";
                gen->push("rax");
            }
            void operator()(const node::ExprIdent &expr_ident)
            {
                if (!gen->m_vars.contains(expr_ident.ident.value.value()))
                {
                    std::cerr << "Unknown identifier: " << expr_ident.ident.value.value() << std::endl;
                    exit(EXIT_FAILURE);
                }

                const auto &var = gen->m_vars.at(expr_ident.ident.value.value());
                std::stringstream offset;
                offset << "QWORD [rsp + ";
                offset << (gen->m_stack_size - var.stack_loc - 1) * 8;
                offset << "]";
                gen->push(offset.str());
            }
        };

        ExprVisitor visitor{.gen = this};
        std::visit(visitor, expr.var);
    }

    void generate_stmt(const node::Stmt &stmt)
    {
        struct StmtVisitor
        {
            Generator *gen;

            void operator()(const node::Exit &stmt_exit) const
            {
                gen->generate_expr(stmt_exit.expr);

                gen->m_output << "  mov rax, 60\n";
                gen->pop("rdi");
                gen->m_output << "  syscall\n";
            }
            void operator()(const node::VarVarDef &var_var_def)
            {
                gen->m_vars.insert({var_var_def.ident.value.value(), Var{.stack_loc = gen->m_stack_size}});
                gen->generate_expr(var_var_def.expr);
            }
        };

        StmtVisitor visitor{.gen = this};
        std::visit(visitor, stmt.var);
    }

    [[nodiscard]] std::string generate()
    {
        m_output << "global _start\n_start:\n";

        for (const node::Stmt &stmt : m_root.stmts)
        {
            generate_stmt(stmt);
        }

        // Default exit code 0
        m_output << "  mov rax, 60\n";
        m_output << "  mov rdi, 0\n";
        m_output << "  syscall";

        return m_output.str();
    }

private:
    struct Var
    {
        size_t stack_loc;
    };

    const node::S m_root;
    std::stringstream m_output;
    size_t m_stack_size = 0;
    std::unordered_map<std::string, Var> m_vars{};

    void push(const std::string &reg)
    {
        m_output << "  push " << reg << "\n";
        m_stack_size++;
    }

    void pop(const std::string &reg)
    {
        m_output << "  pop " << reg << "\n";
        m_stack_size--;
    }
};