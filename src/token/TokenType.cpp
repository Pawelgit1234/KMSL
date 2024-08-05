#include "TokenType.hpp"

namespace kmsl
{
    std::vector<std::pair<std::string, TokenType>> token_list = {
        {"\\n|\\r", TokenType::LINE_END},
        {"\\^\\^=", TokenType::LOG_ASSIGN},
        {"\\*\\*=", TokenType::POWER_ASSIGN},
        {"\\+\\+=", TokenType::PLUS_ASSIGN},
        {"\\-\\-=", TokenType::MINUS_ASSIGN},
        {"%%=", TokenType::ROOT_ASSIGN},
        {"//=", TokenType::FLOOR_ASSIGN},
        {"<<=", TokenType::BIT_LEFT_SHIFT_ASSIGN},
        {">>=", TokenType::BIT_RIGHT_SHIFT_ASSIGN},
        {"\\+=", TokenType::PLUS_ASSIGN},
        {"\\-=", TokenType::MINUS_ASSIGN},
        {"\\*=", TokenType::MULTIPLY_ASSIGN},
        {"/=", TokenType::DIVIDE_ASSIGN},
        {"%=", TokenType::MODULO_ASSIGN},
        {"&=", TokenType::BIT_AND_ASSIGN},
        {"\\|=", TokenType::BIT_OR_ASSIGN},
        {"\\^=", TokenType::BIT_XOR_ASSIGN},
        {"\\^\\^", TokenType::LOG},
        {"\\*\\*", TokenType::POWER},
        {"\\+\\+", TokenType::PLUS_ONE},
        {"\\-\\-", TokenType::MINUS_ONE},
        {"%%", TokenType::ROOT},
        {"//", TokenType::FLOOR},
        {"<<", TokenType::BIT_LEFT_SHIFT},
        {">>", TokenType::BIT_RIGHT_SHIFT},
        {"&&", TokenType::LOGICAL_AND},
        {"\\|\\|", TokenType::LOGICAL_OR},
        {"==", TokenType::EQUALS},
        {"!=", TokenType::NOT_EQUALS},
        {"<=", TokenType::LESS_THAN_OR_EQUAL},
        {">=", TokenType::GREATER_THAN_OR_EQUAL},
        {",", TokenType::COMMA},
        {"\\+", TokenType::PLUS},
        {"\\-", TokenType::MINUS},
        {"\\*", TokenType::MULTIPLY},
        {"/", TokenType::DIVIDE},
        {"%", TokenType::MODULO},
        {"&", TokenType::BIT_AND},
        {"\\|", TokenType::BIT_OR},
        {"\\^", TokenType::BIT_XOR},
        {"~", TokenType::BIT_NOT},
        {"!", TokenType::LOGICAL_NOT},
        {"<", TokenType::LESS_THAN},
        {">", TokenType::GREATER_THAN},
        {"=", TokenType::ASSIGN},
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE},
        {"print", TokenType::PRINT},
        {"input", TokenType::INPUT},
        {"[a-zA-Z][a-zA-Z0-9]*", TokenType::VARIABLE},
        {"true|false", TokenType::BOOL},
        {"[0-9]+\\.[0-9]+", TokenType::FLOAT},
        {"[0-9]+", TokenType::INT},
        {"\"[^\"]*\"", TokenType::STRING},
        {"\\(", TokenType::LPAR},
        {"\\)", TokenType::RPAR},
        {"\\{", TokenType::LBRACE},
        {"\\}", TokenType::RBRACE},
        {"\\s+", TokenType::SPACE}
    };
}
