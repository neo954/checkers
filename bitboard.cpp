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
 *  $Author: neo $
 *  $Date: 2007-11-05 16:24:04 $
 *  $Revision: 1.13 $
 */

#include <cassert>
#include <stdexcept>
#include "bitboard.hpp"

namespace checkers
{
	bitboard::bitboard(char file, char rank)
	{
		if (file < 'a' || file > 'h')
		{
			throw std::logic_error("Error (illegal file):"
				" square");
		}
		if (rank < '1' || rank > '8')
		{
			throw std::logic_error("Error (illegal rank):"
				" square");
		}
		if (file % 2 != rank % 2)
		{
			throw std::logic_error("Error (dark square only):"
				" square");
		}

		this->_bitboard = 0x1 << ((rank - '1') * 4 + (file - 'a') / 2);
	}

	std::ostream& operator <<(std::ostream& os, const bitboard& rhs)
	{
		assert(1 == rhs.bit_count());

		switch (rhs._bitboard)
		{
		case 0x1 <<  0: os << "a1"; break;
		case 0x1 <<  1: os << "c1"; break;
		case 0x1 <<  2: os << "e1"; break;
		case 0x1 <<  3: os << "g1"; break;
		case 0x1 <<  4: os << "b2"; break;
		case 0x1 <<  5: os << "d2"; break;
		case 0x1 <<  6: os << "f2"; break;
		case 0x1 <<  7: os << "h2"; break;
		case 0x1 <<  8: os << "a3"; break;
		case 0x1 <<  9: os << "c3"; break;
		case 0x1 << 10: os << "e3"; break;
		case 0x1 << 11: os << "g3"; break;
		case 0x1 << 12: os << "b4"; break;
		case 0x1 << 13: os << "d4"; break;
		case 0x1 << 14: os << "f4"; break;
		case 0x1 << 15: os << "h4"; break;
		case 0x1 << 16: os << "a5"; break;
		case 0x1 << 17: os << "c5"; break;
		case 0x1 << 18: os << "e5"; break;
		case 0x1 << 19: os << "g5"; break;
		case 0x1 << 20: os << "b6"; break;
		case 0x1 << 21: os << "d6"; break;
		case 0x1 << 22: os << "f6"; break;
		case 0x1 << 23: os << "h6"; break;
		case 0x1 << 24: os << "a7"; break;
		case 0x1 << 25: os << "c7"; break;
		case 0x1 << 26: os << "e7"; break;
		case 0x1 << 27: os << "g7"; break;
		case 0x1 << 28: os << "b8"; break;
		case 0x1 << 29: os << "d8"; break;
		case 0x1 << 30: os << "f8"; break;
		case 0x1 << 31: os << "h8"; break;
		default:
			assert("Should not reach here");
			throw std::logic_error("Error (unknown error): ...");
			break;
		}

		return os;
	}
}

// End of file
