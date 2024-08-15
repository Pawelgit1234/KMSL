﻿#include "Lexer.hpp"

namespace kmsl
{
    Lexer::Lexer(const std::string& code)
        : pos_(0)
    {
        std::string modified_code; // I do not know how to fix this f-bull-s, so I just 
        for (char ch : code)       // add these piece of code
        {
            modified_code += ch;
            if (ch == '\n')
                modified_code += ' ';
        }
        code_ = modified_code + '\n';
    }

    Lexer::~Lexer()
    {
        code_.clear();
        tokens_.clear();
    }

    std::vector<Token> Lexer::scanTokens()
    {
        while (pos_ < code_.length())
            scanToken();

        return tokens_;
    }

    void Lexer::scanToken()
    {
        if (pos_ >= code_.length()) return;

        std::string code = code_.substr(pos_);
        for (const auto& token_type : token_list)
        {
            std::regex pattern("^" + token_type.first);
            std::smatch match;

            if (std::regex_search(code, match, pattern))
            {
                std::string matched_str = match.str(0);
                pos_ += matched_str.length();
                Token token(token_type.second, matched_str, pos_);

                if (token.type == TokenType::COMMENT)
                    tokens_.push_back(Token(TokenType::LINE_END, "\n", pos_));
                else if (token.type != TokenType::SPACE)
                {
                    if (token.type == TokenType::STRING) // removing the "" from the string
                        token.text = token.text.substr(1, token.text.length() - 2);
                    tokens_.push_back(token);
                }
                return;
            }
        }
        pos_++;
    }
}
