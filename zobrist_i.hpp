/* $Id: zobrist_i.hpp,v 1.9 2009-04-10 18:34:23 neo Exp $

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
/** @file zobrist_i.hpp
 *  @brief The Zobrist key.
 */

#ifndef __ZOBRIST_I_HPP__
#define __ZOBRIST_I_HPP__

namespace checkers
{
	inline zobrist::zobrist(uint64_t key) :
		_key(key)
	{
	}

	inline uint64_t zobrist::key(void) const
	{
		return this->_key;
	}

	inline void zobrist::change_black_piece(const bitboard& piece)
	{
		assert(1 == piece.count());
		this->_key ^= this->_black_pieces[piece.ntz()];
	}

	inline void zobrist::change_white_piece(const bitboard& piece)
	{
		assert(1 == piece.count());
		this->_key ^= this->_white_pieces[piece.ntz()];
	}

	inline void zobrist::change_king(const bitboard& piece)
	{
		assert(1 == piece.count());
		this->_key ^= this->_kings[piece.ntz()];
	}

	inline void zobrist::change_side(void)
	{
		this->_key ^= this->_change_side;
	}

	inline uint32_t zobrist::rand32(void)
	{
		static uint32_t seed = 0U;
		seed = (uint32_t(9301U) * seed + uint32_t(49297U)) %
			uint32_t(233280U);

		return seed;
	}

	inline bool operator ==(const zobrist& lhs, const zobrist& rhs)
	{
		return lhs._key == rhs._key;
	}

	inline bool operator !=(const zobrist& lhs, const zobrist& rhs)
	{
		return !(lhs == rhs);
	}
}

#endif // __ZOBRIST_I_HPP__
// End of file
