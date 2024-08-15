﻿#include "Parser.hpp"
#include <iostream>
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

			if (codeStringNode)
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

	void Parser::removeTokensUntil(std::vector<TokenType> remove_types, std::vector<TokenType> stop_types)
	{
		auto it = std::find_if(tokens_.begin() + pos_, tokens_.end(), [&](const Token& token) {
			return std::find(stop_types.begin(), stop_types.end(), token.type) != stop_types.end();
			});

		int stop_index = std::distance(tokens_.begin(), it);

		for (int i = pos_; i < stop_index; i++)
			if (std::find(remove_types.begin(), remove_types.end(), tokens_[i].type) != remove_types.end())
				tokens_.erase(tokens_.begin() + i);
	}

	std::unique_ptr<AstNode> Parser::parseLine()
	{
		if (match({ TokenType::VARIABLE}).type != TokenType::INVALID)
		{
			std::unique_ptr<AstNode> varNode = parseVariable();
			return varNode;
		}
		else if (match({ TokenType::PLUS_ONE, TokenType::MINUS_ONE, TokenType::INPUT }).type != TokenType::INVALID)
		{
			Token oper = current_token_;
			std::unique_ptr<UnarOpNode> unarNode(std::make_unique<UnarOpNode>(oper, std::make_unique<VariableNode>(require({ TokenType::VARIABLE }))));
			return unarNode;
		}
		else if (match({ TokenType::PRINT }).type != TokenType::INVALID)
		{
			Token oper = current_token_;
			std::unique_ptr<UnarOpNode> unarNode(std::make_unique<UnarOpNode>(oper, parseExpression()));
			return unarNode;
		}
		else if (match({ TokenType::IF }).type != TokenType::INVALID)
		{
			std::unique_ptr<IfNode> ifNode = parseIf();
			return ifNode;
		}
		else if (match({ TokenType::FOR }).type != TokenType::INVALID)
		{
			std::unique_ptr<ForNode> forNode = parseFor();
			return forNode;
		}
		else if (match({ TokenType::WHILE }).type != TokenType::INVALID)
		{
			std::unique_ptr<WhileNode> whileNode = parseWhile();
			return whileNode;
		}
		else if (match({ TokenType::LINE_END }).type != TokenType::INVALID)
		{
			pos_--;
			return nullptr;
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
			std::unique_ptr<BinaryOpNode> binarNode(std::make_unique<BinaryOpNode>(assign, std::move(varNode), std::move(parseExpression())));
			return binarNode;
		}

		throw std::runtime_error("On positon" + std::to_string(pos_) + "expected an another value.");
	}

	std::unique_ptr<IfNode> Parser::parseIf()
	{
		require({ TokenType::LPAR });
		removeTokensUntil({ TokenType::LINE_END }, { TokenType::LBRACE });
		std::unique_ptr<AstNode> conditionNode = parseExpression();
		require({ TokenType::RPAR });
		require({ TokenType::LBRACE});

		std::unique_ptr<BlockNode> thenNode = std::make_unique<BlockNode>();
		while (match({ TokenType::RBRACE }).type == TokenType::INVALID)
		{
			std::unique_ptr<AstNode> codeStringNode = parseLine();
			require({ TokenType::LINE_END });
			
			if (codeStringNode)
				thenNode->addStatement(std::move(codeStringNode));
		}

		std::unique_ptr<BlockNode> elseNode = std::make_unique<BlockNode>();
		
		bool found = false;
		for (int i = pos_; i < tokens_.size(); i++)
		{
			if (tokens_[i].type == TokenType::ELSE)
			{
				found = true;
				break;
			}

			if (tokens_[i].type != TokenType::LINE_END)
				break;
		}

		if (found)
		{
			pos_ += 2;
			removeTokensUntil({ TokenType::LINE_END }, { TokenType::LBRACE });
			require({ TokenType::LBRACE });
			
			while (match({ TokenType::RBRACE }).type == TokenType::INVALID)
			{
				std::unique_ptr<AstNode> codeStringNode = parseLine();
				require({ TokenType::LINE_END });

				if (codeStringNode)
					elseNode->addStatement(std::move(codeStringNode));
			}
		}

		std::unique_ptr<IfNode> ifNode = std::make_unique<IfNode>(
			std::move(conditionNode),
			std::move(thenNode),
			std::move(elseNode)
		);
		return ifNode;
	}

	std::unique_ptr<ForNode> Parser::parseFor()
	{
		require({ TokenType::LPAR });
		removeTokensUntil({ TokenType::LINE_END }, { TokenType::LBRACE });

		require({ TokenType::VARIABLE });
		std::unique_ptr<AstNode> initializerNode = parseVariable();
		require({ TokenType::COMMA });
		std::unique_ptr<AstNode> conditionNode = parseExpression();
		require({ TokenType::COMMA });
		require({ TokenType::VARIABLE });
		std::unique_ptr<AstNode> incrementNode = parseVariable();

		require({ TokenType::RPAR });
		require({ TokenType::LBRACE });

		std::unique_ptr<BlockNode> bodyNode = std::make_unique<BlockNode>();

		while (match({ TokenType::RBRACE }).type == TokenType::INVALID)
		{
			std::unique_ptr<AstNode> codeStringNode = parseLine();
			require({ TokenType::LINE_END });

			if (codeStringNode)
				bodyNode->addStatement(std::move(codeStringNode));
		}

		std::unique_ptr<ForNode> forNode = std::make_unique<ForNode>(
			std::move(initializerNode),
			std::move(conditionNode),
			std::move(incrementNode),
			std::move(bodyNode)
		);
		return forNode;
	}

	std::unique_ptr<WhileNode> Parser::parseWhile()
	{
		require({ TokenType::LPAR });
		removeTokensUntil({ TokenType::LINE_END }, { TokenType::LBRACE });
		std::unique_ptr<AstNode> conditionNode = parseExpression();
		require({ TokenType::RPAR });
		require({ TokenType::LBRACE });

		std::unique_ptr<BlockNode> bodyNode = std::make_unique<BlockNode>();

		while (match({ TokenType::RBRACE }).type == TokenType::INVALID)
		{
			std::unique_ptr<AstNode> codeStringNode = parseLine();
			require({ TokenType::LINE_END });

			if (codeStringNode)
				bodyNode->addStatement(std::move(codeStringNode));
		}

		std::unique_ptr<WhileNode> whileNode = std::make_unique<WhileNode>(
			std::move(conditionNode),
			std::move(bodyNode)
		);

		return whileNode;
	}

	std::unique_ptr<AstNode> Parser::parseTerm()
	{
		std::unique_ptr<AstNode> node = parseFactor();

		while (match({ TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MODULO, TokenType::POWER, TokenType::FLOOR, TokenType::LOG, TokenType::ROOT, TokenType::BIT_AND, TokenType::BIT_OR, TokenType::BIT_XOR, TokenType::BIT_LEFT_SHIFT, TokenType::BIT_RIGHT_SHIFT }).type != TokenType::INVALID)
		{
			Token token = current_token_;
			node = std::make_unique<BinaryOpNode>(token, std::move(node), std::move(parseFactor()));
		}

		return node;
	}

	std::unique_ptr<AstNode> Parser::parseFactor()
	{
		if (match({ TokenType::PLUS, TokenType::MINUS, TokenType::LOGICAL_NOT, TokenType::BIT_NOT }).type != TokenType::INVALID)
		{
			Token oper = current_token_;
			std::unique_ptr<AstNode> node = parseFactor();
			return std::make_unique<UnarOpNode>(oper, std::move(node));
		}
		else if (match({ TokenType::LPAR }).type != TokenType::INVALID)
		{
			std::unique_ptr<AstNode> node = parseExpression();
			require({ TokenType::RPAR });
			return node;
		}
		else if (match({ TokenType::STRING, TokenType::INT, TokenType::FLOAT, TokenType::BOOL }).type != TokenType::INVALID)
			return std::make_unique<LiteralNode>(current_token_);
		else if (match({ TokenType::VARIABLE }).type != TokenType::INVALID)
			return std::make_unique<VariableNode>(current_token_);

		throw std::runtime_error("Incorrect Token");
	}

	std::unique_ptr<AstNode> Parser::parseExpression()
	{
		std::unique_ptr<AstNode> node = parseTerm();

		while (match({ TokenType::PLUS, TokenType::MINUS, TokenType::LOGICAL_AND, TokenType::LOGICAL_OR, TokenType::LESS_THAN, TokenType::GREATER_THAN, TokenType::LESS_THAN_OR_EQUAL, TokenType::GREATER_THAN_OR_EQUAL, TokenType::EQUALS, TokenType::NOT_EQUALS }).type != TokenType::INVALID)
		{
			Token token = current_token_;
			node = std::make_unique<BinaryOpNode>(token, std::move(node), std::move(parseTerm()));
		}

		return node;
	}
}