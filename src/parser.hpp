#pragma once

#include <optional>
#include <vector>

#include "tokenization.hpp"

namespace node
{
    struct IntLit
    {
        Token m_value;
    };
    struct Expr
    {
        std::optional<IntLit> m_int_lit;
    };
    struct Exit
    {
        Expr m_expr;
    };
    struct S
    {
        std::optional<Exit> m_exit;
    };
}

class Parser
{
public:
    inline explicit Parser(std::vector<Token> tokens) : m_tokens(tokens)
    {
    }

    std::optional<node::IntLit> parse_int_lit()
    {
        if (peek().has_value() && peek().value().type == TokenType::int_lit)
            return node::IntLit{.m_value = consume()};
        return {};
    }

    std::optional<node::Expr> parse_expr()
    {
        if (auto node_int_lit = parse_int_lit())
            return node::Expr{.m_int_lit = node_int_lit.value()};
        return {};
    }

    std::optional<node::Exit> parse_exit()
    {
        std::optional<node::Exit> nodeExit;

        if (auto node_expr = parse_expr())
        {
            nodeExit = node::Exit{.m_expr = node_expr.value()};
        }
        else
        {
            std::cerr << "Invalid exit code" << std::endl;
            exit(EXIT_FAILURE);
        }

        if (!peek().has_value() || peek().value().type != TokenType::line_end)
        {
            std::cerr << "Invalid syntax! Try ending your lines." << std::endl;
            exit(EXIT_FAILURE);
        }
        consume(); // The line_end

        return nodeExit;
    }

    std::optional<node::S> parse()
    {
        std::optional<node::S> nodeS;

        while (peek().has_value())
        {
            if (peek().value().type == TokenType::exit)
            {
                consume();
                if (auto node_exit = parse_exit())
                {
                    nodeS = node::S{.m_exit = node_exit.value()};
                }
            }
            else
            {
                std::cerr << "Invalid Syntax" << std::endl;
                exit(EXIT_FAILURE);
            }
        }

        m_index = 0;
        return nodeS;
    }

private:
    const std::vector<Token> m_tokens;
    size_t m_index = 0;

    [[nodiscard]] inline std::optional<Token> peek(int offset = 0) const
    {
        if (m_index + offset >= m_tokens.size())
            return {};
        else
            return m_tokens.at(m_index + offset);
    }

    inline Token consume()
    {
        return m_tokens.at(m_index++);
    }
};