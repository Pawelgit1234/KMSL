#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace kmsl
{
	enum class DataType
	{
		INT, FLOAT, STRING, BOOL, UNDEFINED // UNDEFINED can be created by input for example
	};

	struct Symbol // all symbols are variables, so no category
	{
	public:
		Symbol() : name(""), dataType(DataType::UNDEFINED), deepness(0) {}
		Symbol(unsigned int d) : name(""), dataType(DataType::UNDEFINED), deepness(d) {}
		Symbol(const std::string& n, unsigned int d) : name(n), dataType(DataType::UNDEFINED), deepness(d) {}
		Symbol(const std::string& n, DataType type, unsigned int d) : name(n), dataType(type), deepness(d) {}
		Symbol(const std::string& n, TokenType type, unsigned int d) : name(n), dataType(convertType(type)), deepness(d) {}

		static DataType convertType(TokenType type)
		{
			DataType t;
			switch (type)
			{
			case TokenType::INT:
				t = DataType::INT;
				break;
			case TokenType::FLOAT:
				t = DataType::FLOAT;
				break;
			case TokenType::STRING:
				t = DataType::STRING;
				break;
			case TokenType::BOOL:
				t = DataType::BOOL;
				break;
			default:
				t = DataType::UNDEFINED; 
				break;
			}

			return t;
		}

		std::string name;
		DataType dataType;
		unsigned short deepness; // n deeper than n - 1
	};

	class SymbolTable
	{
	public:
		SymbolTable() { enterScope(); }

		void enterScope() { scopes_.emplace_back(); }
		void exitScope() { if (!scopes_.empty()) scopes_.pop_back(); }
		void addSymbol(const Symbol& symbol)
		{
			if (!scopes_.empty())
				scopes_.back()[symbol.name] = symbol;
		}

		Symbol* getSymbol(const std::string& name)
		{
			for (auto it = scopes_.rbegin(); it != scopes_.rend(); ++it) {
				auto symIt = it->find(name);
				if (symIt != it->end())
					return &symIt->second;
			}
			return nullptr;
		}

		bool isDeclared(const std::string& name) { return getSymbol(name) != nullptr; }

	private:
		std::vector<std::unordered_map<std::string, Symbol>> scopes_;
	};
}