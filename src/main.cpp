#include <string>

#include "Intepreter.hpp"

#include "io/IoController.hpp"

int main(int argc, char* argv[])
{
	std::string code = "FOR (i = 0, i < 10, i++) { PRINT i\n !!\n}";
	kmsl::Intepreter intepreter(code);
	intepreter.execute();
	return 0;
}