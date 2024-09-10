#include <string>

#include "Intepreter.hpp"
#include "utils/FileReader.hpp"

int main(int argc, char* argv[])
{
	kmsl::utils::FileReader fr("example.kmsl");
	std::string code = fr.read();
	kmsl::Intepreter intepreter(code);
	intepreter.execute();
	return 0;
}