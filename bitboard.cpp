/* $Id: bitboard.cpp,v 1.22 2007-11-26 15:20:21 neo Exp $

   This file is a part of ponder, a English/American checkers game.

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
/** @file bitboard.cpp
 *  @brief Bitboard.
 */

#include <cassert>
#include <stdexcept>
#include "bitboard.hpp"

namespace checkers
{
	/** @param file ASCII lowercase character, `a' to 'h'.
	 *  @param rank ASCII digit character `1' to `8'.
	 */
	bitboard::bitboard(char file, char rank)
	{
		if (file < 'a' || file > 'h')
		{
			/** @throw std::logic_error when @e file is out of
			 *   range.
			 */
			throw std::logic_error(std::string("Error"
				" (illegal square, file out of range): ") +
				file + rank);
		}
		if (rank < '1' || rank > '8')
		{
			/** @throw std::logic_error when @e rank is out of
			 *   range.
			 */
			throw std::logic_error(std::string("Error"
				" (illegal square, rank out of range): ") +
				file + rank);
		}
		if (file % 2 != rank % 2)
		{
			/** @throw std::logic_error when @e file and @e rank
			 *   does not indecate a dark sqare on the game board.
			 */
			throw std::logic_error(std::string("Error"
				" (illegal square, dark square only): ") +
				file + rank);
		}

		this->_bitboard = 0x1U << ((rank - '1') * 4 + (file - 'a') / 2);
	}

	unsigned int bitboard::bitcount(void) const
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
		assert(1 == rhs.bitcount());

		static const char* const square[] =
		{
			"a1", "c1", "e1", "g1", "b2", "d2", "f2", "h2",
			"a3", "c3", "e3", "g3", "b4", "d4", "f4", "h4",
			"a5", "c5", "e5", "g5", "b6", "d6", "f6", "h6",
			"a7", "c7", "e7", "g7", "b8", "d8", "f8", "h8"
		};

		os << square[rhs.ntz()];

		return os;
	}
}

// End of file
