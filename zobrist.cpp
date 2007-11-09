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
/** @file zobrist.cpp
 *  @brief
 *  $Author: neo $
 *  $Date: 2007-11-09 09:55:02 $
 *  $Revision: 1.2 $
 */

#include "zobrist.hpp"

namespace checkers
{
	zobrist::zobrist(const board& board)
	{
		bitboard pieces;
		bitboard p;

		for (pieces = board.get_black_pieces();
			p = pieces.lsb(), pieces &= ~p;
			static_cast<bool>(pieces))
		{
			this->_key ^= _black_pieces[p.ntz()];
		}

		for (pieces = board.get_white_pieces();
			p = pieces.lsb(), pieces &= ~p;
			static_cast<bool>(pieces))
		{
			this->_key ^= _white_pieces[p.ntz()];
		}

		for (pieces = board.get_kings();
			p = pieces.lsb(), pieces &= ~p;
			static_cast<bool>(pieces))
		{
			this->_key ^= _kings[p.ntz()];
		}
	}

	uint64_t zobrist::rand64(void)
	{
		return rand() ^ ((uint64_t)rand() << 15) ^
			((uint64_t)rand() << 30) ^
			((uint64_t)rand() << 45) ^
			((uint64_t)rand() << 60);
	}

	uint64_t zobrist::_black_pieces[32] =
	{
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
	};
	uint64_t zobrist::_white_pieces[32] =
	{
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
	};
	uint64_t zobrist::_kings[32] =
	{
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
	};
	uint64_t zobrist::_player = rand64();
}

// End of file
