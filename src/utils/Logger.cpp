#include "Logger.hpp"

namespace kmsl
{
	namespace utils
	{
		Logger::Logger(const std::string& filename)
		{
			logfile_.open(filename, std::ios::app);
			if (!logfile_.is_open()) 
				std::cerr << "Error: Unable to open file: " << filename << std::endl;
		}

		Logger::~Logger() {
			if (logfile_.is_open())
				logfile_.close();
		}

		void Logger::log(const std::string& message, MessageType type)
		{
			if (logfile_.is_open())
			{
				time_t now;
				time(&now);
				struct tm timeinfo;
				localtime_s(&timeinfo, &now);
				char buffer[20];
				strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

				std::string str_type;
				switch (type)
				{
				case MessageType::INFO:
					str_type = "INFO";
					break;
				case MessageType::WARNING:
					str_type = "WARNING";
					break;
				case MessageType::CRITICAL:
					str_type = "CRITICAL";
					break;
				default:
					str_type = "UNDEFINED";
					break;
				}

				logfile_ << "[" << buffer << "] [" << str_type << "] " << message << std::endl;
			}
			else
				std::cerr << "Error: Log file is not open." << std::endl;
		}


	}
}