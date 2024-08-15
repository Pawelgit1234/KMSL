#include <string>

#include "Intepreter.hpp"

int main(int argc, char* argv[])
{
	//std::string code = "n = 5\n fact=1\n while(n>0)\n{\nfact *= n\nn--\n}\nprint fact\n a = -(n + 2)\n if (3 * 4 == a )\n{ print \"Hello\"\n}\nelse\n{ print \"World\"\n}\n for (i = 0, i < 10, i++)\n{ print i\n}";
	std::string code = "print 3.5 - 3";
	kmsl::Intepreter intepreter(code);
	intepreter.execute();
	

	return 0;
}