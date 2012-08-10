/* HEADER
 *
 * File: r2-singleton.hpp
 * Created by: Lars Woxberg (Rarosu)
 * Created on: September 1, 2011, 1:58 PM
 *
 * License:
 *   Copyright (C) 2010 Lars Woxberg
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Comments:
 *	Be warned that these classes are NOT thread-safe. Add a mutex lock to accessing
 *	a reference/pointer, then making the derived class thread-safe would probably get 
 *	around this problem.
 * Depends on: 
 *
 * Updates:
 *	2012-08-10 (Rarosu) - Removed assert from InstancePtr(). Might want to check if instance has been created by
 *		comparing it to NULL.
 */

#ifndef R2_SINGLETON_HPP
#define R2_SINGLETON_HPP

#include "r2-assert.hpp"

namespace r2 {
	/* A singleton template class. Inherit from this
	 * class to make your class a singleton. Pass along
	 * your class as the template argument.
	 *
	 * Note - this has to be the first class you inherit, if you
	 * are using multiple inheritance.
	 *
	 * Credits to Scott Bilas (Game Programming Gems) for the ideas and tricks
	 * behind this class.
	 */
	template <typename T>
	class Singleton {
	public:
		virtual ~Singleton();

		static T& Instance();
		static T* InstancePtr();
	protected:
		Singleton();
	private:
		static T* s_instance;

		T* GetDerivedAddress(Singleton<T>* p_singleton_ptr);

		Singleton(const Singleton& p_copy) {}
		Singleton& operator=(const Singleton& p_copy) {}
	};


	/* See Singleton.
	 * The difference with this class is that globally, only
	 * a constant reference/pointer can be accessed.
	 */
	template <typename T>
	class SingletonConst {
	public:
		virtual ~SingletonConst();

		static const T& Instance();
		static const T* InstancePtr();
	protected:
		SingletonConst();
	private:
		static T* s_instance;

		T* GetDerivedAddress(SingletonConst<T>* p_singleton_ptr);

		SingletonConst(const SingletonConst& p_copy) {}
		SingletonConst& operator=(const SingletonConst& p_copy) {}
	};










	/* IMPLEMENTATION (Singleton)
	 */

	template <typename T>
	T* Singleton<T>::s_instance = NULL;


	template <typename T>
	Singleton<T>::Singleton() {
		r2AssertM(s_instance == NULL, "Cannot create two or more instances of a singleton");

		s_instance = GetDerivedAddress(this);
	}

	template <typename T>
	Singleton<T>::~Singleton() {
		r2AssertM(s_instance != NULL, "A singleton was deleted without the static instance being set. Please report this.");

		s_instance = NULL;
	}


	template <typename T>
	T& Singleton<T>::Instance() {
		r2AssertM(s_instance != NULL, "Singleton has not been created yet");

		return *s_instance;
	}

	template <typename T>
	T* Singleton<T>::InstancePtr() {
		//r2AssertM(s_instance != NULL, "Singleton has not been created yet");

		return s_instance;
	}

	template <typename T>
	T* Singleton<T>::GetDerivedAddress(Singleton<T>* p_singleton_ptr) {
		int offset;

		// calculate the static offset between the singleton and the template argument class
		offset =	reinterpret_cast<int>(reinterpret_cast<T*>( 1));
		offset -=	reinterpret_cast<int>(reinterpret_cast<Singleton<T>*>( 1));

		// add the offset on the given singleton pointer, to yield the address of the derived class.
		return reinterpret_cast<T*>(reinterpret_cast<int>( p_singleton_ptr + offset ));
	}











	/* IMPLEMENTATION (SingletonConst)
	 */

	template <typename T>
	T* SingletonConst<T>::s_instance = NULL;


	template <typename T>
	SingletonConst<T>::SingletonConst() {
		r2AssertM(s_instance == NULL, "Cannot create two or more instances of a singleton");

		s_instance = GetDerivedAddress(this);
	}

	template <typename T>
	SingletonConst<T>::~SingletonConst() {
		r2AssertM(s_instance != NULL, "A singleton was deleted without the static instance being set. Please report this.");

		s_instance = NULL;
	}

	template <typename T>
	const T& SingletonConst<T>::Instance() {
		r2AssertM(s_instance != NULL, "Singleton has not been created yet");

		return *s_instance;
	}

	template <typename T>
	const T* SingletonConst<T>::InstancePtr() {
		//r2AssertM(s_instance != NULL, "Singleton has not been created yet");

		return s_instance;
	}

	template <typename T>
	T* SingletonConst<T>::GetDerivedAddress(SingletonConst<T>* p_singleton_ptr) {
		int offset;

		// calculate the static offset between the singleton and the template argument class
		offset =	reinterpret_cast<int>(reinterpret_cast<T*>( 1));
		offset -=	reinterpret_cast<int>(reinterpret_cast<SingletonConst<T>*>( 1));

		// add the offset on the given singleton pointer, to yield the address of the derived class.
		return reinterpret_cast<T*>(reinterpret_cast<int>( p_singleton_ptr + offset ));
	}
}

#endif	/* R2_SINGLETON_HPP */
