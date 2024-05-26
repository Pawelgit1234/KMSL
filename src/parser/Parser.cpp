#include "Parser.hpp"

namespace kmsl
{
	Parser::Parser(std::vector<Token> tokens)
		: tokens_(tokens), pos_(0), current_token_(TokenType::INVALID, "", 0) {}

	Parser::~Parser()
	{
		tokens_.clear();
	}

	std::unique_ptr<AstNode> Parser::parse()
	{
		std::unique_ptr<BlockNode> root;

		while (pos_ < tokens_.size())
		{
			std::unique_ptr<AstNode> codeStringNode = parseExpression();
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
				pos_++;
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

	std::unique_ptr<AstNode> Parser::parseExpression()
	{
		if (match({ TokenType::VARIABLE}).type != TokenType::INVALID)
		{
			std::unique_ptr<AstNode> varNode = parseVariable();
			return varNode;
		}
		else if (match({ TokenType::PLUS_ONE, TokenType::MINUS_ONE, TokenType::PRINT, TokenType::INPUT }).type != TokenType::INVALID) // Unar
		{
			std::unique_ptr<AstNode> unarNode = parseUnar();
			return unarNode;
		}
		else if (match({ TokenType::IF }).type != TokenType::INVALID)
		{
			std::unique_ptr<AstNode> ifNode = parseIf();
			return ifNode;
		}
		else if (match({ TokenType::FOR }).type != TokenType::INVALID)
		{
			std::unique_ptr<AstNode> forNode = parseFor();
			return forNode;
		}
		else if (match({ TokenType::WHILE }).type != TokenType::INVALID)
		{
			std::unique_ptr<AstNode> whileNode = parseWhile();
			return whileNode;
		}
		
		throw std::runtime_error("On positon" + std::to_string(pos_) + "expected an another value.");
	}

	std::unique_ptr<UnarOpNode> Parser::parseUnar()
	{
		return std::unique_ptr<UnarOpNode>();
	}

	std::unique_ptr<AstNode> Parser::parseVariable()
	{
		std::unique_ptr<VariableNode> varNode(std::make_unique<VariableNode>(current_token_));
		if (match({ TokenType::PLUS_ONE, TokenType::MINUS_ONE }).type != TokenType::INVALID)
		{
			std::unique_ptr<UnarOpNode> unarNode(std::make_unique<UnarOpNode>(current_token_, varNode));
			return unarNode;
		}
		else if (match({ TokenType::ASSIGN, TokenType::PLUS_ASSIGN, TokenType::MINUS_ASSIGN, TokenType::MULTIPLY_ASSIGN, TokenType::DIVIDE_ASSIGN, TokenType::MODULO_ASSIGN, TokenType::FLOOR_ASSIGN, TokenType::POWER_ASSIGN, TokenType::ROOT_ASSIGN, TokenType::LOG_ASSIGN, TokenType::BIT_AND_ASSIGN, TokenType::BIT_OR_ASSIGN, TokenType::BIT_XOR_ASSIGN, TokenType::BIT_LEFT_SHIFT_ASSIGN, TokenType::BIT_RIGHT_SHIFT_ASSIGN }).type != TokenType::INVALID)
		{
			std::unique_ptr<BinaryOpNode> binarNode(std::make_unique<BinaryOpNode>(current_token_, varNode, parseTerm()));
			return binarNode;
		}

		throw std::runtime_error("On positon" + std::to_string(pos_) + "expected an another value.");
	}
	std::unique_ptr<AstNode> Parser::parseTerm()
	{
		return std::unique_ptr<AstNode>();
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
}