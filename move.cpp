/* This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007 Mamiyami Information.
                     Gong Jie <neo@mamiyami.com>

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
/** @file move.cpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date $Date: 2007-11-02 09:44:30 $
 *  @version $Revision: 1.12 $
 */

#include <cassert>
#include <ostream>
#include "move.hpp"

namespace checkers
{
	std::ostream& operator <<(std::ostream& os, const move& rhs)
	{
		assert(1 == rhs._orig.bit_count());
		assert(1 == rhs._dest.bit_count());

		std::pair<int, int> orig = rhs._orig.to_square();
		std::pair<int, int> dest = rhs._dest.to_square();

		os << static_cast<char>(orig.first + 'a')
			<< static_cast<char>(orig.second + '1')
			<< static_cast<char>(dest.first + 'a')
			<< static_cast<char>(dest.second + '1');

		return os;
	}
}

// End of file
