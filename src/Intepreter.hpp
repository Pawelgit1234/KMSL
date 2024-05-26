#pragma once

#include <variant>
#include <string>
#include <unordered_map>

using variant = std::variant<bool, int, float, std::string>;

namespace kmsl
{
	class Intepreter
	{
	public:

	private:
		std::unordered_map<std::string, variant> variables_;
	};
}



