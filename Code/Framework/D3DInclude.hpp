#ifndef FRAMEWORK_D3D_INCLUDE_HPP
#define FRAMEWORK_D3D_INCLUDE_HPP

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10math.h>

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
}

#endif