/* HEADER
 *
 * File: r2-global.hpp
 * Created by: Lars Woxberg (Rarosu)
 * Created on: August 25, 2011, 18:25 PM
 *
 * License:
 *   Copyright (C) 2011 Lars Woxberg
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
 * Depends on: 
 *
 * Updates:
 *	2011-08-26 (Rarosu) - Updated with system defines
 */
#ifndef R2_GLOBAL_HPP
#define R2_GLOBAL_HPP


// determine what system we are compiling on.
#if defined(_WIN32) || defined(__WIN32__)
	#define R2_SYSTEM_WINDOWS
#elif defined(linux) || defined(__linux)
	#define R2_SYSTEM_LINUX
#elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
	#define R2_SYSTEM_FREEBSD
#else
	#warning Unrecognized system
#endif


// determine whether the debug flag should be on or not.
#ifdef R2_SYSTEM_WINDOWS
	#ifdef _DEBUG
		#define r2DEBUG
	#endif
#else
	#ifndef NDEBUG
		#define r2DEBUG
	#endif
#endif

#endif
 
 