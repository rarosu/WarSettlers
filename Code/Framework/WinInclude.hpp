#ifndef FRAMEWORK_WIN_INCLUDE_HPP
#define FRAMEWORK_WIN_INCLUDE_HPP

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

namespace Framework
{
	/**
		Some helper functions for the primitives in the WinAPI and/or DirectX library
	*/
	inline long GetRectWidth(const RECT& r)
	{
		return r.right - r.left;
	}

	inline long GetRectHeight(const RECT& r)
	{
		return r.bottom - r.top;
	}
}

#endif