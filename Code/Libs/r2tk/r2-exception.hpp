/* HEADER
 *
 * File: r2-exception.hpp
 * Created by: Lars Woxberg (Rarosu)
 * Created on: October 17, 2010, 1:14 PM
 *
 * License:
 *  Copyright (C) 2010 Lars Woxberg
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Comments:
 *
 * Updates:
 * 	2010-10-17 (Rarosu) - Added I/O exception and macros for easy throwing (postfixed with M)
 * 	2010-10-19 (Rarosu) - Added Exception::DivisionByZero
 * 	2011-04-06 (Rarosu) - Added Exception::NotImplemented
 * 	2011-08-14 (Rarosu) - 
 *		Added the r2 prefix to all exception macros.
 *		Added the Exception namespace and removed the Exception prefix from all classes.
 */

#ifndef R2_EXCEPTION_HPP
#define	R2_EXCEPTION_HPP

#include <string>
#include <stdexcept>
#include <sstream>

/* Macros for using exceptions easily.
 */
#define r2ExceptionRuntimeM(p_message)				r2::Exception::Runtime(p_message, __FILE__, __LINE__)
#define r2ExceptionLogicM(p_message)				r2::Exception::Logic(p_message, __FILE__, __LINE__)
#define r2ExceptionIOM(p_message)					r2::Exception::IO(p_message, __FILE__, __LINE__)
#define r2ExceptionOverflowM(p_message)				r2::Exception::Overflow(p_message, __FILE__, __LINE__)
#define r2ExceptionUnderflowM(p_message)			r2::Exception::Underflow(p_message, __FILE__, __LINE__)
#define r2ExceptionDomainM(p_message)				r2::Exception::Domain(p_message, __FILE__, __LINE__)
#define r2ExceptionArgumentM(p_message)				r2::Exception::Argument(p_message, __FILE__, __LINE__)
#define r2ExceptionOutOfRangeM(p_message)			r2::Exception::OutOfRange(p_message, __FILE__, __LINE__)
#define r2ExceptionDivisionByZeroM(p_message)		r2::Exception::DivisionByZero(p_message, __FILE__, __LINE__)
#define r2ExceptionNotImplementedM(p_message)		r2::Exception::NotImplemented(p_message, __FILE__, __LINE__)

namespace r2
{
	namespace Exception
	{
		namespace priv
		{

			/**
			 * A base exception to extend new exception types from.
			 */
			class Base
			{
			protected:
				/**
				 * Creates an error message and stores it.
				 * @param p_message An explaining text of the error
				 * @param p_category The type of exception (Runtime, I/O, etc.)
				 * @param p_file The file the exception happened in. Use __FILE__.
				 * @param p_line The line the exception happened at. Use __LINE__.
				 */
				Base(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw ();

				/**
				 * Creates an error message without file/line stamp.
				 * @param p_message An explaining text of the error
				 * @param p_category The type of exception (Runtime, I/O, etc.)
				 */
				Base(const std::string& p_message, const std::string& p_category) throw ();

				/**
				 * Virtual destructor - for inheritance purposes.
				 */
				virtual ~Base() throw ();
			protected:

				/**
				 * Get the constructed error message, which should be returned via what().
				 * @return The error message
				 */
				inline const std::string& GetErrorMessage() const throw () {
					return m_error_message;
				}
			private:
				std::string m_error_message;
			};
		}

		/**
		 * Defines errors that can only be determined during runtime (such as an overflow).
		 */
		class Runtime : public std::runtime_error, public priv::Base
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Runtime(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit Runtime(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~Runtime() throw ();

			/**
			 * Get the error message.
			 * @return The formatted error message.
			 */
			virtual const char* what() const throw ();
		protected:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Runtime(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 */
			Runtime(const std::string& p_message, const std::string& p_category) throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * Defines errors in logic that can be avoided before runtime (such as an out-of-range exception).
		 */
		class Logic : public std::logic_error, public priv::Base
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Logic(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit Logic(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~Logic() throw ();

			/**
			 * Get the error message.
			 * @return The formatted error message.
			 */
			virtual const char* what() const throw ();
		protected:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Logic(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 */
			Logic(const std::string& p_message, const std::string& p_category) throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * Defines errors that happen when accessing files and other resources.
		 */
		class IO : public std::ios::failure, public priv::Base
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			IO(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit IO(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~IO() throw ();

			/**
			 * Get the error message.
			 * @return The formatted error message.
			 */
			virtual const char* what() const throw ();
		protected:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			IO(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 */
			IO(const std::string& p_message, const std::string& p_category) throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * To be thrown when a container or buffer will overflow (too much data was added)
		 */
		class Overflow : public Runtime
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Overflow(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit Overflow(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~Overflow() throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * To be thrown when a container or buffer will underflow (too much data was extracted)
		 */
		class Underflow : public Runtime
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Underflow(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit Underflow(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~Underflow() throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * To be thrown when a (mathematical) function is given a parameter outside its mathematical domain.
		 */
		class Domain : public Logic
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Domain(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit Domain(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~Domain() throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * To be thrown when a function is given an invalid argument (e.g. a null pointer).
		 */
		class Argument : public Logic
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Argument(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit Argument(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~Argument() throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * To be thrown when an element whose index is out range is requested.
		 */
		class OutOfRange : public Logic
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			OutOfRange(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit OutOfRange(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~OutOfRange() throw ();
		private:
			static const std::string K_CATEGORY;
		};

		class DivisionByZero : public Logic
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			DivisionByZero(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit DivisionByZero(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~DivisionByZero() throw ();
		private:
			static const std::string K_CATEGORY;
		};

		/**
		 * To be thrown when something has not been implemented but is called upon anyway.
		 */
		class NotImplemented : public Logic
		{
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			NotImplemented(const std::string& p_message, const std::string& p_file, int p_line) throw ();

			/**
			 * Create an exception without file/line stamp.
			 * @param p_message An explaining text of the error
			 */
			explicit NotImplemented(const std::string& p_message) throw ();

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			virtual ~NotImplemented() throw ();
		private:
			static const std::string K_CATEGORY;
		};
	}


}

#endif	/* R2_EXCEPTION_HPP */

