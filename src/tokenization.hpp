#pragma once

#include <iostream>
#include <optional>
#include <string>
#include <vector>

enum class TokenType
{
    exit,
    int_lit,
    line_end
};

struct Token
{
    TokenType type;
    std::optional<std::string> value{};
};

class Tokenizer
{
public:
    inline explicit Tokenizer(std::string src) : m_src(std::move(src))
    {
    }

    inline std::vector<Token> tokenize()
    {
        std::vector<Token> tokens;

        std::string buf;

        while (peak().has_value())
        {
            if (std::isalpha(peak().value()))
            {
                buf.push_back(consume());
                while (peak().has_value() && std::isalnum(peak().value()))
                {
                    buf.push_back(consume());
                }

                if (buf == "exit")
                {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                }
                else
                {
                    std::cerr << "Invalid token encountered!\n"
                              << peak().value() << "\n"
                              << buf << std::endl;
                    exit(EXIT_FAILURE);
                }
                continue;
            }

            if (std::isdigit(peak().value()))
            {
                buf.push_back(consume());
                while (peak().has_value() && std::isdigit(peak().value()))
                {
                    buf.push_back(consume());
                }
                tokens.push_back({.type = TokenType::int_lit, .value = buf});
                buf.clear();
                continue;
            }

            if (peak().value() == '!' || peak().value() == '?')
            {
                tokens.push_back({.type = TokenType::line_end, .value = std::string{consume()}});
                continue;
            }

            if (std::isspace(peak().value()))
            {
                consume();
                continue;
            }

            std::cerr << "Invalid token encountered!\n"
                      << peak().value() << std::endl;
            exit(EXIT_FAILURE);
            continue;
        }

        m_index = 0;
        return tokens;
    }

private:
    const std::string m_src;
    size_t m_index = 0;

    [[nodiscard]] inline std::optional<char> peak(int offset = 0) const
    {
        if (m_index + offset >= m_src.size())
            return {};
        else
            return m_src.at(m_index + offset);
    }

    inline char consume()
    {
        return m_src.at(m_index++);
    }
};