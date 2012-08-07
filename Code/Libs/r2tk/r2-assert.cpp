/* SOURCE
 *
 * File: r2-assert.cpp
 * Created by: Lars Woxberg (Rarosu)
 * Created on: October 17, 2010, 7:28 PM
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
#include "r2-assert.hpp"

namespace r2 {
	namespace Exception {

		/* Define the category string for the exception
		 */
		const std::string Assert::K_CATEGORY("Assertion Failure");

		/**
		 * Create an exception
		 * @param p_message An explaining text of the error
		 * @param p_file The file the exception happened in. Use __FILE__.
		 * @param p_line The line the exception happened at. Use __LINE__.
		 */
		Assert::Assert(const std::string& p_message, const std::string& p_file, int p_line) throw () :
			priv::Base(p_message, K_CATEGORY, p_file, p_line) { }

		/**
		 * Virtual destructor - for inheritance purposes.
		 */
		Assert::~Assert() throw () { }

		/**
		 * Get the error message.
		 * @return The formatted error message.
		 */
		const char* Assert::what() const throw () {
			return GetErrorMessage().c_str();
		}
	}
}



