/* SOURCE
 *
 * File: r2-exception.cpp
 * Created by: Lars Woxberg (Rarosu)
 * Created on: October 17, 2010, 1:15 PM
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
 *
 * Updates:
 *
 */
#include "r2-exception.hpp"

namespace r2 {
	namespace Exception {
		namespace priv {

			/**
			 * Creates an error message and stores it.
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Base::Base(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw () {
				std::stringstream l_stream;
				l_stream << p_line;

				m_error_message = "[" + p_category + "] [" + p_file + ", " + l_stream.str() + "] " + p_message;
			}

			/**
			 * Creates an error message without file/line stamp.
			 * @param p_message An explaining text of the error
			 * @param p_category The type of exception (Runtime, I/O, etc.)
			 */
			Base::Base(const std::string& p_message, const std::string& p_category) throw () {
				m_error_message = "[" + p_category + "] " + p_message;
			}

			/**
			 * Virtual destructor - for inheritance purposes.
			 */
			Base::~Base() throw () { }
		}


		/* Define the exception categories.
		 */
		const std::string Runtime::K_CATEGORY("Runtime Exception");
		const std::string Logic::K_CATEGORY("Logic Exception");
		const std::string IO::K_CATEGORY("I/O Exception");
		const std::string Overflow::K_CATEGORY("Overflow Exception");
		const std::string Underflow::K_CATEGORY("Underflow Exception");
		const std::string Domain::K_CATEGORY("Domain Exception");
		const std::string Argument::K_CATEGORY("Invalid Argument Exception");
		const std::string OutOfRange::K_CATEGORY("Out Of Range Exception");
		const std::string DivisionByZero::K_CATEGORY("Division By Zero Exception");
		const std::string NotImplemented::K_CATEGORY("Not Implemented Exception");

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Runtime::Runtime(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		priv::Base(p_message, K_CATEGORY, p_file, p_line), std::runtime_error(p_message) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		Runtime::Runtime(const std::string& p_message) throw () :
		priv::Base(p_message, K_CATEGORY), std::runtime_error(p_message) { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_category The type of exception (Runtime, I/O, etc.)
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Runtime::Runtime(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw () :
		priv::Base(p_message, p_category, p_file, p_line), std::runtime_error(p_message) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 * @param p_category The type of exception (Runtime, I/O, etc.)
		 */
		Runtime::Runtime(const std::string& p_message, const std::string& p_category) throw () :
		priv::Base(p_message, p_category), std::runtime_error(p_message) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		Runtime::~Runtime() throw () { }

		/**
		 * Get the error message.
		 * @return The formatted error message.
		 */
		const char* Runtime::what() const throw () {
			return GetErrorMessage().c_str();
		}

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Logic::Logic(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		priv::Base(p_message, K_CATEGORY, p_file, p_line), std::logic_error(p_message) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		Logic::Logic(const std::string& p_message) throw () :
		priv::Base(p_message, K_CATEGORY), std::logic_error(p_message) { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_category The type of exception (Runtime, I/O, etc.)
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Logic::Logic(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw () :
		priv::Base(p_message, p_category, p_file, p_line), std::logic_error(p_message) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 * @param p_category The type of exception (Runtime, I/O, etc.)
		 */
		Logic::Logic(const std::string& p_message, const std::string& p_category) throw () :
		priv::Base(p_message, p_category), std::logic_error(p_message) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		Logic::~Logic() throw () { }

		/**
		 * Get the error message.
		 * @return The formatted error message.
		 */
		const char* Logic::what() const throw () {
			return GetErrorMessage().c_str();
		}

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		IO::IO(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		priv::Base(p_message, K_CATEGORY, p_file, p_line), std::ios::failure(p_message) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		IO::IO(const std::string& p_message) throw () :
		priv::Base(p_message, K_CATEGORY), std::ios::failure(p_message) { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_category The type of exception (Runtime, I/O, etc.)
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		IO::IO(const std::string& p_message, const std::string& p_category, const std::string& p_file, int p_line) throw () :
		priv::Base(p_message, p_category, p_file, p_line), std::ios::failure(p_message) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 * @param p_category The type of exception (Runtime, I/O, etc.)
		 */
		IO::IO(const std::string& p_message, const std::string& p_category) throw () :
		priv::Base(p_message, p_category), std::ios::failure(p_message) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		IO::~IO() throw () { }

		/**
		 * Get the error message.
		 * @return The formatted error message.
		 */
		const char* IO::what() const throw () {
			return GetErrorMessage().c_str();
		}

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Overflow::Overflow(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		Runtime(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		Overflow::Overflow(const std::string& p_message) throw () :
		Runtime(p_message, K_CATEGORY) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		Overflow::~Overflow() throw () { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Underflow::Underflow(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		Runtime(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		Underflow::Underflow(const std::string& p_message) throw () :
		Runtime(p_message, K_CATEGORY) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		Underflow::~Underflow() throw () { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Domain::Domain(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		Logic(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		Domain::Domain(const std::string& p_message) throw () :
		Logic(p_message, K_CATEGORY) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		Domain::~Domain() throw () { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Argument::Argument(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		Logic(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		Argument::Argument(const std::string& p_message) throw () :
		Logic(p_message, K_CATEGORY) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		Argument::~Argument() throw () { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		OutOfRange::OutOfRange(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		Logic(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		OutOfRange::OutOfRange(const std::string& p_message) throw () :
		Logic(p_message, K_CATEGORY) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		OutOfRange::~OutOfRange() throw () { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		DivisionByZero::DivisionByZero(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		Logic(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		DivisionByZero::DivisionByZero(const std::string& p_message) throw () :
		Logic(p_message, K_CATEGORY) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		DivisionByZero::~DivisionByZero() throw () { }

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		NotImplemented::NotImplemented(const std::string& p_message, const std::string& p_file, int p_line) throw () :
		Logic(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Create an exception without file/line stamp.
		 * @param p_message An explaining text of the error
		 */
		NotImplemented::NotImplemented(const std::string& p_message) throw () :
		Logic(p_message, K_CATEGORY) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		NotImplemented::~NotImplemented() throw () { }
	}


}
