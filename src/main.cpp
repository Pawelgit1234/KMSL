#include <string>

#include "Intepreter.hpp"

#include "io/IoController.hpp"

int main(int argc, char* argv[])
{
	std::string code = "print 'Hello World'"; 
	kmsl::Intepreter intepreter(code);
	intepreter.execute();
	return 0;
}