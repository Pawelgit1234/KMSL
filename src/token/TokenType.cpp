﻿#include "TokenType.hpp"

namespace kmsl
{
    std::vector<std::pair<std::string, TokenType>> token_list = {
        {"(move|MOVE)\\b", TokenType::MOVE},
        {"(dmove|DMOVE)\\b", TokenType::DMOVE},
        {"(scroll|SCROLL)\\b", TokenType::SCROLL},
        {"(type|TYPE)\\b", TokenType::TYPE},
        {"(press|PRESS)\\b", TokenType::PRESS},
        {"(hold|HOLD)\\b", TokenType::HOLD},
        {"(release|RELEASE)\\b", TokenType::RELEASE},
        {"(state|STATE)\\b", TokenType::STATE},
        {"(wait|WAIT)\\b", TokenType::WAIT},
        {"(getx|GETX)\\b", TokenType::GETX},
        {"(gety|GETY)\\b", TokenType::GETY},
        {"(year|YEAR)\\b", TokenType::YEAR},
        {"(month|MONTH)\\b", TokenType::MONTH},
        {"(week|WEEK)\\b", TokenType::WEEK},
        {"(day|DAY)\\b", TokenType::DAY},
        {"(hour|HOUR)\\b", TokenType::HOUR},
        {"(minute|MINUTE)\\b", TokenType::MINUTE},
        {"(second|SECOND)\\b", TokenType::SECOND},
        {"(sin|SIN)\\b", TokenType::SIN},
        {"(cos|COS)\\b", TokenType::COS},
        {"(tan|TAN)\\b", TokenType::TAN},
        {"(acos|ACOS)\\b", TokenType::ACOS},
        {"(asin|ASIN)\\b", TokenType::ASIN},
        {"(atan|ATAN)\\b", TokenType::ATAN},
        {"(abs|ABS)\\b", TokenType::ABS},
        {"(rceil|RCEIL)\\b", TokenType::RCEIL},
        {"(rfloor|RFLOOR)\\b", TokenType::RFLOOR},
        {"(pi|PI)\\b", TokenType::PI},
        {"(e|E)\\b", TokenType::E},
        {"(phi|PHI)\\b", TokenType::PHI},
        {"(readfile|READFILE)\\b", TokenType::READFILE},
        {"(writefile|WRITEFILE)\\b", TokenType::WRITEFILE},
        {"(appendfile|APPENDFILE)\\b", TokenType::APPENDFILE},
        {"(createfile|CREATEFILE)\\b", TokenType::CREATEFILE},
        {"(remove|REMOVE)\\b", TokenType::REMOVE},
        {"(copy|COPY)\\b", TokenType::COPY},
        {"(rename|RENAME)\\b", TokenType::RENAME},
        {"(exists|EXISTS)\\b", TokenType::EXISTS},
        {"(createdir|CREATEDIR)\\b", TokenType::CREATEDIR},
        {"(milli|MILLI)\\b", TokenType::MILLI},
        {"(do|DO)\\b", TokenType::DO},
        {"(os|OS)\\b", TokenType::OS},
        {"(random|RANDOM)\\b", TokenType::RANDOM},
        {"!!", TokenType::EXIT},
        {"(\\n|\\r)", TokenType::LINE_END},
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
        {"(if|IF)\\b", TokenType::IF},
        {"(else|ELSE)\\b", TokenType::ELSE},
        {"(while|WHILE)\\b", TokenType::WHILE},
        {"(for|FOR)\\b", TokenType::FOR},
        {"(break|BREAK)\\b", TokenType::BREAK},
        {"(continue|CONTINUE)\\b", TokenType::CONTINUE},
        {"(print|PRINT)\\b", TokenType::PRINT},
        {"(input|INPUT)\\b", TokenType::INPUT},
        {"(true|false|TRUE|FALSE)\\b", TokenType::BOOL},
        {"[0-9]+\\.[0-9]+", TokenType::FLOAT},
        {"[0-9]+", TokenType::INT},
        {"(\"[^\"]*\"|'[^']*')", TokenType::STRING},
        {"[a-zA-Z_][a-zA-Z0-9_]*", TokenType::VARIABLE},
        {"\\(", TokenType::LPAR},
        {"\\)", TokenType::RPAR},
        {"\\{", TokenType::LBRACE},
        {"\\}", TokenType::RBRACE},
        {"\\s+", TokenType::SPACE},
        {"#.*(?=\\n|$)", TokenType::COMMENT}
    };
}
