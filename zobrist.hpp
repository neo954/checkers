/* $Id: zobrist.hpp,v 1.12 2009-08-09 13:32:13 neo Exp $

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007, 2008, 2009 Mamiyami Information.
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
/** @file zobrist.hpp
 *  @brief The Zobrist key.
 */

#ifndef __ZOBRIST_HPP__
#define __ZOBRIST_HPP__

extern "C"
{
	#include <stdint.h>
}
#include <cassert>
#include "bitboard.hpp"

namespace checkers
{
	/** @class zobrist
	 *  @brief
	 */
	class zobrist
	{
	public:
		/// Construct from a 64-bit unsigned intenger.
		explicit inline zobrist(uint64_t key = 0x0UL);

		/// Get the Zobrist key.
		inline uint64_t key(void) const;
		void inline change_black_piece(const bitboard& piece);
		void inline change_white_piece(const bitboard& piece);
		void inline change_king(const bitboard& piece);
		void inline change_side(void);

		friend bool operator ==(const zobrist& lhs, const zobrist& rhs);

	private:
		static inline uint16_t rand16(void);
		static inline uint32_t rand32(void);
		static uint64_t rand64(void);

		static uint64_t _black_pieces[];
		static uint64_t _white_pieces[];
		static uint64_t _kings[];
		static uint64_t _change_side;

		uint64_t _key;
	};

	/// Relational ``equal to''.
	inline bool operator ==(const zobrist& lhs, const zobrist& rhs);
	/// Relational ``not equal to''.
	inline bool operator !=(const zobrist& lhs, const zobrist& rhs);
}

#include "zobrist_i.hpp"
#endif // __ZOBRIST_HPP__
// End of file
