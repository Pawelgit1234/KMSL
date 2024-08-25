#include <string>

#include "Intepreter.hpp"

#include "io/IoController.hpp"

int main(int argc, char* argv[])
{
	std::string code = "n = 5\n fact=1\n while(n>0)\n{\nfact *= n\nn--\n}\nprint fact\n a = -(n + 2)\n if (3 * 4 == a )\n{ print \"hello\"\n}\nelse\n{ print \"world\"\n}\n for (i = 0, i < 10, i++)\n{ print i\n}";
	std::string code = "scroll 20, 5.6";
	kmsl::intepreter intepreter(code);
	intepreter.execute();

	return 0;
}