#ifndef FRAMEWORK_D3D_INCLUDE_HPP
#define FRAMEWORK_D3D_INCLUDE_HPP

#include <string>
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h>
#include <Libs\r2tk\r2-exception.hpp>

/**
	Macro for easy throwing
*/
#define DirectXErrorM(result, p_message) Framework::DirectXError(result, p_message, __FILE__, __LINE__)

namespace Framework
{
	/**
		Release a COM resource and set the pointer to NULL.
	*/
	template <typename T>
	inline void SafeRelease(T*& resource)
	{
		if (resource != NULL)
		{
			resource->Release();
			resource = NULL;
		}
	}

	/**
		To be thrown when a DirectX function fails
	*/
	class DirectXError : public r2::Exception::Runtime
	{
	public:
		/**
			Create an exception
		*/
		DirectXError(HRESULT result, const std::string& message, const std::string& file, int line) throw();

		/**
			Create an exception without file/line stamp
		*/
		DirectXError(HRESULT result, const std::string& message) throw();

		/**
			Destructor
		*/
		~DirectXError() throw();
	private:
		/**
			Internal class for combining the result and the message
			into one string.
		*/
		class DirectXMessageConstructor
		{
		public:
			DirectXMessageConstructor(HRESULT result, const std::string& message);

			std::string m_message;
		};

		static const std::string C_CATEGORY;
	};
}

#endif