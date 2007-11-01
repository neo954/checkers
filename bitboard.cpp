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
/** @file bitboard.cpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date $Date: 2007-11-01 16:50:07 $
 *  @version $Revision: 1.11 $
 */

#include <cassert>
#include <sstream>
#include <stdexcept>
#include "bitboard.hpp"

namespace checkers
{
	bitboard::bitboard(int file, int rank) :
		_bitboard(0x1 << (rank * 4 + file / 2))
	{
		assert(0 <= file && file < 8);
		assert(0 <= rank && rank < 8);
		assert(file % 2 == rank % 2);
	}

	std::pair<int, int> bitboard::to_square(void) const
	{
		assert(1 == this->bit_count());

		int file;
		int rank;

		switch (this->_bitboard)
		{
		case 0x1 <<  0:
		case 0x1 <<  8:
		case 0x1 << 16:
		case 0x1 << 24:
			file = 0;
			break;
		case 0x1 <<  4:
		case 0x1 << 12:
		case 0x1 << 20:
		case 0x1 << 28:
			file = 1;
			break;
		case 0x1 <<  1:
		case 0x1 <<  9:
		case 0x1 << 17:
		case 0x1 << 25:
			file = 2;
			break;
		case 0x1 <<  5:
		case 0x1 << 13:
		case 0x1 << 21:
		case 0x1 << 29:
			file = 3;
			break;
		case 0x1 <<  2:
		case 0x1 << 10:
		case 0x1 << 18:
		case 0x1 << 26:
			file = 4;
			break;
		case 0x1 <<  6:
		case 0x1 << 14:
		case 0x1 << 22:
		case 0x1 << 30:
			file = 5;
			break;
		case 0x1 <<  3:
		case 0x1 << 11:
		case 0x1 << 19:
		case 0x1 << 27:
			file = 6;
			break;
		case 0x1 <<  7:
		case 0x1 << 15:
		case 0x1 << 23:
		case 0x1 << 31:
			file = 7;
			break;
		default:
			// Should not reach here
			file = -1;
			assert(file);
			break;
		}

		switch (this->_bitboard)
		{
		case 0x1 <<  0:
		case 0x1 <<  1:
		case 0x1 <<  2:
		case 0x1 <<  3:
			rank = 0;
			break;
		case 0x1 <<  4:
		case 0x1 <<  5:
		case 0x1 <<  6:
		case 0x1 <<  7:
			rank = 1;
			break;
		case 0x1 <<  8:
		case 0x1 <<  9:
		case 0x1 << 10:
		case 0x1 << 11:
			rank = 2;
			break;
		case 0x1 << 12:
		case 0x1 << 13:
		case 0x1 << 14:
		case 0x1 << 15:
			rank = 3;
			break;
		case 0x1 << 16:
		case 0x1 << 17:
		case 0x1 << 18:
		case 0x1 << 19:
			rank = 4;
			break;
		case 0x1 << 20:
		case 0x1 << 21:
		case 0x1 << 22:
		case 0x1 << 23:
			rank = 5;
			break;
		case 0x1 << 24:
		case 0x1 << 25:
		case 0x1 << 26:
		case 0x1 << 27:
			rank = 6;
			break;
		case 0x1 << 28:
		case 0x1 << 29:
		case 0x1 << 30:
		case 0x1 << 31:
			rank = 7;
			break;
		default:
			// Should not reach here
			rank = -1;
			assert(rank);
			break;
		}

		return std::pair<int, int>(file, rank);
	}
}

// End of file
