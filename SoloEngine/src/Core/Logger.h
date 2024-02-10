#pragma once
#include <iostream>
#include <string>

namespace Solo
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
					levelStr = "TRACE:   ";
					break;
				case LogLevel::INSANE:
					levelStr = "INSANE:   ";
					break;
				default:
					levelStr = "UNKNOWN: ";
					break;

				}
				
				std::cout << levelStr << msg << std::endl;

			}
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

} // namespace Solo
