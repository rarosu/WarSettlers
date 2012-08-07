/* HEADER
 *
 * File: r2-assert.hpp
 * Created by: Lars Woxberg (Rarosu)
 * Created on: October 17, 2010, 7:22 PM
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
 *	Depends on: 
 *	+ r2::Exception::priv::Base
 *	+ r2-global.hpp (r2DEBUG)
 *
 * Updates:
 *  2011-08-14 (Rarosu) - 
 *		Added r2 prefix to the assert 
 *		Added extra parenthesis' in macro to give arguments highest operator precedence.
 *		Added Exception namespace and removed Exception prefix from ExceptionAssert.
 *	2011-08-25 (Rarosu) -
 *		Made use of r2DEBUG, a cross-platform debug define.
 *	2011-09-01 (Rarosu) -
 *		Included r2-global.hpp, to make sure the r2DEBUG flag is defined properly.
 */

#ifndef R2_ASSERT_HPP
#define	R2_ASSERT_HPP

#include "r2-global.hpp"
#include "r2-exception.hpp"


// Define an assert macro
#ifdef r2DEBUG
	#define r2AssertM(p_condition, p_message) \
		if (!(p_condition)) \
			throw r2::Exception::Assert(p_message, __FILE__, __LINE__);
#else
	#define r2AssertM(p_condition, p_message)
#endif




namespace r2 {
	namespace Exception {
		/**
		 * Thrown whenever an assertion fails. This should only be thrown in debug. Use
		 * the assert macro included in this file instead of throwing this exception yourself.
		 */
		class Assert : public priv::Base, public std::exception {
		public:
			/**
			 * Create an exception
			 * @param p_message An explaining text of the error
			 * @param p_file The file the exception happened in. Use __FILE__.
			 * @param p_line The line the exception happened at. Use __LINE__.
			 */
			Assert(const std::string& p_message, const std::string& p_file, int p_line) throw();

			/**
			* Virtual destructor - for inheritance purposes.
			*/
			virtual ~Assert() throw();

			/**
			 * Get the error message.
			 * @return The formatted error message.
			 */
			virtual const char* what() const throw();
		private:
			static const std::string K_CATEGORY;
		};
	}
}

#endif	/* R2_ASSERT_HPP */

