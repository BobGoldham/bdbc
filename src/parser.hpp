#pragma once

#include <optional>
#include <variant>
#include <vector>

#include "tokenization.hpp"

namespace node
{
    struct ExprIntLit
    {
        Token value;
    };
    struct ExprIdent
    {
        Token ident;
    };
    struct Expr
    {
        std::variant<ExprIntLit, ExprIdent> var;
    };
    struct Exit
    {
        Expr expr;
    };
    struct VarVarDef
    {
        Token ident;
        Expr expr;
    };
    struct Stmt
    {
        std::variant<Exit, VarVarDef> var;
    };
    struct S
    {
        std::vector<Stmt> stmts;
        // std::optional<Exit> exit;
    };
}

class Parser
{
public:
    inline explicit Parser(std::vector<Token> tokens) : m_tokens(tokens)
    {
    }

    std::optional<node::Expr> parse_expr()
    {
        if (peek().has_value() && peek().value().type == TokenType::int_lit)
            return node::Expr{.var = node::ExprIntLit{.value = consume()}};

        if (peek().has_value() && peek().value().type == TokenType::ident)
            return node::Expr{.var = node::ExprIdent{.ident = consume()}};

        return {};
    }

    std::optional<node::Exit> parse_exit()
    {
        std::optional<node::Exit> nodeExit;

        consume();

        if (!peek().has_value() || !(peek().value().type == TokenType::space))
        {
            std::cerr << "Invalid exit syntax" << std::endl;
            exit(EXIT_FAILURE);
        }
        consume();

        if (auto node_expr = parse_expr())
        {
            nodeExit = node::Exit{.expr = node_expr.value()};
        }
        else
        {
            std::cerr << "Invalid exit code" << std::endl;
            exit(EXIT_FAILURE);
        }

        while (peek().has_value() && peek().value().type == TokenType::space)
            consume();

        if (!peek().has_value() || peek().value().type != TokenType::line_end)
        {
            std::cerr << "Invalid syntax! Try ending your lines." << std::endl;
            exit(EXIT_FAILURE);
        }
        consume(); // The line_end

        return nodeExit;
    }

    std::optional<node::VarVarDef> parse_var_var()
    {
        if (!peek(1).has_value() || peek(1).value().type != TokenType::space)
            return {};
        if (!peek(2).has_value() || peek(2).value().type != TokenType::ident)
            return {};
        size_t space_it = 3;
        while (peek(space_it).has_value() && peek(space_it).value().type == TokenType::space)
            space_it++;
        if (!peek(space_it).has_value() || peek(space_it).value().type != TokenType::eq)
            return {};
        while (peek(space_it + 1).has_value() && peek(space_it + 1).value().type == TokenType::space)
            space_it++;

        consume();
        consume();
        node::VarVarDef vvd{.ident = consume()};

        for (int i = 2; i < space_it; i++)
            consume();

        if (auto expr = parse_expr())
            vvd.expr = expr.value();
        else
        {
            std::cerr << "Invalid expression!";
            exit(EXIT_FAILURE);
        }

        if (peek().has_value() && peek().value().type == TokenType::line_end)
            consume();
        else
        {
            std::cerr << "Try ending your lines, duh..";
            exit(EXIT_FAILURE);
        }

        return vvd;
    }

    std::optional<node::Stmt> parse_stmt()
    {
        if (peek().has_value() && peek().value().type == TokenType::exit)
            return node::Stmt{.var = parse_exit().value()};
        if (peek().has_value() && peek().value().type == TokenType::varVar)
            return node::Stmt{.var = parse_var_var().value()};

        return {};
    }

    std::optional<node::S> parse()
    {
        node::S nodeS;

        while (peek().has_value())
        {
            while (peek().value().type == TokenType::space)
                consume();
            if (auto stmt = parse_stmt())
            {
                nodeS.stmts.push_back(stmt.value());
            }
            else
            {
                std::cerr << "Invalid statement!" << std::endl;
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