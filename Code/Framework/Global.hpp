#ifndef FRAMEWORK_GLOBAL_HPP
#define FRAMEWORK_GLOBAL_HPP

#if defined(DEBUG) | defined(_DEBUG)
	#define DEBUG_MODE
#endif

#include <D3D10_1.h>
#include <D3DX10.h>

namespace Framework
{
	/**
		Methods for deleting a pointer and automatically setting it to NULL afterwards.
		Note, deleting NULL is defined to do nothing, so no need to check.
	*/
	template <typename T>
	inline void SafeDelete(T*& memory)
	{
		delete memory;
		memory = NULL;
	}

	template <typename T>
	inline void SafeDeleteArray(T*& memory)
	{
		delete [] memory;
		memory = NULL;
	}

	struct Vertex
	{
		D3DXVECTOR4 m_position;
		D3DXCOLOR m_color;

		Vertex() { }; 
	};
}

#endif