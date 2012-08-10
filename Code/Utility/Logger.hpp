#ifndef UTILITY_LOGGER_HPP
#define UTILITY_LOGGER_HPP

#include <string>
#include <fstream>

#include <Libs\r2tk\r2-singleton.hpp>

namespace Utility
{
	/**
		A class for logging messages

		Utilize as following:
			Utility::Logger::Instance().I("Information message");
			Utility::Logger::Instance().D("Debug message");
			Utility::Logger::Instance().W("Warning message");
			Utility::Logger::Instance().E("Error message");
		Requires a logger (only one per application) to have been created before use.
	*/
	class Logger : public r2::Singleton<Logger>
	{
	public:
		/**
			Constructor, opens a log file to print to.
		*/
		Logger(const std::string& logFile);
		~Logger() throw();

		/**
			Log a message with generic information. Printed
			in both release and debug.
		*/
		void I(const char* format, ...);

		/**
			Log a message with debug information. Will
			only be printed when the project is compiled in
			debug mode. Will flush immediately.
		*/
		void D(const char* format, ...);

		/**
			Log a message with a warning. Printed
			in both release and debug.
		*/
		void W(const char* format, ...);

		/**
			Log a message with an error. Printed
			in both release and debug. Will flush immediately.
		*/
		void E(const char* format, ...);
	private:
		/**
			Construct a message and print it to both file and console.
		*/
		void PrintString(const std::string& level, const std::string& message, bool flush);

		std::fstream m_logFile;

		static const unsigned int C_BUFFER_SIZE;
	};	
}

#endif