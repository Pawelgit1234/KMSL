#include "Interpreter.hpp"

namespace kmsl
{
	Interpreter::Interpreter() : break_loop_(false), continue_loop_(false),
		exit_program_(false), logging_enabled_(false), console_running_(false), deepness_(0),
		error_handler_(), has_errors_(false) {}

	Interpreter::~Interpreter()
	{
		variables_.clear();
	}

	void Interpreter::execute()
	{
		if (!has_errors_)
			visit(root_.get());

		if (error_handler_.getErrorsCount() > 0)
		{
			error_handler_.showErrors();
			has_errors_ = true;
			error_handler_.clearErrors();
			return;
		}
	}

	void Interpreter::runConsole()
	{
		console_running_ = true;
		bool in_construction = false;

		std::string input;
		std::string construction; // it's for constructions like 'for', 'while', 'if'

		while (true)
		{
			if (in_construction)
				std::cout << ". ";
			else
				std::cout << "> ";

			std::getline(std::cin, input);

			if (input == "")
				in_construction = false;

			Lexer l(input);
			std::vector<Token> tokens = l.scanTokens();

			if (tokens[0].type == TokenType::IF || tokens[0].type == TokenType::WHILE || tokens[0].type == TokenType::FOR || in_construction)
			{
				in_construction = true;
				construction += input + '\n';
			}

			if (!in_construction)
			{
				if (!construction.empty())
					setCode(FileReader::replaceEscapedNewlines(construction));
				else
					setCode(FileReader::replaceEscapedNewlines(input));

				execute();

				if (exit_program_)
					break;

				std::cout << std::endl;
				construction.clear();
			}
		}
	}

	void Interpreter::setCode(const std::string& c, bool auto_visit)
	{
		has_errors_ = false;

		std::string code = c + " "; // for the error_handler, when it cuts the code in lines
		error_handler_.setCode(FileReader::replaceEscapedNewlines(code));

		kmsl::Lexer lexer(code);
		std::vector<kmsl::Token> tokens = lexer.scanTokens();

		if (logging_enabled_)
		{
			std::cout << "LEXER: " << std::endl;
			for (const auto& t : tokens)
				std::cout << "Pos: " << t.pos << " Type: " << (int)t.type << " Text: " << t.text << std::endl;
		}

		kmsl::Parser parser(tokens, error_handler_);
		std::unique_ptr<BlockNode> ast = parser.parse();

		if (error_handler_.getErrorsCount() > 0)
		{
			error_handler_.showErrors();
			has_errors_ = true;
			error_handler_.clearErrors();
			return;
		}

		if (logging_enabled_)
		{
			std::cout << "PARSER: " << std::endl;
			std::cout << ast->toString() << std::endl << std::endl;
		}

		kmsl::SemanticAnalyzer semantic(ast, error_handler_);
		if (console_running_)
			semantic.set_symbols(&symbols_);
		semantic.analyze();

		if (error_handler_.getErrorsCount() > 0)
		{
			error_handler_.showErrors();
			has_errors_ = true;
			error_handler_.clearErrors();
			return;
		}

		if (logging_enabled_)
			std::cout << "SEMANTIC ANALYZER: OK" << std::endl << "PROGRAM OUTPUT:" << std::endl;

		std::srand(std::time(0));

		// DO code must not be setted in root_
		if (auto_visit) 
		{
			visit(ast.get());

			if (error_handler_.getErrorsCount() > 0)
			{
				error_handler_.showErrors();
				has_errors_ = true;
				error_handler_.clearErrors();
				return;
			}
		}
		else
			root_ = std::move(ast);
	}

	variant Interpreter::visitNode(AstNode* node)
	{
		if (auto blockNode = dynamic_cast<BlockNode*>(node))
			return visit(blockNode);
		else if (auto variableNode = dynamic_cast<VariableNode*>(node))
			return visit(variableNode);
		else if (auto literalNode = dynamic_cast<LiteralNode*>(node))
		{
			if (console_running_ && is_printable_)
			{
				is_printable_ = false;
				make_print(literalNode, literalNode->token);
			}
			else
				return visit(literalNode);
		}
		else if (auto unarOpNode = dynamic_cast<UnarOpNode*>(node))
		{
			if (console_running_ && is_printable_)
			{
				is_printable_ = false;
				make_print(unarOpNode, unarOpNode->op);
			}
			else
				return visit(unarOpNode);
		}
		else if (auto binarOpNode = dynamic_cast<BinarOpNode*>(node))
		{
			if (console_running_ && is_printable_)
			{
				is_printable_ = false;
				make_print(binarOpNode, binarOpNode->op);
			}
			else
				return visit(binarOpNode);
		}
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
		else if (auto commandNode = dynamic_cast<CommandNode*>(node))
			visit(commandNode);

		return variant();
	}
	  
	variant Interpreter::visit(BlockNode* node)
	{
		deepness_++;
		for (auto& stmt : node->getStatements())
		{
			if (continue_loop_ || break_loop_ || exit_program_)
			{	
				continue_loop_ = false;
				break;
			}

			auto binaryOpNode = dynamic_cast<BinarOpNode*>(stmt.get());
			auto unarOpNode = dynamic_cast<UnarOpNode*>(stmt.get());

			if (dynamic_cast<VariableNode*>(stmt.get()) || dynamic_cast<LiteralNode*>(stmt.get()) ||
				(binaryOpNode &&
				(binaryOpNode->op.type == TokenType::PLUS ||
				binaryOpNode->op.type == TokenType::MINUS ||
				binaryOpNode->op.type == TokenType::MULTIPLY ||
				binaryOpNode->op.type == TokenType::DIVIDE ||
				binaryOpNode->op.type == TokenType::FLOOR ||
				binaryOpNode->op.type == TokenType::MODULO ||
				binaryOpNode->op.type == TokenType::ROOT ||
				binaryOpNode->op.type == TokenType::LOG ||
				binaryOpNode->op.type == TokenType::POWER ||
				binaryOpNode->op.type == TokenType::PLUS_ONE ||
				binaryOpNode->op.type == TokenType::MINUS_ONE ||
				binaryOpNode->op.type == TokenType::BIT_AND ||
				binaryOpNode->op.type == TokenType::BIT_OR ||
				binaryOpNode->op.type == TokenType::BIT_XOR ||
				binaryOpNode->op.type == TokenType::BIT_NOT ||
				binaryOpNode->op.type == TokenType::BIT_LEFT_SHIFT ||
				binaryOpNode->op.type == TokenType::BIT_RIGHT_SHIFT ||
				binaryOpNode->op.type == TokenType::LOGICAL_AND ||
				binaryOpNode->op.type == TokenType::LOGICAL_OR ||
				binaryOpNode->op.type == TokenType::LOGICAL_NOT ||
				binaryOpNode->op.type == TokenType::EQUALS ||
				binaryOpNode->op.type == TokenType::NOT_EQUALS ||
				binaryOpNode->op.type == TokenType::LESS_THAN ||
				binaryOpNode->op.type == TokenType::GREATER_THAN ||
				binaryOpNode->op.type == TokenType::LESS_THAN_OR_EQUAL ||
				binaryOpNode->op.type == TokenType::GREATER_THAN_OR_EQUAL)||
				(unarOpNode && (unarOpNode->op.type == TokenType::SIN ||
				unarOpNode->op.type == TokenType::COS ||
				unarOpNode->op.type == TokenType::TAN ||
				unarOpNode->op.type == TokenType::ASIN ||
				unarOpNode->op.type == TokenType::ACOS ||
				unarOpNode->op.type == TokenType::ATAN ||
				unarOpNode->op.type == TokenType::ABS ||
				unarOpNode->op.type == TokenType::RCEIL ||
				unarOpNode->op.type == TokenType::RFLOOR ||
				unarOpNode->op.type == TokenType::READFILE ||
				unarOpNode->op.type == TokenType::EXISTS ||
				unarOpNode->op.type == TokenType::PLUS || 
				unarOpNode->op.type == TokenType::MINUS || 
				unarOpNode->op.type == TokenType::BIT_NOT || 
				unarOpNode->op.type == TokenType::STATE ||
				unarOpNode->op.type == TokenType::LOGICAL_NOT))) && console_running_)
			{
				is_printable_ = true;
			}

			visitNode(stmt.get());
		}

		variables_.erase(
			std::remove_if(variables_.begin(), variables_.end(),
				[&](const Variable& var) { return var.deepness > deepness_; }),
			variables_.end());

		deepness_--;

		return variant();
	}

	variant& Interpreter::visit(VariableNode* node)
	{
		if (console_running_ && is_printable_)
		{
			is_printable_ = false;

			UnarOpNode printNode(Token(TokenType::PRINT, "print", node->token.pos), std::make_unique<VariableNode>(*node));
			visit(&printNode);
			return temp_var_;
		}
		else if (node->token.type == TokenType::GETX || node->token.type == TokenType::GETY)
		{
			int x, y;
			IoController::getMouseCoordinates(x, y);

			switch (node->token.type)
			{
			case TokenType::GETX:
				temp_var_ = x;
				break;
			case TokenType::GETY:
				temp_var_ = y;
				break;
			}

			return temp_var_;
		}
		else if (node->token.type == TokenType::RANDOM)
		{
			temp_var_ = std::rand();
			return temp_var_;
		}
		else if (node->token.type == TokenType::MONTH || node->token.type == TokenType::WEEK ||
			node->token.type == TokenType::DAY || node->token.type == TokenType::HOUR ||
			node->token.type == TokenType::MINUTE || node->token.type == TokenType::SECOND ||
			node->token.type == TokenType::MILLI || node->token.type == TokenType::YEAR)
		{
			auto now = std::chrono::system_clock::now();
			std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
			std::tm local_time;
			localtime_s(&local_time, &now_time_t);

			switch (node->token.type)
			{
			case TokenType::YEAR:
				temp_var_ = local_time.tm_year + 1900;
				break;
			case TokenType::MONTH:
				temp_var_ = local_time.tm_mon + 1;
				break;
			case TokenType::DAY:
				temp_var_ = local_time.tm_mday;
				break;
			case TokenType::HOUR:
				temp_var_ = local_time.tm_hour;
				break;
			case TokenType::MINUTE:
				temp_var_ = local_time.tm_min;
				break;
			case TokenType::SECOND:
				temp_var_ = local_time.tm_sec;
				break;
			case TokenType::MILLI:
			{
				auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
					now.time_since_epoch()) % 1000;
				temp_var_ = static_cast<int>(milliseconds.count());
				break;
			}
			case TokenType::WEEK:
			{
				int yday = local_time.tm_yday;
				temp_var_ = yday / 7 + 1;
				break;
			}
			}

			return temp_var_;
		}
		else if (node->token.type == TokenType::PI)
		{
			float pi = M_PI;
			temp_var_ = pi;
			return temp_var_;
		}
		else if (node->token.type == TokenType::PHI)
		{
			float phi = 1.61803399;
			temp_var_ = phi;
			return temp_var_;
		}
		else if (node->token.type == TokenType::E)
		{
			float e = M_E;
			temp_var_ = e;
			return temp_var_;
		}

		auto it = std::find_if(variables_.begin(), variables_.end(),
			[&](const Variable& var) { return var.name == node->token.text; });


		if (it != variables_.end())
			return it->value;

		error_handler_.report(ErrorType::RUNTIME_ERROR, "The variable: '" + node->token.text + "' does not exists", node->token.pos);
	}

	variant Interpreter::visit(UnarOpNode* node)
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
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Expected int/float value for increment/decrement", variableNode->token.pos);
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
				std::cout << (std::get<bool>(value) ? "TRUE" : "FALSE");
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Unsupported type for printing", node->op.pos);
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
					error_handler_.report(ErrorType::RUNTIME_ERROR, "'!' works only with bool", node->op.pos);
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
					error_handler_.report(ErrorType::RUNTIME_ERROR, "'!' works only with bool", node->op.pos);
				else if (op == TokenType::BIT_NOT)
					error_handler_.report(ErrorType::RUNTIME_ERROR, "'~' works only with int", node->op.pos);
			}
			else if (std::holds_alternative<bool>(value))
			{
				if (op == TokenType::PLUS)
					error_handler_.report(ErrorType::RUNTIME_ERROR, "'+' works only with int/float", node->op.pos);
				else if (op == TokenType::MINUS)
					error_handler_.report(ErrorType::RUNTIME_ERROR, "'-' works only with int/float", node->op.pos);
				else if (op == TokenType::LOGICAL_NOT)
					return !std::get<bool>(value);
				else if (op == TokenType::BIT_NOT)
					error_handler_.report(ErrorType::RUNTIME_ERROR, "'~' works only with int", node->op.pos);
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "String cannot operate with +/-/!/~", node->op.pos);
		}
		else if (op == TokenType::INPUT)
		{
			auto variableNode = dynamic_cast<VariableNode*>(node->operand.get());
			variant variable;

			std::string input;
			std::getline(std::cin, input);

			std::stringstream ss(input);

			int intValue;
			float floatValue;

			if (ss >> intValue && ss.eof()) 
				variable = intValue;
			else 
			{
				ss.clear(); 
				ss.str(input); 


				if (ss >> floatValue && ss.eof())
					variable = floatValue;
				else 
					variable = input;
			}

			auto it = std::find_if(variables_.begin(), variables_.end(),
				[&](const Variable& var) { return variableNode->token.text == var.name; });

			if (it != variables_.end()) 
				it->value = variable;
			else 
				variables_.emplace_back(variable, variableNode->token.text, deepness_);
		}
		else if (op == TokenType::STATE)
		{
			variant operand = visitNode(node->operand.get());
			std::string key;

			if (std::holds_alternative<std::string>(operand))
				key = std::get<std::string>(operand);
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "STATE parameter should be string", node->op.pos);

			return IoController::getState(key);
		}
		else if (op == TokenType::WAIT)
		{
			variant operand = visitNode(node->operand.get());
			float time;

			if (std::holds_alternative<int>(operand) || std::holds_alternative<float>(operand))
			{
				if (std::holds_alternative<int>(operand))
					time = static_cast<float>(std::get<int>(operand));
				else if (std::holds_alternative<float>(operand))
					time = std::get<float>(operand);

				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(time * 1000)));
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "WAIT parameter should be int/float", node->op.pos);
		}
		else if (op == TokenType::OS)
		{
			variant operand = visitNode(node->operand.get());
			std::string command;

			if (std::holds_alternative<std::string>(operand))
			{
				command = std::get<std::string>(operand);
				std::system(command.c_str());
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "OS parameter should be string", node->op.pos);
		}
		else if (op == TokenType::SIN || op == TokenType::COS || op == TokenType::TAN || op == TokenType::ACOS || op == TokenType::ASIN || op == TokenType::ATAN || op == TokenType::ABS || op == TokenType::RCEIL || op == TokenType::RFLOOR)
		{
			variant operand = visitNode(node->operand.get());
			float n;

			if (std::holds_alternative<int>(operand) || std::holds_alternative<float>(operand))
			{
				if (std::holds_alternative<int>(operand))
					n = static_cast<float>(std::get<int>(operand));
				else if (std::holds_alternative<float>(operand))
					n = std::get<float>(operand);

				switch (op)
				{
				case TokenType::SIN:
					return std::abs(std::sin(n)) < 1e-7 ? 0.0f : std::sin(n);
				case TokenType::COS:
					return std::cos(n);
				case TokenType::TAN:
					return std::abs(std::tan(n)) < 1e-7 ? 0.0f : std::tan(n);
				case TokenType::ASIN:
					if (n <= 1 && n >= -1)
						return std::asin(n);
					else
						error_handler_.report(ErrorType::RUNTIME_ERROR, "ASIN takes only numbers from -1 to 1", node->op.pos);
					break;
				case TokenType::ACOS:
					if (n <= 1 && n >= -1)
						return std::acos(n);
					else
						error_handler_.report(ErrorType::RUNTIME_ERROR, "ACOS takes only numbers from -1 to 1", node->op.pos);
					break;
				case TokenType::ATAN:
					if (n <= 1 && n >= -1)
						return std::atan(n);
					else
						error_handler_.report(ErrorType::RUNTIME_ERROR, "ATAN takes only numbers from -1 to 1", node->op.pos);
					break;
				case TokenType::ABS:
					return std::abs(n);
				case TokenType::RCEIL:
					return (int)std::ceil(n);
				case TokenType::RFLOOR:
					return (int)std::floor(n);
				default:
					break;
				}
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Math functions takes only int/float", node->op.pos);
		}
		else if (op == TokenType::CREATEFILE)
		{
			variant operand = visitNode(node->operand.get());
			std::string filename;

			if (std::holds_alternative<std::string>(operand))
			{
				filename = std::get<std::string>(operand);
				std::ofstream file(filename);

				if (!file.is_open())
					error_handler_.report(ErrorType::RUNTIME_ERROR, "File '" + filename + "' cannot be created", node->op.pos);
				else
					file.close();
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "CREATEFFILE parameter should be string", node->op.pos);
		}
		else if (op == TokenType::REMOVE)
		{
			variant operand = visitNode(node->operand.get());
			std::string filename;

			if (std::holds_alternative<std::string>(operand))
			{
				filename = std::get<std::string>(operand);
				if (std::filesystem::exists(filename))
					std::filesystem::remove(filename);
				else
					error_handler_.report(ErrorType::RUNTIME_ERROR, "File '" + filename + "' does not exists", node->op.pos);
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "REMOVE parameter should be string", node->op.pos);
		}
		else if (op == TokenType::READFILE)
		{
			variant operand = visitNode(node->operand.get());
			std::string filename;

			if (std::holds_alternative<std::string>(operand))
			{
				filename = std::get<std::string>(operand);
				if (std::filesystem::exists(filename))
				{
					kmsl::FileReader fr(filename);
					std::string text = fr.read();

					return text;
				}
				else 
					error_handler_.report(ErrorType::RUNTIME_ERROR, "File '" + filename + "' cannot be open", node->op.pos);
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "READFILE parameter should be string", node->op.pos);
		}
		else if (op == TokenType::EXISTS)
		{
			variant operand = visitNode(node->operand.get());
			std::string filename;

			if (std::holds_alternative<std::string>(operand))
			{
				filename = std::get<std::string>(operand);
				return std::filesystem::exists(filename);
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "EXISTS parameter should be string", node->op.pos);
		}
		else if (op == TokenType::CREATEDIR)
		{
			variant operand = visitNode(node->operand.get());
			std::string dirname;

			if (std::holds_alternative<std::string>(operand))
			{
				dirname = std::get<std::string>(operand);
				std::filesystem::create_directory(dirname);
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "CREATEDIR parameter should be string", node->op.pos);
		}
		else if (op == TokenType::DO)
		{
			variant operand = visitNode(node->operand.get());
			std::string code;

			if (std::holds_alternative<std::string>(operand))
				code = std::get<std::string>(operand);
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "DO parameter should be string", node->op.pos);

			deepness_--; // let the variable in DO code to has right deepness
			setCode(code, true); 
			deepness_++;
		}

		return variant();
	}

	variant Interpreter::visit(BinarOpNode* node)
	{
		switch (node->op.type)
		{
		case TokenType::ASSIGN:
		{
			auto variableNode = dynamic_cast<VariableNode*>(node->leftOperand.get());

			auto it = std::find_if(variables_.begin(), variables_.end(), 
				[&](const Variable& var) { return variableNode->token.text == var.name; });

			variant valueNode = visitNode(node->rightOperand.get());

			if (it != variables_.end()) 
				it->value = valueNode;
			else
				variables_.emplace_back(valueNode, variableNode->token.text, deepness_);

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
			expand_argumented_assigments(node);
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
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Division by zero", node->op.pos);
						return 0;
					}
					return static_cast<float>(left) / static_cast<float>(right);
				case TokenType::MODULO: return left % right;
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Modulo by zero", node->op.pos);
						return 0;
					}
					return left % right;
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
				case TokenType::POWER: return std::pow(static_cast<float>(left), static_cast<float>(right));
				case TokenType::FLOOR:
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Division by zero in floor operation", node->op.pos);
						return 0;
					}
					return static_cast<int>(std::floor(left / right));
				case TokenType::LOG:
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Logarithm base and argument must be greater than zero", node->op.pos);
						return 0;
					}
					return std::log(static_cast<float>(left)) / std::log(static_cast<float>(right));
				case TokenType::ROOT:
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Root degree must be greater than zero", node->op.pos);
						return 0;
					}
					return std::pow(static_cast<float>(left), 1.0f / static_cast<float>(right));
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
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Division by zero", node->op.pos);
						return 0;
					}
					return left / right;
				case TokenType::POWER: return std::pow(left, right);
				case TokenType::FLOOR:
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Division by zero in floor operation", node->op.pos);
						return 0;
					}
					return static_cast<int>(std::floor(left / right));
				case TokenType::LOG: 
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Logarithm base and argument must be greater than zero", node->op.pos);
						return 0;
					}
					return std::log(left) / std::log(right);
				case TokenType::ROOT:
					if (right == 0)
					{
						error_handler_.report(ErrorType::RUNTIME_ERROR, "Root degree must be greater than zero", node->op.pos);
						return 0;
					}
					return std::pow(left, 1.0f / right);
				case TokenType::LESS_THAN: return left < right;
				case TokenType::GREATER_THAN: return left > right;
				case TokenType::LESS_THAN_OR_EQUAL: return left <= right;
				case TokenType::GREATER_THAN_OR_EQUAL: return left >= right;
				case TokenType::EQUALS: return left == right;
				case TokenType::NOT_EQUALS: return left != right;
				default: error_handler_.report(ErrorType::RUNTIME_ERROR, "Unsupported operation for float", node->op.pos);
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
				default: error_handler_.report(ErrorType::RUNTIME_ERROR, "Unsupported operation for boolean", node->op.pos);
				}
			}
			else if (((std::holds_alternative<std::string>(leftValue) && std::holds_alternative<int>(rightValue)) ||
				(std::holds_alternative<int>(leftValue) && std::holds_alternative<std::string>(rightValue))) && node->op.type == TokenType::MULTIPLY)
			{
				std::string var = std::holds_alternative<std::string>(leftValue) ? std::get<std::string>(leftValue) : std::get<std::string>(rightValue);
				int times = std::holds_alternative<int>(leftValue) ? std::get<int>(leftValue) : std::get<int>(rightValue);

				if (times < 0)
					error_handler_.report(ErrorType::RUNTIME_ERROR, "Cannot multiply string by a negative number", node->op.pos);

				std::string string = var;
				for (int i = 1; i < times; i++)
					var += string;
				return var;
			}
			else if ((std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) ||
					(std::holds_alternative<std::string>(leftValue) && std::holds_alternative<int>(rightValue)) ||
					(std::holds_alternative<int>(leftValue) && std::holds_alternative<std::string>(rightValue)) ||
					(std::holds_alternative<std::string>(leftValue) && std::holds_alternative<float>(rightValue)) ||
					(std::holds_alternative<float>(leftValue) && std::holds_alternative<std::string>(rightValue)) ||
					(std::holds_alternative<std::string>(leftValue) && std::holds_alternative<bool>(rightValue)) ||
					(std::holds_alternative<bool>(leftValue) && std::holds_alternative<std::string>(rightValue)))
			{
				std::string left;
				std::string right;

				if (std::holds_alternative<std::string>(leftValue))
					left = std::get<std::string>(leftValue);
				else if (std::holds_alternative<int>(leftValue))
					left = std::to_string(std::get<int>(leftValue));
				else if (std::holds_alternative<float>(leftValue))
					left = std::to_string(std::get<float>(leftValue));
				else if (std::holds_alternative<bool>(leftValue))
					left = (std::get<bool>(leftValue)) ? "TRUE" : "FALSE";

				if (std::holds_alternative<std::string>(rightValue))
					right = std::get<std::string>(rightValue);
				else if (std::holds_alternative<int>(rightValue))
					right = std::to_string(std::get<int>(rightValue));
				else if (std::holds_alternative<float>(rightValue))
					right = std::to_string(std::get<float>(rightValue));
				else if (std::holds_alternative<bool>(rightValue))
					right = (std::get<bool>(rightValue)) ? "TRUE" : "FALSE";

				switch (node->op.type)
				{
				case TokenType::PLUS: return left + right;
				case TokenType::EQUALS: return left == right;
				case TokenType::NOT_EQUALS: return left != right;
				case TokenType::LESS_THAN: return left.length() < right.length();
				case TokenType::GREATER_THAN: return left.length() > right.length();
				case TokenType::LESS_THAN_OR_EQUAL: return left.length() <= right.length();
				case TokenType::GREATER_THAN_OR_EQUAL: return left.length() >= right.length();
				default: error_handler_.report(ErrorType::RUNTIME_ERROR, "Unsupported operation for strings", node->op.pos);
				}
			}
			else
			{
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Unsupported operand types for binary operation", node->op.pos);
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
				else
					error_handler_.report(ErrorType::RUNTIME_ERROR, "The time parameters type should be int/float", node->op.pos);
			}

			if (node->op.type == TokenType::TYPE)
			{
				if (std::holds_alternative<std::string>(left))
				{
					std::string text = std::get<std::string>(left);
					IoController::type(text, time);
				}
				else
					error_handler_.report(ErrorType::RUNTIME_ERROR, "The type parameter should be string", node->op.pos);
			}
			else if (node->op.type == TokenType::SCROLL)
			{
				if (std::holds_alternative<int>(left))
				{
					int amount = std::get<int>(left);
					IoController::scroll(amount, time);
				}
				else
					error_handler_.report(ErrorType::RUNTIME_ERROR, "The scroll parameter should be int", node->op.pos);
			}
		}
		case TokenType::WRITEFILE:
		case TokenType::APPENDFILE:
		case TokenType::COPY:
		case TokenType::RENAME:
		{
			variant left = visitNode(node->leftOperand.get());
			variant right = visitNode(node->rightOperand.get());

			std::string filename, second;

			if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right))
			{
				filename = std::get<std::string>(left);
				second = std::get<std::string>(right);
			}
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Filename and text parameters should be string", node->op.pos);

			switch (node->op.type)
			{
			case TokenType::WRITEFILE:
			{
				std::ofstream file(filename);

				if (!file.is_open())
					error_handler_.report(ErrorType::RUNTIME_ERROR, "File '" + filename + "' cannot be open", node->op.pos);
				else
				{
					file << second;
					file.close();
				}

				break;
			}
			case TokenType::APPENDFILE:
			{
				std::ofstream file;
				file.open(filename, std::ios_base::app);

				if (!file.is_open())
					error_handler_.report(ErrorType::RUNTIME_ERROR, "File '" + filename + "' cannot be open", node->op.pos);
				else
				{
					file << second;
					file.close();
				}

				break;
			}
			case TokenType::COPY:
				if (!std::filesystem::exists(filename))
					error_handler_.report(ErrorType::RUNTIME_ERROR, "File '" + filename + "' cannot be found", node->op.pos);
				else
					std::filesystem::copy(filename, second);

				break;
			case TokenType::RENAME:
				if (!std::filesystem::exists(filename))
					error_handler_.report(ErrorType::RUNTIME_ERROR, "File '" + filename + "' cannot be found", node->op.pos);
				else
					std::filesystem::rename(filename, second);
				
				break;
			}
		}

		}
		return variant();
	}

	variant Interpreter::visit(IfNode* node)
	{
		variant conditionResult = visitNode(node->conditionNode.get());

		if (!std::holds_alternative<bool>(conditionResult))
			error_handler_.report(ErrorType::RUNTIME_ERROR, "The condition in if should be a boolean expression", node->token.pos);

		if (std::get<bool>(conditionResult))
			visit(dynamic_cast<BlockNode*>(node->thenBranchNode.get()));
		else if (node->elseBranchNode)
			visit(dynamic_cast<BlockNode*>(node->elseBranchNode.get()));

		return variant();
	}

	variant Interpreter::visit(ForNode* node)
	{
		deepness_++; // increasing the deepnees helps to delete the variable in the FOR loop (FOR (i = 0, ..., ...)  )
		visitNode(node->initializerNode.get());
		deepness_--;

		while (true)
		{
			variant conditionResult = visitNode(node->conditionNode.get());

			if (!std::holds_alternative<bool>(conditionResult)) 
				error_handler_.report(ErrorType::RUNTIME_ERROR, "The condition in for should be a boolean expression", node->token.pos);
			if (!std::get<bool>(conditionResult) || break_loop_ || exit_program_)
			{
				break_loop_ = false;
				break;
			}

			visitNode(node->bodyNode.get());
			visitNode(node->incrementNode.get());
		}
		return variant();
	}

	variant Interpreter::visit(WhileNode* node)
	{
		while (true)
		{
			variant conditionResult = visitNode(node->conditionNode.get());

			if (!std::holds_alternative<bool>(conditionResult))
				error_handler_.report(ErrorType::RUNTIME_ERROR, "The condition in for should be a boolean expression", node->token.pos);

			if (!std::get<bool>(conditionResult) || break_loop_ || exit_program_)
			{
				break_loop_ = false;
				break;
			}

			visitNode(node->bodyNode.get());
		}
		return variant();
	}

	variant Interpreter::visit(LiteralNode* node)
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
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Failed to convert string to int", node->token.pos);
			return intValue;
		}
		case DataType::FLOAT:
		{
			float floatValue;
			ss >> floatValue;
			if (ss.fail())
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Failed to convert string to float", node->token.pos);
			return floatValue;
		}
		case DataType::BOOL:
		{
			if (value == "TRUE")
				return true;
			else if (value == "FALSE")
				return false;
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "Failed to convert string to bolean", node->token.pos);
		}
		case DataType::STRING:
		{
			return value;
		}
		}
	}

	variant Interpreter::visit(KeyNode* node)
	{
		auto processButtons = [&](float& time) -> std::vector<std::string> {
			std::vector<std::string> buttons;

			if (!node->buttonNodes.empty())
			{
				auto lastNode = node->buttonNodes.back().get();
				variant lastValue = visitNode(lastNode);

				if (std::holds_alternative<int>(lastValue))
				{
					time = static_cast<float>(std::get<int>(lastValue));
					node->buttonNodes.pop_back();
				}
				else if (std::holds_alternative<float>(lastValue))
				{
					time = std::get<float>(lastValue);
					node->buttonNodes.pop_back();
				}
			}

			for (const auto& btnNode : node->buttonNodes) {
				variant button = visitNode(btnNode.get());
				if (std::holds_alternative<std::string>(button))
					buttons.emplace_back(std::get<std::string>(button));
				else
					error_handler_.report(ErrorType::RUNTIME_ERROR, "All buttons must be string", node->token.pos);
			}

			return buttons;
		};

		float time = 0.f;
		switch (node->token.type)
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
		}

		return variant();
	}

	variant Interpreter::visit(MouseNode* node)
	{
		auto setValues = [&](int& x, int& y, float& time) {
			variant xValue = visitNode(node->xNode.get());
			variant yValue = visitNode(node->yNode.get());

			if (std::holds_alternative<int>(xValue))
				x = std::get<int>(xValue);
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "The x pos of the curser should be int", node->token.pos);

			if (std::holds_alternative<int>(yValue))
				y = std::get<int>(yValue);
			else
				error_handler_.report(ErrorType::RUNTIME_ERROR, "The y pos of the curser should be int", node->token.pos);


			if (node->tNode)
			{
				variant tValue = visitNode(node->tNode.get());
				if (std::holds_alternative<int>(tValue))
					time = static_cast<float>(std::get<int>(tValue));
				else if (std::holds_alternative<float>(tValue))
					time = std::get<float>(tValue);
				else
					error_handler_.report(ErrorType::RUNTIME_ERROR, "The cursor movement time should be int/float", node->token.pos);
			}
		};


		int x, y;
		float time = 0;
		setValues(x, y, time);

		if (node->token.type == TokenType::MOVE)
			IoController::moveTo(x, y, time);
		else
			IoController::moveBy(x, y, time);

		return variant();
	}

	variant Interpreter::visit(CommandNode* node)
	{
		if (node->type.type == TokenType::BREAK)
			break_loop_ = true;
		else if (node->type.type == TokenType::CONTINUE)
			continue_loop_ = true;
		else if (node->type.type == TokenType::EXIT)
			exit_program_ = true;
		return variant();
	}

	// a o= n -----> a = a o n (o = operator)
	// e.g. a += 2 ----> a = a + 2
	void Interpreter::expand_argumented_assigments(BinarOpNode* node)
	{
		auto variableNode = dynamic_cast<VariableNode*>(node->leftOperand.get());

		std::string text = node->op.text.substr(0, node->op.text.size() - 1); // delete the =
		Lexer l(text); // get the type
		Token newToken = Token(l.scanTokens()[0].type, text, node->op.pos);

		std::unique_ptr<BinarOpNode> newNode(std::make_unique<BinarOpNode>(newToken, node->leftOperand->clone(), node->rightOperand->clone()));
		std::unique_ptr<BinarOpNode> fullNewNode(std::make_unique<BinarOpNode>(Token(TokenType::ASSIGN, "=", node->op.pos), node->leftOperand->clone(), std::move(newNode)));
		visit(fullNewNode.get());
	}

	void Interpreter::make_print(AstNode* node, const Token& token)
	{
		UnarOpNode printNode(Token(TokenType::PRINT, "print", token.pos), node->clone());
		visit(&printNode);
	}
}
