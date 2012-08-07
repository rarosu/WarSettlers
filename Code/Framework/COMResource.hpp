#ifndef FRAMEWORK_COM_RESOURCE_HPP
#define FRAMEWORK_COM_RESOURCE_HPP

#include "D3DInclude.hpp"
#include <Libs\r2tk\r2-assert.hpp>

namespace Framework
{
	/**
		A class for wrapping around COM resources. Allows for safe
		copying of resources and RAII.
	*/
	template <typename T>
	class COMResource
	{
	public:
		COMResource();
		COMResource(T* resource);
		COMResource(COMResource<T>& copy);
		~COMResource();

		COMResource<T>& operator=(T* resource);
		COMResource<T>& operator=(COMResource<T>& copy);

		T* operator->();
		const T* operator->() const;

		bool operator==(const T* rhs) const;
		bool operator!=(const T* rhs) const;
		
		bool operator==(const COMResource<T>& rhs) const;
		bool operator!=(const COMResource<T>& rhs) const;

		T*& Resource();
	private:
		T* m_resource;
	};

	

	/** IMPLEMENTATION */
	template <typename T>
	COMResource<T>::COMResource()
		: m_resource(NULL)
	{}

	template <typename T>
	COMResource<T>::COMResource(T* resource)
		: m_resource(resource)
	{}

	template <typename T>
	COMResource<T>::COMResource(COMResource<T>& copy)
	{
		m_resource = copy.m_resource;
		m_resource->AddRef();
	}

	template <typename T>
	COMResource<T>::~COMResource()
	{
		SafeRelease(m_resource);
	}


	template <typename T>
	COMResource<T>& COMResource<T>::operator=(T* resource)
	{
		SafeRelease(m_resource);

		m_resource = resource;
	}

	template <typename T>
	COMResource<T>& COMResource<T>::operator=(COMResource<T>& copy)
	{
		if (&copy != this)
		{
			SafeRelease(m_resource);

			m_resource = copy.m_resource;
			m_resource->AddRef();
		}
	}

	template <typename T>
	T* COMResource<T>::operator->()
	{
		return m_resource;
	}

	template <typename T>
	const T* COMResource<T>::operator->() const
	{
		return m_resource;
	}

	template <typename T>
	bool COMResource<T>::operator==(const T* rhs) const
	{
		return m_resource == rhs;
	}

	template <typename T>
	bool COMResource<T>::operator!=(const T* rhs) const
	{
		return m_resource != rhs;
	}

	template <typename T>
	bool COMResource<T>::operator==(const COMResource<T>& rhs) const
	{
		return m_resource == rhs.m_resource;
	}

	template <typename T>
	bool COMResource<T>::operator!=(const COMResource<T>& rhs) const
	{
		return m_resource != rhs.m_resource;
	}

	template <typename T>
	T*& COMResource<T>::Resource()
	{
		return m_resource;
	}
}

#endif