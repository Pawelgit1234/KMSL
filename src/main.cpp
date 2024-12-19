#include <string>

#include <boost/program_options.hpp>

#include "interpreter/Interpreter.hpp"
#include "interpreter/FileReader.hpp"

int main(int argc, char* argv[])
{
	namespace po = boost::program_options;
	po::options_description desc("Allowed options");

	desc.add_options()
		("version,v", "show the version of KMSL")
		("help,h", "Show help message")
		("log,l", "Enable logging")
		("file", po::value<std::string>(), "File to execute");

	po::positional_options_description p;
	p.add("file", -1);

	po::variables_map vm;
	try
	{
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);
	}
	catch (po::error& e)
	{
		std::cerr << "Error: " << e.what() << "\n";
		return 1;
	}

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		return 0;
	}

	if (vm.count("version"))
	{
		std::cout << "KMSL 1.0.0" << std::endl;
		return 0;
	}

	bool logging_enabled = vm.count("log") > 0;
	
	if (vm.count("file"))
	{
		std::string filepath = vm["file"].as<std::string>();
		kmsl::FileReader fr(filepath);
		std::string code = fr.read();
		
		kmsl::Interpreter interpreter;
		interpreter.setLoggingEnabled(logging_enabled);
		interpreter.setCode(code);
		interpreter.execute();
	}
	else
	{
		kmsl::Interpreter interpreter;
		interpreter.setLoggingEnabled(logging_enabled);
		interpreter.runConsole();
	}
	return 0;
}