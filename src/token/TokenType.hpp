#pragma once

#include <unordered_map>
#include <string>

namespace kmsl
{
	enum class TokenType
	{
		/* Keywords */
		IF, ELIF, ELSE,
		WHILE, FOR, BREAK, CONTINUE,
		PRINT, INPUT,

		/* Operators */
		PLUS, MINUS, MULTIPLY, DIVIDE, FLOOR, MODULO, ROOT, LOG, POWER, PLUS_ONE, MINUS_ONE,
		BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, BIT_LEFT_SHIFT, BIT_RIGHT_SHIFT,
		LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT,
		EQUALS, NOT_EQUALS, LESS_THAN, GREATER_THAN, LESS_THAN_OR_EQUAL, GREATER_THAN_OR_EQUAL,
		ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN, MODULO_ASSIGN, FLOOR_ASSIGN, POWER_ASSIGN, ROOT_ASSIGN, LOG_ASSIGN, BIT_AND_ASSIGN, BIT_OR_ASSIGN, BIT_XOR_ASSIGN, BIT_LEFT_SHIFT_ASSIGN, BIT_RIGHT_SHIFT_ASSIGN,
        LPAR, RPAR,

		/* Var types */
        VARIABLE,
		BOOL,
		FLOAT,
		INT,
		STRING,

        /* Others */
        SPACE,
        LINE_END,
	};

    std::unordered_map<std::string, TokenType> regex = {
        {"if", TokenType::IF},
        {"elif", TokenType::ELIF},
        {"else", TokenType::ELSE},
        {"while", TokenType::WHILE},
        {"for", TokenType::FOR},
        {"break", TokenType::BREAK},
        {"continue", TokenType::CONTINUE},
        {"print", TokenType::PRINT},
        {"input", TokenType::INPUT},
        {"\\+", TokenType::PLUS},
        {"\\-", TokenType::MINUS},
        {"\\*", TokenType::MULTIPLY},
        {"/", TokenType::DIVIDE},
        {"//", TokenType::FLOOR},
        {"%", TokenType::MODULO},
        {"%%", TokenType::ROOT},
        {"\\^\\^", TokenType::LOG},
        {"\\*\\*", TokenType::POWER},
        {"\\+\\+", TokenType::PLUS_ONE},
        {"\\-\\-", TokenType::MINUS_ONE},
        {"&", TokenType::BIT_AND},
        {"\\|", TokenType::BIT_OR},
        {"\\^", TokenType::BIT_XOR},
        {"~", TokenType::BIT_NOT},
        {"<<", TokenType::BIT_LEFT_SHIFT},
        {">>", TokenType::BIT_RIGHT_SHIFT},
        {"&&", TokenType::LOGICAL_AND},
        {"\\|\\|", TokenType::LOGICAL_OR},
        {"!", TokenType::LOGICAL_NOT},
        {"==", TokenType::EQUALS},
        {"!=", TokenType::NOT_EQUALS},
        {"<", TokenType::LESS_THAN},
        {">", TokenType::GREATER_THAN},
        {"<=", TokenType::LESS_THAN_OR_EQUAL},
        {">=", TokenType::GREATER_THAN_OR_EQUAL},
        {"=", TokenType::ASSIGN},
        {"\\+=", TokenType::PLUS_ASSIGN},
        {"\\-=", TokenType::MINUS_ASSIGN},
        {"\\*=", TokenType::MULTIPLY_ASSIGN},
        {"/=", TokenType::DIVIDE_ASSIGN},
        {"%=", TokenType::MODULO_ASSIGN},
        {"//=", TokenType::FLOOR_ASSIGN},
        {"\\*\\*=", TokenType::POWER_ASSIGN},
        {"%%=", TokenType::ROOT_ASSIGN},
        {"\\^\\^=", TokenType::LOG_ASSIGN},
        {"&=", TokenType::BIT_AND_ASSIGN},
        {"\\|=", TokenType::BIT_OR_ASSIGN},
        {"\\^=", TokenType::BIT_XOR_ASSIGN},
        {"<<=", TokenType::BIT_LEFT_SHIFT_ASSIGN},
        {">>=", TokenType::BIT_RIGHT_SHIFT_ASSIGN},
        {"[a-zA-Z][a-zA-Z0-9]", TokenType::VARIABLE},
        {"true|false", TokenType::BOOL},
        {"\d+\.\d+", TokenType::FLOAT},
        {"\d+", TokenType::INT},
        {"\"[^\"]*\"", TokenType::STRING},
        {"\\(", TokenType::LPAR},
        {"\\)", TokenType::RPAR},
        {" \\t", TokenType::SPACE},
        {"\\n\\r", TokenType::LINE_END}
    };
}