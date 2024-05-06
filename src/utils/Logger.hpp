#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <ctime>

namespace kmsl
{
	namespace utils
	{
		enum class MessageType
		{
			INFO,
			WARNING,
			CRITICAL,
		};

		class Logger
		{		
		public:
			Logger(const std::string& filename);
			~Logger();

			void log(const std::string& message, MessageType type);

		private:
			std::ofstream logfile_;
		};
	}
}