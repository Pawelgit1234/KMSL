#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

namespace kmsl
{
	enum class DataType
	{
		INT, FLOAT, STRING, BOOL
	};

	struct Symbol // all symbols are variables, so no category
	{
		std::string name;
		DataType dataType;
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