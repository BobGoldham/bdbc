#pragma once

#include <sstream>

#include "parser.hpp"

class Generator
{
public:
    inline explicit Generator(node::S root) : m_root(std::move(root))
    {
    }

    [[nodiscard]] std::string generate() const
    {
        std::stringstream output;
        output << "global _start\n_start:\n";

        output << "  mov rax, 60\n";
        output << "  mov rdi, " << m_root.m_exit.value().m_expr.m_int_lit.value().m_value.value.value() << "\n";
        output << "  syscall";

        return output.str();
    }

private:
    const node::S m_root;
};