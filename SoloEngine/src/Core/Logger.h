#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <format>
namespace SL
{
	enum class LogLevel
	{
		OFF,
		FATAL,
		ERROR,
		WARNING,
		INFO,
		TRACE,
		INSANE
	};

	class Logger {
	public:

		static inline void Log(std::string msg, LogLevel level)
		{
			if (level <= maxLogLevel_)
			{
				std::string levelStr;

				switch (level)
				{
				case LogLevel::OFF:
					levelStr = "OFF:     ";
					break;
				case LogLevel::FATAL:
					levelStr = "FATAL:   ";
					break;
				case LogLevel::ERROR:
					levelStr = "ERROR:   ";
					break;
				case LogLevel::WARNING:
					levelStr = "WARNING: ";
					break;
				case LogLevel::INFO:
					levelStr = "INFO:    ";
					break;
				case LogLevel::TRACE:
					levelStr = "TRACE   |";
					break;
				case LogLevel::INSANE:
					levelStr = "INSANE   |";
					break;
				default:
					levelStr = "UNKNOWN |";
					break;

				}
				
				std::cout << getTimeStr()  << levelStr << msg << std::endl; // TODO Figure out how to change target to output to gui

			}
		}


        static inline std::string getTimeStr()
        {
            std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

            std::string s(30, '\0');
            #define _CRT_SECURE_NO_WARNINGS
            std::strftime(&s[0], s.size(), "%H:%M:%S |", std::localtime(&now)); // TODO Figure out how to add MS or something here.
            #undef _CRT_SECURE_NO_WARNINGS
            return s;
        }       

		static inline void SetLevel(LogLevel level)
		{
			maxLogLevel_ = level;
			std::string levelStr;

			switch (level)
			{
			case LogLevel::OFF:
				levelStr = "OFF";
				break;
			case LogLevel::FATAL:
				levelStr = "FATAL";
				break;
			case LogLevel::ERROR:
				levelStr = "ERROR";
				break;
			case LogLevel::WARNING:
				levelStr = "WARNING";
				break;
			case LogLevel::INFO:
				levelStr = "INFO";
				break;
			case LogLevel::TRACE:
				levelStr = "TRACE";
				break;
			case LogLevel::INSANE:
				levelStr = "INSANE";
				break;
			default:
				levelStr = "UNKNOWN";
				break;

			}
			Log("Log Level Set to : " + levelStr, LogLevel::INFO);
		}

	private:
		static inline LogLevel maxLogLevel_ = LogLevel::INFO;
	};

} // namespace