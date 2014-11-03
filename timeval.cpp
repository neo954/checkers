/* $Id$

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007 Quux Information.
                     Gong Jie <neo@quux.me>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
 */
/** @file timeval.cpp
 *  @brief Time calculate.
 */

#include <cerrno>
#include <cstring>
#include <stdexcept>
#include <string>
#include "timeval.hpp"

namespace checkers
{
	struct timeval timeval::now(void)
	{
		struct timeval now;
		if (::gettimeofday(&now, NULL) < 0)
		{
			/** @throw std::runtime_error when gettimeofday()
			 *   failed.
			 */
			throw std::runtime_error(
				std::string("gettimeofday() failed: ") +
				std::strerror(errno));
		}

		/// @return the time of day.
		return now;
	}


	struct timeval operator -(const struct timeval& rhs)
	{
		struct timeval tv =
		{
			-1 - rhs.tv_sec,
			1000000 - rhs.tv_usec
		};
		return tv;
	}

	struct timeval& operator +=(struct timeval& lhs,
		const struct timeval& rhs)
	{
		lhs.tv_sec  += rhs.tv_sec;
		lhs.tv_usec += rhs.tv_usec;
		if (lhs.tv_usec >= 1000000)
		{
			lhs.tv_usec -= 1000000;
			++lhs.tv_sec;
		}

		return lhs;
	}
}

// End of file
