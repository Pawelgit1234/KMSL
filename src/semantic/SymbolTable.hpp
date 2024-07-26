#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace kmsl
{
	enum class DataType
	{
		INT, FLOAT, STRING, BOOL
	};

	struct Symbol
	{
		std::string name;
		DataType dataType;
	};

	class SymbolTable
	{
	public:
		void addSymbol(const Symbol& symbol) {symbols_[symbol.name] = symbol; }
		Symbol* getSymbol(const std::string& name) {
			auto it = symbols_.find(name);
			if (it != symbols_.end())
				return &it->second;
			return nullptr;
		}
	private:
		std::unordered_map<std::string, Symbol> symbols_;
	};
}