#include "utils/Logger.hpp"

int main(int argc, char* argv[])
{
	kmsl::utils::Logger log("log.txt");
	log.log("Hello", kmsl::utils::MessageType::INFO);
	return 0;
}