#include "Parser.hpp"

namespace kmsl
{
	Parser::Parser(std::vector<Token> tokens)
		: tokens_(tokens), pos_(0), current_token_(tokens_[pos_]) {}

	Parser::~Parser()
	{
		tokens_.clear();
	}

	std::unique_ptr<BlockNode> Parser::parse()
	{
		std::unique_ptr<BlockNode> root = std::make_unique<BlockNode>();

		while (pos_ < tokens_.size())
		{
			std::unique_ptr<AstNode> codeStringNode = parseLine();
			require({ TokenType::LINE_END });
			root->addStatement(std::move(codeStringNode));
		}
		return root;
	}

	Token Parser::match(std::vector<TokenType> types)
	{
		if (pos_ < tokens_.size())
		{
			const Token currentToken = tokens_[pos_];
			auto it = std::find(types.begin(), types.end(), currentToken.type);
			if (it != types.end())
			{
				pos_++; // next token if the previous one is right
				current_token_ = currentToken;
				return currentToken;
			}
		}
		
		return Token(TokenType::INVALID, "", pos_);
	}

	Token Parser::require(std::vector<TokenType> types)
	{
		const Token token = match(types);
		if (token.type == TokenType::INVALID)
			throw std::runtime_error("Error on pos " + std::to_string(pos_));

		return token;
	}

	std::unique_ptr<AstNode> Parser::parseLine()
	{
		if (match({ TokenType::VARIABLE}).type != TokenType::INVALID)
		{
			std::unique_ptr<AstNode> varNode = parseVariable();
			return varNode;
		}
		else if (match({ TokenType::PLUS_ONE, TokenType::MINUS_ONE }).type != TokenType::INVALID)
		{
			Token oper = current_token_;
			std::unique_ptr<UnarOpNode> unarNode(std::make_unique<UnarOpNode>(oper, std::make_unique<VariableNode>(require({ TokenType::VARIABLE }))));
			return unarNode;
		}
		else if (match({ TokenType::PRINT, TokenType::INPUT }).type != TokenType::INVALID)
		{
			
		}
		else if (match({ TokenType::IF }).type != TokenType::INVALID)
		{
			
		}
		else if (match({ TokenType::FOR }).type != TokenType::INVALID)
		{
			
		}
		else if (match({ TokenType::WHILE }).type != TokenType::INVALID)
		{
			
		}
		
		throw std::runtime_error("On positon" + std::to_string(pos_) + "expected an another value.");
	}

	std::unique_ptr<AstNode> Parser::parseVariable()
	{
		std::unique_ptr<VariableNode> varNode(std::make_unique<VariableNode>(current_token_));
		if (match({ TokenType::PLUS_ONE, TokenType::MINUS_ONE }).type != TokenType::INVALID)
		{
			std::unique_ptr<UnarOpNode> unarNode(std::make_unique<UnarOpNode>(current_token_, std::move(varNode)));
			return unarNode;
		}
		else if (match({ TokenType::ASSIGN, TokenType::PLUS_ASSIGN, TokenType::MINUS_ASSIGN, TokenType::MULTIPLY_ASSIGN, TokenType::DIVIDE_ASSIGN, TokenType::MODULO_ASSIGN, TokenType::FLOOR_ASSIGN, TokenType::POWER_ASSIGN, TokenType::ROOT_ASSIGN, TokenType::LOG_ASSIGN, TokenType::BIT_AND_ASSIGN, TokenType::BIT_OR_ASSIGN, TokenType::BIT_XOR_ASSIGN, TokenType::BIT_LEFT_SHIFT_ASSIGN, TokenType::BIT_RIGHT_SHIFT_ASSIGN }).type != TokenType::INVALID)
		{
			Token assign = current_token_;
			std::unique_ptr<BinaryOpNode> binarNode(std::make_unique<BinaryOpNode>(assign, std::move(varNode), std::move(parseTerm())));
			return binarNode;
		}

		throw std::runtime_error("On positon" + std::to_string(pos_) + "expected an another value.");
	}

	std::unique_ptr<IfNode> Parser::parseIf()
	{
		return std::unique_ptr<IfNode>();
	}
	std::unique_ptr<ForNode> Parser::parseFor()
	{
		return std::unique_ptr<ForNode>();
	}
	std::unique_ptr<WhileNode> Parser::parseWhile()
	{
		return std::unique_ptr<WhileNode>();
	}

	std::unique_ptr<AstNode> Parser::parseTerm()
	{
		return std::unique_ptr<AstNode>();
	}
}