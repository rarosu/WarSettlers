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

		PrintString("INFO", buffer, false);

		va_end(args);
	}

	void Logger::D(const char* format, ...)
	{
#ifdef DEBUG_MODE
		va_list args;
		va_start(args, format);

		char buffer[C_BUFFER_SIZE];
		vsprintf_s(buffer, format, args);

		PrintString("DEBUG", buffer, true);

		va_end(args);
#endif
	}

	void Logger::W(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char buffer[C_BUFFER_SIZE];
		vsprintf_s(buffer, format, args);

		PrintString("WARNING", buffer, false);

		va_end(args);
	}

	void Logger::E(const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		char buffer[C_BUFFER_SIZE];
		vsprintf_s(buffer, format, args);

		PrintString("ERROR", buffer, true);

		va_end(args);
	}


	void Logger::PrintString(const std::string& level, const std::string& message, bool flush)
	{
		std::stringstream stream;
		stream << "[" << level << "] " << message;

		m_logFile << stream.str();
		OutputDebugString(stream.str().c_str());
	}
}