#include "SemanticAnalyzer.hpp"

namespace kmsl
{
	SemanticAnalyzer::SemanticAnalyzer(std::unique_ptr<BlockNode>& root, ErrorHandler& error_handler) : symbol_table_(), root_(root),
		inside_loop_(false), in_console_(false), symbols_(nullptr), deepness_(0), error_handler_(error_handler) {}

	void SemanticAnalyzer::analyze()
	{
		visit(root_.get());
	}

	void SemanticAnalyzer::visitNode(AstNode* node)
	{
		if (auto blockNode = dynamic_cast<BlockNode*>(node))
			visit(blockNode);
		else if (auto variableNode = dynamic_cast<VariableNode*>(node))
			visit(variableNode);
		else if (auto unarOpNode = dynamic_cast<UnarOpNode*>(node))
			visit(unarOpNode);
		else if (auto binaryOpNode = dynamic_cast<BinarOpNode*>(node))
			visit(binaryOpNode);
		else if (auto ifNode = dynamic_cast<IfNode*>(node))
			visit(ifNode);
		else if (auto forNode = dynamic_cast<ForNode*>(node))
			visit(forNode);
		else if (auto whileNode = dynamic_cast<WhileNode*>(node))
			visit(whileNode);
		else if (auto keyNode = dynamic_cast<KeyNode*>(node))
			visit(keyNode);
		else if (auto mouseNode = dynamic_cast<MouseNode*>(node))
			visit(mouseNode);
		else if (auto commandNode = dynamic_cast<CommandNode*>(node))
			visit(commandNode);
	}

	void SemanticAnalyzer::visit(BlockNode* node)
	{
		symbol_table_.enterScope();
		deepness_++;
		for (auto& stmt : node->getStatements())
			visitNode(stmt.get());

		symbol_table_.exitScope();

		if (in_console_)
			symbols_->erase(
				std::remove_if(symbols_->begin(), symbols_->end(),
					[&](const Symbol& sym) { return sym.deepness > deepness_; }),
				symbols_->end());

		deepness_--;
	}

	void SemanticAnalyzer::visit(VariableNode* node)
	{
		if (node->token.type != TokenType::GETX &&
			node->token.type != TokenType::GETY &&
			node->token.type != TokenType::RANDOM &&
			node->token.type != TokenType::PI &&
			node->token.type != TokenType::E &&
			node->token.type != TokenType::PHI &&
			node->token.type != TokenType::YEAR &&
			node->token.type != TokenType::MONTH &&
			node->token.type != TokenType::WEEK &&
			node->token.type != TokenType::DAY &&
			node->token.type != TokenType::HOUR &&
			node->token.type != TokenType::MINUTE &&
			node->token.type != TokenType::SECOND &&
			node->token.type != TokenType::MILLI)
		{
			if (in_console_)
			{
				auto it = std::find_if(symbols_->begin(), symbols_->end(),
					[&](const Symbol& symbol) { return symbol.name == node->token.text; });

				if (it == symbols_->end())
					error_handler_.report(ErrorType::SEMANTIC_ERROR, "The variable '" + node->token.text + "' does not exists", node->token.pos);
			}
			else if (!symbol_table_.isDeclared(node->token.text))
				error_handler_.report(ErrorType::SEMANTIC_ERROR, "The variable '" + node->token.text + "' does not exists", node->token.pos);
		}
	}

	void SemanticAnalyzer::visit(UnarOpNode* node)
	{
		TokenType op = node->op.type;
		
		if (op == TokenType::PLUS_ONE ||
			op == TokenType::MINUS_ONE ||
			op == TokenType::PRINT ||
			op == TokenType::PLUS ||
			op == TokenType::MINUS ||
			op == TokenType::LOGICAL_NOT ||
			op == TokenType::BIT_NOT ||
			op == TokenType::WAIT ||
			op == TokenType::STATE ||
			op == TokenType::OS ||
			op == TokenType::DO ||
			op == TokenType::SIN ||
			op == TokenType::COS ||
			op == TokenType::TAN ||
			op == TokenType::ACOS ||
			op == TokenType::ASIN ||
			op == TokenType::ATAN ||
			op == TokenType::ABS ||
			op == TokenType::RCEIL ||
			op == TokenType::RFLOOR ||
			op == TokenType::READFILE ||
			op == TokenType::CREATEFILE ||
			op == TokenType::REMOVE ||
			op == TokenType::EXISTS ||
			op == TokenType::CREATEDIR)
			visitNode(node->operand.get());
		else if (op == TokenType::INPUT)
		{
			auto variableNode = dynamic_cast<VariableNode*>(node->operand.get());
			Symbol s(variableNode->token.text, deepness_);

			if (in_console_)
			{
				auto it = std::find_if(symbols_->begin(), symbols_->end(),
					[&](const Symbol& sym) { return sym.name == variableNode->token.text; });

				if (it != symbols_->end())
					*it = s;
				else
					symbols_->push_back(s);
			}

			symbol_table_.addSymbol(s);
		}
	}

	void SemanticAnalyzer::visit(BinarOpNode* node)
	{
		switch (node->op.type)
		{
		case TokenType::ASSIGN:
		{
			DataType type = determineType(node->rightOperand.get());

			auto variableNode = dynamic_cast<VariableNode*>(node->leftOperand.get());
			if (!symbol_table_.getSymbol(variableNode->token.text))
			{
				Symbol s(variableNode->token.text, type, deepness_);
				symbol_table_.addSymbol(s);

				if (in_console_)
				{
					auto it = std::find_if(symbols_->begin(), symbols_->end(),
						[&](const Symbol& sym) { return sym.name == variableNode->token.text; });

					if (it != symbols_->end())
						*it = s;
					else
						symbols_->push_back(s);
				}
			}
			else
			{
				Symbol* s = symbol_table_.getSymbol(variableNode->token.text);
				s->dataType = type;

				if (in_console_)
				{
					auto it = std::find_if(symbols_->begin(), symbols_->end(),
						[&](const Symbol& sym) { return sym.name == variableNode->token.text; });

					if (it != symbols_->end())
						*it = *s;
					else
						symbols_->push_back(*s);
				}
			}
			break;
		}
		case TokenType::PLUS_ASSIGN:
		case TokenType::MINUS_ASSIGN:
		case TokenType::MULTIPLY_ASSIGN:
		case TokenType::DIVIDE_ASSIGN:
		case TokenType::MODULO_ASSIGN:
		case TokenType::FLOOR_ASSIGN:
		case TokenType::POWER_ASSIGN:
		case TokenType::ROOT_ASSIGN:
		case TokenType::LOG_ASSIGN:
		case TokenType::BIT_AND_ASSIGN:
		case TokenType::BIT_OR_ASSIGN:
		case TokenType::BIT_XOR_ASSIGN:
		case TokenType::BIT_LEFT_SHIFT_ASSIGN:
		case TokenType::BIT_RIGHT_SHIFT_ASSIGN:
		{
			DataType type = determineType(node->rightOperand.get());

			auto variableNode = dynamic_cast<VariableNode*>(node->leftOperand.get());

			if (in_console_)
			{
				auto it = std::find_if(symbols_->begin(), symbols_->end(),
					[&](const Symbol& sym) { return sym.name == variableNode->token.text; });

				if (it != symbols_->end())
					it->dataType = type;
				else
					error_handler_.report(ErrorType::SEMANTIC_ERROR, "The variable: '" + variableNode->token.text + "' does not exists", variableNode->token.pos);
			}
			else
			{
				if (!symbol_table_.getSymbol(variableNode->token.text))
					error_handler_.report(ErrorType::SEMANTIC_ERROR, "The variable: '" + variableNode->token.text + "' does not exists", variableNode->token.pos);
				else
				{
					Symbol* s = symbol_table_.getSymbol(variableNode->token.text);
					s->dataType = type;
				}
			}
			break;
		}
		case TokenType::MULTIPLY:
		case TokenType::DIVIDE:
		case TokenType::MODULO:
		case TokenType::POWER:
		case TokenType::FLOOR:
		case TokenType::LOG:
		case TokenType::BIT_AND:
		case TokenType::BIT_OR:
		case TokenType::BIT_XOR:
		case TokenType::BIT_LEFT_SHIFT:
		case TokenType::BIT_RIGHT_SHIFT:
		case TokenType::PLUS:
		case TokenType::MINUS:
		case TokenType::LOGICAL_AND:
		case TokenType::LOGICAL_OR:
		case TokenType::LESS_THAN:
		case TokenType::GREATER_THAN:
		case TokenType::LESS_THAN_OR_EQUAL:
		case TokenType::GREATER_THAN_OR_EQUAL:
		case TokenType::EQUALS:
		case TokenType::NOT_EQUALS:
		case TokenType::TYPE:
		case TokenType::SCROLL:
		case TokenType::WRITEFILE:
		case TokenType::APPENDFILE:
		case TokenType::COPY:
		case TokenType::RENAME:
			visitNode(node->leftOperand.get());
			visitNode(node->rightOperand.get());
			break;

		}
	}

	void SemanticAnalyzer::visit(IfNode* node)
	{
		if (determineType(node->conditionNode.get()) != DataType::BOOL)
			error_handler_.report(ErrorType::SEMANTIC_ERROR, "The condition in 'if' should be a boolean expression", node->token.pos + 1);
		visit(dynamic_cast<BlockNode*>(node->thenBranchNode.get()));
		if (node->elseBranchNode.get()) visit(dynamic_cast<BlockNode*>(node->elseBranchNode.get()));
	}

	void SemanticAnalyzer::visit(ForNode* node)
	{
		bool wasInsideLoop = inside_loop_;
		inside_loop_ = true;

		deepness_++;
		visitNode(node->initializerNode.get());
		deepness_--;

		if (determineType(node->conditionNode.get()) != DataType::BOOL)
			error_handler_.report(ErrorType::SEMANTIC_ERROR, "The condition in 'for' should be a boolean expression", node->token.pos + 2);
		visitNode(node->incrementNode.get());
		visit(dynamic_cast<BlockNode*>(node->bodyNode.get()));

		inside_loop_ = wasInsideLoop;
	}

	void SemanticAnalyzer::visit(WhileNode* node)
	{
		bool wasInsideLoop = inside_loop_;
		inside_loop_ = true;

		if (determineType(node->conditionNode.get()) != DataType::BOOL)
			error_handler_.report(ErrorType::SEMANTIC_ERROR, "The condition in 'while' should be a boolean expression", node->token.pos + 1);
		visit(dynamic_cast<BlockNode*>(node->bodyNode.get()));

		inside_loop_ = wasInsideLoop;
	}

	void SemanticAnalyzer::visit(KeyNode* node)
	{
		for (const auto& b : node->buttonNodes)
			visitNode(b.get());
	}

	void SemanticAnalyzer::visit(MouseNode* node)
	{
		visitNode(node->xNode.get());
		visitNode(node->yNode.get());
		visitNode(node->tNode.get());
	}

	void SemanticAnalyzer::visit(CommandNode* node)
	{
		if (node->type.type == TokenType::BREAK || node->type.type == TokenType::CONTINUE)
		{
			if (!inside_loop_)
			error_handler_.report(ErrorType::SEMANTIC_ERROR, "'" + node->type.text + "' statement is not inside a loop!", node->type.pos);
		}
	}

	DataType SemanticAnalyzer::determineType(AstNode* node)
	{
		if (auto literalNode = dynamic_cast<LiteralNode*>(node))
			return Symbol::convertType(literalNode->token.type);
		else if (auto variableNode = dynamic_cast<VariableNode*>(node))
		{
			visit(variableNode);
			if (Symbol* s = symbol_table_.getSymbol(variableNode->token.text))
				return s->dataType;
		}
		else if (auto unarOpNode = dynamic_cast<UnarOpNode*>(node))
		{
			visit(unarOpNode);
			if (unarOpNode->op.type == TokenType::PLUS || unarOpNode->op.type == TokenType::MINUS || unarOpNode->op.type == TokenType::BIT_NOT)
				return DataType::INT;
			else if (unarOpNode->op.type == TokenType::LOGICAL_NOT)
				return DataType::BOOL;
		}
		else if (auto binaryOpNode = dynamic_cast<BinarOpNode*>(node))
		{
			visit(binaryOpNode);
			return determineBinaryOpType(binaryOpNode);
		}
		return DataType::UNDEFINED;
	}

	DataType SemanticAnalyzer::determineBinaryOpType(BinarOpNode* node)
	{
		DataType leftType = determineType(node->leftOperand.get());
		DataType rightType = determineType(node->rightOperand.get());

		switch (node->op.type)
		{
		case TokenType::PLUS:
		case TokenType::MINUS:
		case TokenType::MULTIPLY:
		case TokenType::DIVIDE:
		case TokenType::MODULO:
		case TokenType::POWER:
		case TokenType::FLOOR:
		case TokenType::LOG:
		case TokenType::BIT_AND:
		case TokenType::BIT_OR:
		case TokenType::BIT_XOR:
		case TokenType::BIT_LEFT_SHIFT:
		case TokenType::BIT_RIGHT_SHIFT:
			if (leftType == DataType::FLOAT || rightType == DataType::FLOAT)
				return DataType::FLOAT;
			else
				return DataType::INT;
		case TokenType::LOGICAL_AND:
		case TokenType::LOGICAL_OR:
		case TokenType::LESS_THAN:
		case TokenType::GREATER_THAN:
		case TokenType::LESS_THAN_OR_EQUAL:
		case TokenType::GREATER_THAN_OR_EQUAL:
		case TokenType::EQUALS:
		case TokenType::NOT_EQUALS:
			return DataType::BOOL;
		default:
			return DataType::UNDEFINED;
		}
	}
}