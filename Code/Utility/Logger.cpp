#include "Logger.hpp"

#include <cstdarg>
#include <sstream>
#include <Framework\Global.hpp>
#include <Framework\D3DInclude.hpp>

namespace Utility
{
	const unsigned int Logger::C_BUFFER_SIZE = 1024;


	Logger::Logger(const std::string& logFile)
		: m_logFile(logFile, std::ios_base::out)
	{}

	Logger::~Logger() throw()
	{
		m_logFile.close();
	}


	void Logger::I(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char buffer[C_BUFFER_SIZE];
		vsprintf_s(buffer, format, args);

		std::string message = ConstructMessage("INFO", buffer);
		LogMessageToFile(message, false);
		LogMessageToConsole(message);

		va_end(args);
	}

	void Logger::D(const char* format, ...)
	{
#ifdef DEBUG_MODE
		va_list args;
		va_start(args, format);

		char buffer[C_BUFFER_SIZE];
		vsprintf_s(buffer, format, args);

		std::string message = ConstructMessage("DEBUG", buffer);
		LogMessageToFile(message, true);
		LogMessageToConsole(message);

		va_end(args);
#endif
	}

	void Logger::W(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char buffer[C_BUFFER_SIZE];
		vsprintf_s(buffer, format, args);

		std::string message = ConstructMessage("WARNING", buffer);
		LogMessageToFile(message, false);
		LogMessageToConsole(message);

		va_end(args);
	}

	void Logger::E(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char buffer[C_BUFFER_SIZE];
		vsprintf_s(buffer, format, args);

		std::string message = ConstructMessage("ERROR", buffer);
		LogMessageToFile(message, true);
		LogMessageToConsole(message);
		MessageBox(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);

		va_end(args);
	}

	std::string Logger::ConstructMessage(const std::string& level, const std::string& message)
	{
		std::stringstream stream;
		stream << "[" << level << "] " << message;

		return stream.str();
	}

	void Logger::LogMessageToFile(const std::string& message, bool flush)
	{
		m_logFile << message;
		
		if (flush)
			m_logFile.flush();
	}

	void Logger::LogMessageToConsole(const std::string& message)
	{
		OutputDebugString(message.c_str());
	}
}