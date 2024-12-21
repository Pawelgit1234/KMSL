#include "Parser.hpp"

namespace kmsl
{
	Parser::Parser(std::vector<Token> tokens, ErrorHandler& error_handler)
		: tokens_(tokens), pos_(0), current_token_(tokens_[pos_]), error_handler_(error_handler) {}

	Parser::~Parser()
	{
		tokens_.clear();
	}

	std::unique_ptr<BlockNode> Parser::parse()
	{
		std::unique_ptr<BlockNode> root = std::make_unique<BlockNode>();

		if (!checkAllPars())
			return root;

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
		
		return Token(TokenType::INVALID, "", current_token_.pos);
	}

	Token Parser::require(std::vector<TokenType> types)
	{
		const Token token = match(types);
		if (token.type == TokenType::INVALID)
			error_handler_.report(ErrorType::SYNTAX_ERROR, "Token on wrong postiton", token.pos - 1);

		return token;
	}

	void Parser::removeTokensUntil(std::vector<TokenType> remove_types, std::vector<TokenType> stop_types)
	{
		auto it = std::find_if(tokens_.begin() + pos_, tokens_.end(), [&](const Token& token) {
			return std::find(stop_types.begin(), stop_types.end(), token.type) != stop_types.end();
			});

		int stop_index = std::distance(tokens_.begin(), it);
		
		if (stop_index == tokens_.size())
		{
			error_handler_.report(ErrorType::SYNTAX_ERROR, "Token on wrong postiton", current_token_.pos - 1);
			return;
		}

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
		else if (match({ TokenType::BREAK, TokenType::CONTINUE, TokenType::EXIT }).type != TokenType::INVALID)
		{
			std::unique_ptr<CommandNode> commandNode(std::make_unique<CommandNode>(current_token_.type));
			return commandNode;
		}
		else if (match({ TokenType::LINE_END }).type != TokenType::INVALID)
		{
			pos_--;
			return nullptr;
		}
		else if (match({ TokenType::MOVE, TokenType::DMOVE }).type != TokenType::INVALID)
		{
			std::unique_ptr<MouseNode> mouseNode = parseMouse();
			return mouseNode;
		}
		else if (match({ TokenType::TYPE, TokenType::SCROLL }).type != TokenType::INVALID)
		{
			std::unique_ptr<BinaryOpNode> typeNode = parseTypeAndScroll();
			return typeNode;
		}
		else if (match({ TokenType::HOLD, TokenType::RELEASE, TokenType::PRESS }).type != TokenType::INVALID)
		{
			TokenType type = current_token_.type;
			std::unique_ptr<KeyNode> keyNode(std::make_unique<KeyNode>(type, parseArguments()));
			return keyNode;
		}
		else if (match({ TokenType::WAIT, TokenType::OS, TokenType::DO,TokenType::CREATEFILE, TokenType::REMOVE, TokenType::CREATEDIR }).type != TokenType::INVALID)
		{
			Token oper = current_token_;
			std::unique_ptr<UnarOpNode> unarNode(std::make_unique<UnarOpNode>(oper, parseExpression()));
			return unarNode;
		}
		else if (match({ TokenType::WRITEFILE, TokenType::APPENDFILE, TokenType::COPY, TokenType::RENAME }).type != TokenType::INVALID) // Binar
		{
			std::unique_ptr<BinaryOpNode> filesystemNode = parseFileAndDir();
			return filesystemNode;
		}
		else if (match({ TokenType::LPAR, TokenType::STRING, TokenType::INT, TokenType::FLOAT, TokenType::BOOL, TokenType::YEAR, TokenType::MONTH, TokenType::WEEK, TokenType::DAY, TokenType::HOUR, TokenType::MINUTE, TokenType::SECOND, TokenType::MILLI, TokenType::SIN, TokenType::COS, TokenType::TAN, TokenType::ASIN, TokenType::ACOS, TokenType::ATAN, TokenType::ABS, TokenType::RCEIL, TokenType::RFLOOR, TokenType::PI, TokenType::E, TokenType::PHI, TokenType::READFILE, TokenType::EXISTS, TokenType::GETX, TokenType::GETY, TokenType::STATE, TokenType::RANDOM, TokenType::PLUS, TokenType::MINUS, TokenType::LOGICAL_NOT, TokenType::BIT_NOT }).type != TokenType::INVALID)
		{
			pos_--;
			std::unique_ptr<AstNode> expressionNode = parseExpression();
			return expressionNode;
		}

		if (pos_ < tokens_.size())
			error_handler_.report(ErrorType::SYNTAX_ERROR, "Expectet another value", tokens_[pos_].pos);
		else
			error_handler_.report(ErrorType::SYNTAX_ERROR, "Expectet another value", current_token_.pos);

		pos_++;
		return std::unique_ptr<AstNode>();
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
		else if (match({ TokenType::PLUS, TokenType::MINUS, TokenType::MULTIPLY, TokenType::DIVIDE, TokenType::MODULO, TokenType::FLOOR, TokenType::POWER, TokenType::ROOT, TokenType::LOG, TokenType::BIT_AND, TokenType::BIT_OR, TokenType::BIT_XOR, TokenType::BIT_LEFT_SHIFT, TokenType::BIT_RIGHT_SHIFT }).type != TokenType::INVALID)
		{
			pos_ -= 2; // a + b: back to a
			return std::move(parseExpression());
		}
		else
			return varNode;
	}

	std::unique_ptr<MouseNode> Parser::parseMouse()
	{
		TokenType type = current_token_.type;
		std::unique_ptr<AstNode> x = parseExpression();
		require({ TokenType::COMMA });
		std::unique_ptr<AstNode> y = parseExpression();
		std::unique_ptr<AstNode> t;

		if (match({ TokenType::COMMA }).type != TokenType::INVALID)
			t = parseExpression();

		std::unique_ptr<MouseNode> mouseNode = std::make_unique<MouseNode>(
			type,
			std::move(x),
			std::move(y),
			std::move(t)
		);

		return mouseNode;
	}

	std::unique_ptr<BinaryOpNode> Parser::parseTypeAndScroll()
	{

		Token token = current_token_;
		std::unique_ptr<AstNode> left = parseExpression();
		std::unique_ptr<AstNode> t;

		if (match({ TokenType::COMMA }).type != TokenType::INVALID)
			t = parseExpression();


		std::unique_ptr<BinaryOpNode> typeNode = std::make_unique<BinaryOpNode>(
			token,
			std::move(left),
			std::move(t)
		);

		return typeNode;
	}

	std::unique_ptr<BinaryOpNode> Parser::parseFileAndDir()
	{
		Token token = current_token_;
		std::unique_ptr<AstNode> left = parseExpression();
		require({ TokenType::COMMA });
		std::unique_ptr<AstNode> right = parseExpression();

		std::unique_ptr<BinaryOpNode> filesystemNode = std::make_unique<BinaryOpNode>(
			token,
			std::move(left),
			std::move(right)
		);

		return filesystemNode;
	}

	std::vector<std::unique_ptr<AstNode>> Parser::parseArguments()
	{
		std::vector<std::unique_ptr<AstNode>> arguments;

		do {
			arguments.push_back(parseExpression());
		} while (match({ TokenType::COMMA }).type != TokenType::INVALID);

		return arguments;
	}

	std::unique_ptr<IfNode> Parser::parseIf()
	{
		require({ TokenType::LPAR });
		Token posToken = current_token_;
		removeTokensUntil({ TokenType::LINE_END }, { TokenType::LBRACE });
		std::unique_ptr<AstNode> conditionNode = parseExpression();
		require({ TokenType::RPAR });

		if (require({ TokenType::LBRACE }).type == TokenType::INVALID)
		{
			error_handler_.report(ErrorType::SYNTAX_ERROR, "'{}' were forgotten", current_token_.pos - 1);

			return std::make_unique<IfNode>(
				std::move(std::unique_ptr<AstNode>()),
				std::move(std::unique_ptr<AstNode>()),
				std::move(std::unique_ptr<AstNode>()),
				current_token_
			);
		}

		std::unique_ptr<BlockNode> thenNode = std::make_unique<BlockNode>();
		while (match({ TokenType::RBRACE }).type == TokenType::INVALID)
		{
			std::unique_ptr<AstNode> codeStringNode = parseLine();
			require({ TokenType::LINE_END });
			
			if (codeStringNode)
				thenNode->addStatement(std::move(codeStringNode));
		}

		std::unique_ptr<BlockNode> elseNode = std::make_unique<BlockNode>();
		
		auto parseElseBlock = [&]()
		{
			removeTokensUntil({ TokenType::LINE_END }, { TokenType::LBRACE });

			if (require({ TokenType::LBRACE }).type == TokenType::INVALID)
			{
				error_handler_.report(ErrorType::SYNTAX_ERROR, "'{}' were forgotten", current_token_.pos - 1);
				return;
			}

			while (match({ TokenType::RBRACE }).type == TokenType::INVALID)
			{
				std::unique_ptr<AstNode> codeStringNode = parseLine();
				require({ TokenType::LINE_END });

				if (codeStringNode)
					elseNode->addStatement(std::move(codeStringNode));
			}
		};

		if (match({ TokenType::ELSE }).type != TokenType::INVALID)
			parseElseBlock();
		else
		{
			pos_++;
			if (match({ TokenType::ELSE }).type != TokenType::INVALID)
				parseElseBlock();
			else
				pos_--;
		}

		std::unique_ptr<IfNode> ifNode = std::make_unique<IfNode>(
			std::move(conditionNode),
			std::move(thenNode),
			std::move(elseNode),
			current_token_
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
		Token posToken = current_token_;
		std::unique_ptr<AstNode> conditionNode = parseExpression();
		require({ TokenType::COMMA });
		require({ TokenType::VARIABLE });
		std::unique_ptr<AstNode> incrementNode = parseVariable();

		require({ TokenType::RPAR });

		if (require({ TokenType::LBRACE }).type == TokenType::INVALID)
		{
			error_handler_.report(ErrorType::SYNTAX_ERROR, "'{}' were forgotten", current_token_.pos - 1);
	
			return std::make_unique<ForNode>(
				std::move(std::unique_ptr<AstNode>()),
				std::move(std::unique_ptr<AstNode>()),
				std::move(std::unique_ptr<AstNode>()),
				std::move(std::unique_ptr<AstNode>()),
				current_token_
			);
		}

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
			std::move(bodyNode),
			posToken
		);
		return forNode;
	}

	std::unique_ptr<WhileNode> Parser::parseWhile()
	{
		require({ TokenType::LPAR });
		Token posToken = current_token_;
		removeTokensUntil({ TokenType::LINE_END }, { TokenType::LBRACE });
		std::unique_ptr<AstNode> conditionNode = parseExpression();
		require({ TokenType::RPAR });
		
		if (require({ TokenType::LBRACE }).type == TokenType::INVALID)
		{
			error_handler_.report(ErrorType::SYNTAX_ERROR, "'{}' were forgotten", current_token_.pos - 1);

			return std::make_unique<WhileNode>(
				std::move(std::unique_ptr<AstNode>()),
				std::move(std::unique_ptr<AstNode>()),
				current_token_
			);
		}

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
			std::move(bodyNode),
			posToken
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
		if (match({ TokenType::PLUS, TokenType::MINUS, TokenType::LOGICAL_NOT, TokenType::BIT_NOT, TokenType::SIN, TokenType::COS, TokenType::TAN, TokenType::ACOS, TokenType::ASIN, TokenType::ATAN, TokenType::ABS, TokenType::RCEIL, TokenType::RFLOOR }).type != TokenType::INVALID)
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
		else if (match({ TokenType::VARIABLE, TokenType::GETX, TokenType::GETY, TokenType::RANDOM, TokenType::PI, TokenType::E, TokenType::PHI, TokenType::YEAR, TokenType::MONTH, TokenType::WEEK, TokenType::DAY, TokenType::HOUR, TokenType::MINUTE, TokenType::SECOND, TokenType::MILLI }).type != TokenType::INVALID)
			return std::make_unique<VariableNode>(current_token_);
		else if (match({ TokenType::STATE, TokenType::READFILE,  TokenType::EXISTS, }).type != TokenType::INVALID)
		{
			Token stateToken = current_token_;
			require({ TokenType::STRING });
			return std::make_unique<UnarOpNode>(stateToken, std::make_unique<LiteralNode>(current_token_));
		}

		error_handler_.report(ErrorType::SYNTAX_ERROR, "Expectet another value", current_token_.pos);
		return std::unique_ptr<AstNode>();
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

	bool Parser::checkAllPars()
	{
		std::vector<Token> pars; // pars and braces 

		for (const auto& t : tokens_)
		{
			if (t.type == TokenType::LPAR || t.type == TokenType::LBRACE)
				pars.push_back(t);
			else if (pars.size() != 0)
				if ((t.type == TokenType::RPAR && pars.back().type == TokenType::LPAR) ||
					(t.type == TokenType::RBRACE && pars.back().type == TokenType::LBRACE))
					pars.pop_back();
				else if ((t.type == TokenType::RPAR && pars.back().type == TokenType::LBRACE) ||
						 (t.type == TokenType::RBRACE && pars.back().type == TokenType::LPAR))
				{
					error_handler_.report(ErrorType::SYNTAX_ERROR, "'" + t.text + "' on wrong position", t.pos);
					break;
				}
		}

		for (const auto& p : pars)
			error_handler_.report(ErrorType::SYNTAX_ERROR, "'" + p.text + "' is not closed", p.pos);

		return pars.size() == 0;
	}
}