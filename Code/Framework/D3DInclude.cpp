#include <sstream>
#include <comdef.h>
#include "D3DInclude.hpp"

namespace Framework
{
	const std::string DirectXError::C_CATEGORY("DirectX Error");

	DirectXError::DirectXMessageConstructor::DirectXMessageConstructor(HRESULT result, const std::string& message)
	{
		std::stringstream stream;

		// Get the error message from the HRESULT and combine it with the message.
		_com_error err(result);
		stream << message << " (" << err.ErrorMessage() << ")";

		// Store it in the message
		m_message = stream.str();
	}

	DirectXError::DirectXError(HRESULT result, const std::string& message, const std::string& file, int line) throw()
		: r2::Exception::Runtime(DirectXMessageConstructor(result, message).m_message, C_CATEGORY, file, line)
	{}

	DirectXError::DirectXError(HRESULT result, const std::string& message) throw()
		: r2::Exception::Runtime(DirectXMessageConstructor(result, message).m_message, C_CATEGORY)
	{}

	DirectXError::~DirectXError() throw()
	{}
}