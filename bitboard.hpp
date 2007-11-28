/* $Id: bitboard.hpp,v 1.27 2007-11-28 17:17:21 neo Exp $

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
/** @file bitboard.hpp
 *  @brief Bitboard.
 */

#ifndef __BITBOARD_HPP__
#define __BITBOARD_HPP__

extern "C"
{
	#include <stdint.h>
}
#include <ostream>

namespace checkers
{
	/** @class bitboard
	 *  @brief A bitboard, used for boardgames such as chess, checkers,
	 *   is a type of data structure and bitset, where each bit represents
	 *   a game position or state, designed for optimization of speed
	 *   and/or memory or disk use in mass calculations.
	 */
	/**  This is the official game board of English/American checkers game,
	 *   8x8 board size.  The playable surface consists only of the dark
	 *   squares.  One dark square is at the lower left corner.
	 *
	 *   The squares are marked from "0" to "31", "0" is the Least
	 *   Significant Bit of the bitboard, and "31", is the Most Significant
	 *   Bit of the bitboard.
	 *
	 *  @verbatim
	     +---+---+---+---+---+---+---+---+
	     |   |(w)|   |(w)|   |(w)|   |(w)| White
	     +---+31-+---+30-+---+29-+---+28-+
	     |(w)|   |(w)|   |(w)|   |(w)|   |
	     +27-+---+26-+---+25-+---+24-+---+
	     |   |(w)|   |(w)|   |(w)|   |(w)|
	     +---+23-+---+22-+---+21-+---+20-+
	     | \ |   | \ |   | \ |   | \ |   |
	     +19-+---+18-+---+17-+---+16-+---+
	     |   | \ |   | \ |   | \ |   | \ |
	     +---+15-+---+14-+---+13-+---+12-+
	     |(b)|   |(b)|   |(b)|   |(b)|   |
	     +11-+---+10-+---+9--+---+8--+---+
	     |   |(b)|   |(b)|   |(b)|   |(b)|
	     +---+7--+---+6--+---+5--+---+4--+
	     |(b)|   |(b)|   |(b)|   |(b)|   | Black
	     +3--+---+2--+---+1--+---+0--+---+ @endverbatim
	 */
	class bitboard
	{
	public:
		/// Construct from a 32-bit unsigned integer
		explicit inline bitboard(uint32_t x = bitboard::EMPTY);

		/// Count set bits in bitboard.
		unsigned int count(void) const;
		/// Count the Number of Tail Zeros
		unsigned int ntz(void) const;
		/// Get the Least Significant Bit
		inline bitboard lsb(void) const;
		/// Empty bitboard
		static const uint32_t EMPTY = 0x0U;
		/// Black pieces initial position
		static const uint32_t BLACK_PIECES_INIT =
			0x1U <<  0 | 0x1U <<  1 | 0x1U <<  2 | 0x1U <<  3 |
			0x1U <<  4 | 0x1U <<  5 | 0x1U <<  6 | 0x1U <<  7 |
			0x1U <<  8 | 0x1U <<  9 | 0x1U << 10 | 0x1U << 11;
		/// White pieces initial position
		static const uint32_t WHITE_PIECES_INIT =
			0x1U << 20 | 0x1U << 21 | 0x1U << 22 | 0x1U << 23 |
			0x1U << 24 | 0x1U << 25 | 0x1U << 26 | 0x1U << 27 |
			0x1U << 28 | 0x1U << 29 | 0x1U << 30 | 0x1U << 31;
		/// The bit mask to left shift 3
		static const uint32_t MASK_L3 =
			0x1U <<  5 | 0x1U <<  6 | 0x1U <<  7 |
			0x1U << 13 | 0x1U << 14 | 0x1U << 15 |
			0x1U << 21 | 0x1U << 22 | 0x1U << 23;
		/// The bit mask to left shift 5
		static const uint32_t MASK_L5 =
			0x1U <<  0 | 0x1U <<  1 | 0x1U <<  2 |
			0x1U <<  8 | 0x1U <<  9 | 0x1U << 10 |
			0x1U << 16 | 0x1U << 17 | 0x1U << 18 |
			0x1U << 24 | 0x1U << 25 | 0x1U << 26;
		/// The bit mask to right shift 3
		static const uint32_t MASK_R3 =
			0x1U <<  8 | 0x1U <<  9 | 0x1U << 10 |
			0x1U << 16 | 0x1U << 17 | 0x1U << 18 |
			0x1U << 24 | 0x1U << 25 | 0x1U << 26;
		/// The bit mask to right shift 5
		static const uint32_t MASK_R5 =
			0x1U <<  5 | 0x1U <<  6 | 0x1U <<  7 |
			0x1U << 13 | 0x1U << 14 | 0x1U << 15 |
			0x1U << 21 | 0x1U << 22 | 0x1U << 23 |
			0x1U << 29 | 0x1U << 30 | 0x1U << 31;
		/// The bit mask for the game board edges
		static const uint32_t EDGES =
			0x1U <<  4 | 0x1U << 11 | 0x1U << 12 |
			0x1U << 19 | 0x1U << 20 | 0x1U << 27;
		/** @brief Kings row for black.  Black man will become king
		 *   when reach this row.
		 */
		static const uint32_t BLACK_KINGS_ROW =
			0x1U << 28 | 0x1U << 29 | 0x1U << 30 | 0x1U << 31;
		/** @brief Kings row for white.  White man will become king
		 *   when reach this row.
		 */
		static const uint32_t WHITE_KINGS_ROW =
			0x1U <<  0 | 0x1U <<  1 | 0x1U <<  2 | 0x1U <<  3;
		/// Logical left shift by @e rhs bit(s).
		inline bitboard operator <<(int rhs) const;
		/// Logical right shift by @e rhs bit(s).
		inline bitboard operator >>(int rhs) const;

		/// Logical left shift by @e rhs bit(s).  Unary operation.
		inline bitboard& operator <<=(int rhs);
		/// Logical right shift by @e rhs bit(s).  Unary operation.
		inline bitboard& operator >>=(int rhs);

		/// Bitwise NOT.
		inline bitboard operator ~(void) const;
		/// Bitwise OR.  Unary operation.
		inline bitboard& operator |=(const bitboard& rhs);
		/// Bitwise AND.  Unary operation.
		inline bitboard& operator &=(const bitboard& rhs);
		/// Bitwise XOR.  Unary operation.
		inline bitboard& operator ^=(const bitboard& rhs);

		/// Implicit convert to bool.
		inline operator bool(void) const;

		friend bitboard operator |(const bitboard& lhs,
			const bitboard& rhs);
		friend bitboard operator |(uint32_t lhs, const bitboard& rhs);
		friend bitboard operator |(const bitboard& lhs, uint32_t rhs);
		friend bitboard operator &(const bitboard& lhs,
			const bitboard& rhs);
		friend bitboard operator &(uint32_t lhs, const bitboard& rhs);
		friend bitboard operator &(const bitboard& lhs, uint32_t rhs);
		friend bitboard operator ^(const bitboard& lhs,
			const bitboard& rhs);
		friend bitboard operator ^(uint32_t lhs, const bitboard& rhs);
		friend bitboard operator ^(const bitboard& lhs, uint32_t rhs);
		friend bool operator ==(const bitboard& lhs,
			 const bitboard& rhs);

		friend std::ostream& operator <<(std::ostream& os,
			const bitboard& rhs);

	private:
		/// The 32-bit unsigned integer to hold the actual bit pattern
		uint32_t _bitboard;
	};

	/// Bitwise OR.
	inline bitboard operator |(const bitboard& lhs, const bitboard& rhs);
	/// Bitwise OR.
	inline bitboard operator |(uint32_t lhs, const bitboard& rhs);
	/// Bitwise OR.
	inline bitboard operator |(const bitboard& lhs, uint32_t rhs);
	/// Bitwise AND.
	inline bitboard operator &(const bitboard& lhs, const bitboard& rhs);
	/// Bitwise AND.
	inline bitboard operator &(uint32_t, const bitboard& rhs);
	/// Bitwise AND.
	inline bitboard operator &(const bitboard& lhs, uint32_t rhs);
	/// Bitwise XOR.
	inline bitboard operator ^(const bitboard& lhs, const bitboard& rhs);
	/// Bitwise XOR.
	inline bitboard operator ^(uint32_t, const bitboard& rhs);
	/// Bitwise XOR.
	inline bitboard operator ^(const bitboard& lhs, uint32_t rhs);
	/// Relational ``equal to''.
	inline bool operator ==(const bitboard& lhs, const bitboard& rhs);
	/// Relational ``not equal to''.
	inline bool operator !=(const bitboard& lhs, const bitboard& rhs);

	/// Stream out the square name of @e rhs on the game board.
	std::ostream& operator <<(std::ostream& os, const bitboard& rhs);
}

#include "bitboard_i.hpp"
#endif // __BITBOARD_HPP__
// End of file
