#include <string>

#include "Intepreter.hpp"

#include "io/IoController.hpp"

int main(int argc, char* argv[])
{
	std::string code = "while (true)\n{if (state \"W\")\n{print 1\nwait 1\n}\n}";
	kmsl::Intepreter intepreter(code);
	intepreter.execute();
	return 0;
}