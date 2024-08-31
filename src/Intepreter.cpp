#include "Intepreter.hpp"

namespace kmsl
{
	Intepreter::Intepreter(const std::string& code)
	{
		kmsl::Lexer lexer(code);
		std::vector<kmsl::Token> tokens = lexer.scanTokens();

		std::cout << "LEXER: " << std::endl;
		for (const auto& t : tokens) // delete
			std::cout << "Pos: " << t.pos << " Type: " << (int)t.type << " Text: " << t.text << std::endl;

		kmsl::Parser parser(tokens);
		root_ = parser.parse();

		std::cout << "PARSER: " << std::endl;
		std::cout << root_->toString() << std::endl << std::endl << "PROGRAM OUTPUT:" << std::endl; // delete

		kmsl::SemanticAnalyzer semantic(root_);
		semantic.analyze();
	}

	Intepreter::~Intepreter()
	{
		variables_.clear();
	}

	void Intepreter::execute()
	{
		visit(root_.get());
	}

	variant Intepreter::visitNode(AstNode* node)
	{
		if (auto blockNode = dynamic_cast<BlockNode*>(node))
			return visit(blockNode);
		else if (auto variableNode = dynamic_cast<VariableNode*>(node))
			return visit(variableNode);
		else if (auto literalNode = dynamic_cast<LiteralNode*>(node))
			return visit(literalNode);
		else if (auto unarOpNode = dynamic_cast<UnarOpNode*>(node))
			return visit(unarOpNode);
		else if (auto binaryOpNode = dynamic_cast<BinaryOpNode*>(node))
			return visit(binaryOpNode);
		else if (auto ifNode = dynamic_cast<IfNode*>(node))
			return visit(ifNode);
		else if (auto forNode = dynamic_cast<ForNode*>(node))
			return visit(forNode);
		else if (auto whileNode = dynamic_cast<WhileNode*>(node))
			return visit(whileNode);
		else if (auto keyNode = dynamic_cast<KeyNode*>(node))
			return visit(keyNode);
		else if (auto mouseNode = dynamic_cast<MouseNode*>(node))
			return visit(mouseNode);
		return variant();
	}
	  
	variant Intepreter::visit(BlockNode* node)
	{
		for (auto& stmt : node->getStatements())
			visitNode(stmt.get());
		return variant();
	}

	variant& Intepreter::visit(VariableNode* node)
	{
		std::string varName = node->token.text;

		auto it = variables_.find(varName);
		if (it != variables_.end())
			return it->second;
		else
			throw std::runtime_error("Variable " + varName + " not found.");
	}

	variant Intepreter::visit(UnarOpNode* node)
	{
		TokenType op = node->op.type;

		if (op == TokenType::PLUS_ONE || op == TokenType::MINUS_ONE)
		{
			auto variableNode = dynamic_cast<VariableNode*>(node->operand.get());
			variant& value = visit(variableNode);

			if (std::holds_alternative<int>(value))
			{
				if (op == TokenType::PLUS_ONE)
					std::get<int>(value)++;
				else if (op == TokenType::MINUS_ONE)
					std::get<int>(value)--;
			}
			else if (std::holds_alternative<float>(value))
			{
				if (op == TokenType::PLUS_ONE)
					std::get<float>(value)++;
				else if (op == TokenType::MINUS_ONE)
					std::get<float>(value)--;
			}
			else
				throw std::runtime_error("Expected integer/float value for increment/decrement.");
		}
		else if (op == TokenType::PRINT)
		{
			variant value = visitNode(node->operand.get());
			if (std::holds_alternative<int>(value))
				std::cout << std::get<int>(value);
			else if (std::holds_alternative<float>(value))
				std::cout << std::get<float>(value);
			else if (std::holds_alternative<std::string>(value))
				std::cout << std::get<std::string>(value);
			else if (std::holds_alternative<bool>(value))
				std::cout << (std::get<bool>(value) ? "true" : "false");
			else
				throw std::runtime_error("Unsupported type for printing.");
		}
		else if (op == TokenType::PLUS || op == TokenType::MINUS || op == TokenType::LOGICAL_NOT || op == TokenType::BIT_NOT)
		{
			variant value = visitNode(node->operand.get());
			if (std::holds_alternative<int>(value))
			{
				if (op == TokenType::PLUS)
					return +std::get<int>(value);
				else if (op == TokenType::MINUS)
					return -std::get<int>(value);
				else if (op == TokenType::LOGICAL_NOT)
					return !std::get<int>(value);
				else if (op == TokenType::BIT_NOT)
					return ~std::get<int>(value);
			}
			else if (std::holds_alternative<float>(value))
			{
				if (op == TokenType::PLUS)
					return +std::get<float>(value);
				else if (op == TokenType::MINUS)
					return -std::get<float>(value);
				else if (op == TokenType::LOGICAL_NOT)
					return !std::get<float>(value);
				else if (op == TokenType::BIT_NOT)
					throw std::runtime_error("~ works only with int.");
			}
			else
				throw std::runtime_error("Expected integer/float value for increment/decrement.");
		}
		else if (op == TokenType::INPUT)
		{
			auto variableNode = dynamic_cast<VariableNode*>(node->operand.get());
			variant& variable = variables_[variableNode->token.text];

			std::string input;
			std::getline(std::cin, input);

			if (std::holds_alternative<int>(variable))
			{
				int intValue;
				std::stringstream(input) >> intValue;
				if (std::stringstream(input).fail())
					throw std::runtime_error("Invalid input for integer type.");
				variable = intValue;
			}
			else if (std::holds_alternative<float>(variable))
			{
				float floatValue;
				std::stringstream(input) >> floatValue;
				if (std::stringstream(input).fail())
					throw std::runtime_error("Invalid input for float type.");
				variable = floatValue;
			}
			else if (std::holds_alternative<std::string>(variable))
				variable = input;
			else
				throw std::runtime_error("Unsupported type for INPUT operation.");
		}
		else if (op == TokenType::STATE)
		{
			variant operand = visitNode(node->operand.get());
			std::string key;

			if (std::holds_alternative<std::string>(operand))
				key = std::get<std::string>(operand);

			return IoController::getState(key);
		}
		else if (op == TokenType::WAIT)
		{
			variant operand = visitNode(node->operand.get());
			float time;

			if (std::holds_alternative<int>(operand))
				time = static_cast<float>(std::get<int>(operand));
			else if (std::holds_alternative<float>(operand))
				time = std::get<float>(operand);

			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(time * 1000)));
		}
		return variant();
	}

	variant Intepreter::visit(BinaryOpNode* node)
	{
		switch (node->op.type)
		{
		case TokenType::ASSIGN:
		{
			auto variableNode = dynamic_cast<VariableNode*>(node->leftOperand.get());
			variant valueNode = visitNode(node->rightOperand.get());
			variables_[variableNode->token.text] = valueNode;
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
			auto variableNode = dynamic_cast<VariableNode*>(node->leftOperand.get());
			variant& value = visit(variableNode);
			variant valueNode = visitNode(node->rightOperand.get());
			if (std::holds_alternative<int>(value) && std::holds_alternative<int>(valueNode))
			{
				int& var = std::get<int>(value);
				int val = std::get<int>(valueNode);

				switch (node->op.type)
				{
				case TokenType::PLUS_ASSIGN: var += val; break;
				case TokenType::MINUS_ASSIGN: var -= val; break;
				case TokenType::MULTIPLY_ASSIGN: var *= val; break;
				case TokenType::DIVIDE_ASSIGN:
					if (val == 0) throw std::runtime_error("Division by zero.");
					var /= val;
					break;
				case TokenType::MODULO_ASSIGN: var %= val; break;
				case TokenType::BIT_AND_ASSIGN: var &= val; break;
				case TokenType::BIT_OR_ASSIGN: var |= val; break;
				case TokenType::BIT_XOR_ASSIGN: var ^= val; break;
				case TokenType::BIT_LEFT_SHIFT_ASSIGN: var <<= val; break;
				case TokenType::BIT_RIGHT_SHIFT_ASSIGN: var >>= val; break;
				case TokenType::FLOOR_ASSIGN: var = std::floor(var / val); break;
				case TokenType::POWER_ASSIGN: var = std::pow(var, val); break;
				case TokenType::ROOT_ASSIGN: var = std::pow(var, 1.0 / val); break;
				case TokenType::LOG_ASSIGN: var = std::log(var) / std::log(val); break;
				default: throw std::runtime_error("Unsupported assignment operation.");
				}
			}
			else if ((std::holds_alternative<int>(value) && std::holds_alternative<float>(valueNode)) ||
				(std::holds_alternative<float>(value) && std::holds_alternative<int>(valueNode)) ||
				(std::holds_alternative<float>(value) && std::holds_alternative<float>(valueNode)))
			{
				float var = std::holds_alternative<int>(value) ? static_cast<float>(std::get<int>(value)) : std::get<float>(value);
				float val = std::holds_alternative<int>(valueNode) ? static_cast<float>(std::get<int>(valueNode)) : std::get<float>(valueNode);

				switch (node->op.type)
				{
				case TokenType::PLUS_ASSIGN: var += val; break;
				case TokenType::MINUS_ASSIGN: var -= val; break;
				case TokenType::MULTIPLY_ASSIGN: var *= val; break;
				case TokenType::DIVIDE_ASSIGN:
					if (val == 0.0f) throw std::runtime_error("Division by zero.");
					var /= val;
					break;
				case TokenType::MODULO_ASSIGN:
					throw std::runtime_error("Modulo operation not supported for floats.");
					break;
				case TokenType::FLOOR_ASSIGN: var = std::floor(var / val); break;
				case TokenType::POWER_ASSIGN: var = std::pow(var, val); break;
				case TokenType::ROOT_ASSIGN: if (val <= 0.0f) throw std::runtime_error("Root degree must be greater than 0."); var = std::pow(var, 1.0f / val); break;
				case TokenType::LOG_ASSIGN: if (var <= 0.0f || val <= 0.0f) throw std::runtime_error("Logarithm base and argument must be greater than 0."); var = std::log(var) / std::log(val); break;
				default: throw std::runtime_error("Unsupported assignment operation for float.");
				}

				variables_[variableNode->token.text] = var;
			}
			else if (std::holds_alternative<std::string>(value) && std::holds_alternative<std::string>(valueNode))
			{
				std::string& var = std::get<std::string>(value);
				std::string val = std::get<std::string>(valueNode);

				switch (node->op.type)
				{
				case TokenType::PLUS_ASSIGN:
					var += val;
					break;
				default:
					throw std::runtime_error("Unsupported assignment operation for strings.");
				}
			}
			else
				throw std::runtime_error("Unsupported types for assignment operation.");

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
		case TokenType::ROOT:
		{
			variant leftValue = visitNode(node->leftOperand.get());
			variant rightValue = visitNode(node->rightOperand.get());

			if (std::holds_alternative<int>(leftValue) && std::holds_alternative<int>(rightValue))
			{
				int left = std::get<int>(leftValue);
				int right = std::get<int>(rightValue);

				switch (node->op.type)
				{
				case TokenType::PLUS: return left + right;
				case TokenType::MINUS: return left - right;
				case TokenType::MULTIPLY: return left * right;
				case TokenType::DIVIDE:
					if (right == 0) throw std::runtime_error("Division by zero.");
					return left / right;
				case TokenType::MODULO: return left % right;
				case TokenType::BIT_AND: return left & right;
				case TokenType::BIT_OR: return left | right;
				case TokenType::BIT_XOR: return left ^ right;
				case TokenType::BIT_LEFT_SHIFT: return left << right;
				case TokenType::BIT_RIGHT_SHIFT: return left >> right;
				case TokenType::LESS_THAN: return left < right;
				case TokenType::GREATER_THAN: return left > right;
				case TokenType::LESS_THAN_OR_EQUAL: return left <= right;
				case TokenType::GREATER_THAN_OR_EQUAL: return left >= right;
				case TokenType::EQUALS: return left == right;
				case TokenType::NOT_EQUALS: return left != right;
				case TokenType::LOGICAL_AND: return left && right;
				case TokenType::LOGICAL_OR: return left || right;
				case TokenType::POWER: return static_cast<int>(std::pow(left, right));
				case TokenType::FLOOR: return static_cast<int>(std::floor(left / right));
				case TokenType::LOG: return static_cast<int>(std::log(left) / std::log(right));
				case TokenType::ROOT:
					if (right <= 0) throw std::runtime_error("Root degree must be greater than 0.");
					return static_cast<int>(std::pow(left, 1.0 / right));
				}
			}
			else if (std::holds_alternative<float>(leftValue) && std::holds_alternative<float>(rightValue)||
				(std::holds_alternative<int>(leftValue) && std::holds_alternative<float>(rightValue)) ||
				(std::holds_alternative<float>(leftValue) && std::holds_alternative<int>(rightValue)))
			{
				float left = std::holds_alternative<int>(leftValue) ? static_cast<float>(std::get<int>(leftValue)) : std::get<float>(leftValue);
				float right = std::holds_alternative<int>(rightValue) ? static_cast<float>(std::get<int>(rightValue)) : std::get<float>(rightValue);

				switch (node->op.type)
				{
				case TokenType::PLUS: return left + right;
				case TokenType::MINUS: return left - right;
				case TokenType::MULTIPLY: return left * right;
				case TokenType::DIVIDE:
					if (right == 0.0f) throw std::runtime_error("Division by zero.");
					return left / right;
				case TokenType::POWER: return std::pow(left, right);
				case TokenType::FLOOR: return std::floor(left / right);
				case TokenType::LOG: return std::log(left) / std::log(right);
				case TokenType::ROOT:
					if (right <= 0) throw std::runtime_error("Root degree must be greater than 0.");
					return std::pow(left, 1.0f / right);
				case TokenType::LESS_THAN: return left < right;
				case TokenType::GREATER_THAN: return left > right;
				case TokenType::LESS_THAN_OR_EQUAL: return left <= right;
				case TokenType::GREATER_THAN_OR_EQUAL: return left >= right;
				case TokenType::EQUALS: return left == right;
				case TokenType::NOT_EQUALS: return left != right;
				default: throw std::runtime_error("Unsupported operation for float.");
				}
			}
			else if (std::holds_alternative<bool>(leftValue) && std::holds_alternative<bool>(rightValue))
			{
				bool left = std::get<bool>(leftValue);
				bool right = std::get<bool>(rightValue);

				switch (node->op.type)
				{
				case TokenType::LOGICAL_AND: return left && right;
				case TokenType::LOGICAL_OR: return left || right;
				case TokenType::EQUALS: return left == right;
				case TokenType::NOT_EQUALS: return left != right;
				case TokenType::LESS_THAN: return left < right;
				case TokenType::GREATER_THAN: return left > right;
				case TokenType::LESS_THAN_OR_EQUAL: return left <= right;
				case TokenType::GREATER_THAN_OR_EQUAL: return left >= right;
				default: throw std::runtime_error("Unsupported operation for boolean.");
				}
			}
			else if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue))
			{
				std::string left = std::get<std::string>(leftValue);
				std::string right = std::get<std::string>(rightValue);

				switch (node->op.type)
				{
				case TokenType::PLUS: return left + right;
				case TokenType::EQUALS: return left == right;
				case TokenType::NOT_EQUALS: return left != right;
				case TokenType::LESS_THAN: return left < right;
				case TokenType::GREATER_THAN: return left > right;
				case TokenType::LESS_THAN_OR_EQUAL: return left <= right;
				case TokenType::GREATER_THAN_OR_EQUAL: return left >= right;
				default: throw std::runtime_error("Unsupported operation for strings.");
				}
			}
			else
			{
				throw std::runtime_error("Unsupported operand types for binary operation.");
			}

			break;
		}
		case TokenType::TYPE:
		case TokenType::SCROLL:
		{
			variant left = visitNode(node->leftOperand.get());
			variant right;
			float time = 0.f;

			if (node->rightOperand)
			{
				right = visitNode(node->rightOperand.get());
				if (std::holds_alternative<int>(right))
					time = static_cast<float>(std::get<int>(right));
				else if (std::holds_alternative<float>(right))
					time = std::get<float>(right);
			}

			if (node->op.type == TokenType::TYPE)
			{
				if (std::holds_alternative<std::string>(left))
				{
					std::string text = std::get<std::string>(left);
					IoController::type(text, time);
				}
			}
			else if (node->op.type == TokenType::SCROLL)
			{
				if (std::holds_alternative<int>(left))
				{
					int amount = std::get<int>(left);
					IoController::scroll(amount, time);
				}
			}
		}

		}
		return variant();
	}

	variant Intepreter::visit(IfNode* node)
	{
		variant conditionResult = visitNode(node->conditionNode.get());

		if (!std::holds_alternative<bool>(conditionResult)) {
			throw std::runtime_error("The condition in if should be a boolean expression!");
		}

		if (std::get<bool>(conditionResult))
			visit(dynamic_cast<BlockNode*>(node->thenBranchNode.get()));
		else if (node->elseBranchNode)
			visit(dynamic_cast<BlockNode*>(node->elseBranchNode.get()));

		return variant();
	}

	variant Intepreter::visit(ForNode* node)
	{
		visitNode(node->initializerNode.get());

		while (true)
		{
			variant conditionResult = visitNode(node->conditionNode.get());

			if (!std::holds_alternative<bool>(conditionResult)) 
				throw std::runtime_error("The condition in for should be a boolean expression!");

			if (!std::get<bool>(conditionResult))
				break;

			visitNode(node->bodyNode.get());
			visitNode(node->incrementNode.get());
		}
		return variant();
	}

	variant Intepreter::visit(WhileNode* node)
	{
		while (true)
		{
			variant conditionResult = visitNode(node->conditionNode.get());

			if (!std::holds_alternative<bool>(conditionResult))
				throw std::runtime_error("The condition in for should be a boolean expression!");

			if (!std::get<bool>(conditionResult))
				break;

			visitNode(node->bodyNode.get());
		}
		return variant();
	}

	variant Intepreter::visit(LiteralNode* node)
	{
		DataType type = Symbol::convertType(node->token.type);
		auto value = node->token.text;

		std::stringstream ss(value);

		switch (type)
		{
		case DataType::INT:
		{
			int intValue;
			ss >> intValue;
			if (ss.fail())
				throw std::runtime_error("Failed to convert string to int.");
			return intValue;
		}
		case DataType::FLOAT:
		{
			float floatValue;
			ss >> floatValue;
			if (ss.fail())
				throw std::runtime_error("Failed to convert string to float.");
			return floatValue;
		}
		case DataType::BOOL:
		{
			if (value == "true")
				return true;
			else if (value == "false")
				return false;
			else
				throw std::runtime_error("Invalid string value for boolean.");
		}
		case DataType::STRING:
		{
			return value;
		}
		default:
			throw std::runtime_error("Unsupported literal type.");
		}
	}

	variant Intepreter::visit(KeyNode* node)
	{
		auto processButtons = [&](float& time) -> std::vector<std::string> {
			std::vector<std::string> buttons;

			if (!node->buttonNodes.empty()) {
				auto lastNode = node->buttonNodes.back().get();
				variant lastValue = visitNode(lastNode);

				if (std::holds_alternative<int>(lastValue)) {
					time = static_cast<float>(std::get<int>(lastValue));
					node->buttonNodes.pop_back();
				}
				else if (std::holds_alternative<float>(lastValue)) {
					time = std::get<float>(lastValue);
					node->buttonNodes.pop_back();
				}
			}

			for (const auto& btnNode : node->buttonNodes) {
				variant button = visitNode(btnNode.get());
				if (std::holds_alternative<std::string>(button)) {
					buttons.emplace_back(std::get<std::string>(button));
				}
			}

			return buttons;
		};

		float time = 0.f;
		switch (node->token_type)
		{
		case TokenType::PRESS:
		{
			auto buttons = processButtons(time);
			IoController::press(buttons, time);
			break;
		}
		case TokenType::HOLD:
		{
			auto buttons = processButtons(time);
			IoController::hold(buttons);
			break;
		}
		case TokenType::RELEASE:
		{
			auto buttons = processButtons(time);
			IoController::release(buttons);
			break;
		}
		default:
			break;
		}

		return variant();
	}

	variant Intepreter::visit(MouseNode* node)
	{
		auto setValues = [&](int& x, int& y, float& time) {
			variant xValue = visitNode(node->xNode.get());
			variant yValue = visitNode(node->yNode.get());

			if (std::holds_alternative<int>(xValue))
				x = std::get<int>(xValue);

			if (std::holds_alternative<int>(yValue))
				y = std::get<int>(yValue);


			if (node->tNode)
			{
				variant tValue = visitNode(node->tNode.get());
				if (std::holds_alternative<int>(tValue))
					time = static_cast<float>(std::get<int>(tValue));
				else if (std::holds_alternative<float>(tValue))
					time = std::get<float>(tValue);
			}
		};


		int x, y;
		float time = 0;
		setValues(x, y, time);

		if (node->token_type == TokenType::MOVE)
			IoController::moveTo(x, y, time);
		else
			IoController::moveBy(x, y, time);

		return variant();
	}

}
