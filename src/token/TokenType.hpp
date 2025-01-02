#pragma once

#include <vector>
#include <string>

namespace kmsl
{
	enum class TokenType
	{
		/* Constructions */
		IF, ELSE,
		WHILE, FOR, BREAK, CONTINUE,

		/* Basic Functions */
		WAIT, EXIT, RANDOM, OS, DO, PRINT, INPUT,

		/* Extra */
		YEAR, MONTH, WEEK, DAY, HOUR, MINUTE, SECOND, MILLI, // time
		SIN, COS, TAN, ACOS, ASIN, ATAN, ABS, RCEIL, RFLOOR, PI, E, PHI,  // complicated math operations and constans
		READFILE, WRITEFILE, APPENDFILE, CREATEFILE, // file operations
		CREATEDIR, // dir operations
		REMOVE, COPY, RENAME, EXISTS, // dir and file operations

		/* Mouse & Keyboard */
		MOVE, DMOVE, SCROLL, TYPE, PRESS, HOLD, RELEASE, GETX, GETY, STATE,
		
		/* Operators */
		PLUS, MINUS, MULTIPLY, DIVIDE, FLOOR, MODULO, ROOT, LOG, POWER, PLUS_ONE, MINUS_ONE,
		BIT_AND, BIT_OR, BIT_XOR, BIT_NOT, BIT_LEFT_SHIFT, BIT_RIGHT_SHIFT,
		LOGICAL_AND, LOGICAL_OR, LOGICAL_NOT,
		EQUALS, NOT_EQUALS, LESS_THAN, GREATER_THAN, LESS_THAN_OR_EQUAL, GREATER_THAN_OR_EQUAL,
		ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, MULTIPLY_ASSIGN, DIVIDE_ASSIGN, MODULO_ASSIGN, FLOOR_ASSIGN, POWER_ASSIGN, ROOT_ASSIGN, LOG_ASSIGN, BIT_AND_ASSIGN, BIT_OR_ASSIGN, BIT_XOR_ASSIGN, BIT_LEFT_SHIFT_ASSIGN, BIT_RIGHT_SHIFT_ASSIGN,
        LPAR, RPAR, LBRACE, RBRACE,

		/* Var types */
        VARIABLE,
		BOOL,
		FLOAT,
		INT,
		STRING,

        /* Others */
        SPACE, LINE_END, COMMENT,
		INVALID,
		COMMA,
	};

	extern std::vector<std::pair<std::string, TokenType>> token_list;
}