/* $Id: bitboard.cpp,v 1.23 2007-11-28 17:17:21 neo Exp $

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
/** @file bitboard.cpp
 *  @brief Bitboard.
 */

#include <cassert>
#include <stdexcept>
#include "bitboard.hpp"

namespace checkers
{
	unsigned int bitboard::count(void) const
	{
		uint32_t x = this->_bitboard;

		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0f0f0f0f;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return x & 0x0000003f;
	}

	unsigned int bitboard::ntz(void) const
	{
		uint32_t x = this->_bitboard;

		if (0 == x)
		{
			return 32;
		}

		unsigned int n = 1;

		if (0 == (x & 0x0000ffff))
		{
			n += 16;
			x >>= 16;
		}
		if (0 == (x & 0x000000ff))
		{
			n += 8;
			x >>= 8;
		}
		if (0 == (x & 0x0000000f))
		{
			n += 4;
			x >>= 4;
		}
		if (0 == (x & 0x00000003))
		{
			n += 2;
			x >>= 2;
		}
		return n - (x & 1);
	}

	std::ostream& operator <<(std::ostream& os, const bitboard& rhs)
	{
		assert(1 == rhs.count());

		os << (rhs.ntz() + 1);

		return os;
	}
}

// End of file
