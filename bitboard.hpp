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
/** @file bitboard.hpp
 *  @brief
 *  $Author: neo $
 *  $Date: 2007-11-13 10:21:29 $
 *  $Revision: 1.18 $
 */

#ifndef __BITBOARD_HPP__
#define __BITBOARD_HPP__

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
	 *   8 x 8 board size.  The playable surface consists only of the dark
	 *   squares.  One dark square is at the lower left corner.  The square
	 *   `a1' is marked as `0', it is the Least Significant Bit of the
	 *   bitboard, and square `h8' is marked  as `31', is the Most
	 *   Significant Bit of the bitboard.
	 *  @verbatim
	        A   B   C   D   E   F   G   H
	      +---+---+---+---+---+---+---+---+
	   8  |   | 28|   | 29|   | 30|   | 31|  8  Black
	      +---+---+---+---+---+---+---+---+
	   7  | 24|   | 25|   | 26|   | 27|   |  7
	      +---+---+---+---+---+---+---+---+
	   6  |   | 20|   | 21|   | 22|   | 23|  6
	      +---+---+---+---+---+---+---+---+
	   5  | 16|   | 17|   | 18|   | 19|   |  5
	      +---+---+---+---+---+---+---+---+
	   4  |   | 12|   | 13|   | 14|   | 15|  4
	      +---+---+---+---+---+---+---+---+
	   3  |  8|   |  9|   | 10|   | 11|   |  3
	      +---+---+---+---+---+---+---+---+
	   2  |   |  4|   |  5|   |  6|   |  7|  2
	      +---+---+---+---+---+---+---+---+
	   1  |  0|   |  1|   |  2|   |  3|   |  1  White
	      +---+---+---+---+---+---+---+---+
	        A   B   C   D   E   F   G   H       @endverbatim
	 */
	class bitboard
	{
	public:
		/// Construct from a 32-bit unsigned integer
		explicit inline bitboard(uint32_t x = 0);
		/** @brief Construct a bitboard with only one bit set.
		 *   According to the file and rank on the game board.
		 */
		bitboard(char file, char rank);

		/// Count set bits in bitboard, with traditional method.
		inline unsigned int bitcount(void) const;
		/// Count the Number of Tail Zeros
		inline unsigned int ntz(void) const;
		/// Get the Least Significant Bit
		inline bitboard lsb(void) const;
		/// Empty bitboard
		static const uint32_t EMPTY = 0x0;
		/// Black pieces initial position
		static const uint32_t BLACK_PIECES_INIT =
			0x1 << 20 | 0x1 << 21 | 0x1 << 22 | 0x1 << 23 |
			0x1 << 24 | 0x1 << 25 | 0x1 << 26 | 0x1 << 27 |
			0x1 << 28 | 0x1 << 29 | 0x1 << 30 | 0x1 << 31;
		/// White pieces initial position
		static const uint32_t WHITE_PIECES_INIT =
			0x1 <<  0 | 0x1 <<  1 | 0x1 <<  2 | 0x1 <<  3 |
			0x1 <<  4 | 0x1 <<  5 | 0x1 <<  6 | 0x1 <<  7 |
			0x1 <<  8 | 0x1 <<  9 | 0x1 << 10 | 0x1 << 11;
		/// The bit mask to left shift 3
		static const uint32_t MASK_L3 =
			0x1 <<  1 | 0x1 <<  2 | 0x1 <<  3 | 0x1 <<  9 |
			0x1 << 10 | 0x1 << 11 | 0x1 << 17 | 0x1 << 18 |
			0x1 << 19 | 0x1 << 25 | 0x1 << 26 | 0x1 << 27;
		/// The bit mask to left shift 5
		static const uint32_t MASK_L5 =
			0x1 <<  4 | 0x1 <<  5 | 0x1 <<  6 | 0x1 << 12 |
			0x1 << 13 | 0x1 << 14 | 0x1 << 20 | 0x1 << 21 |
			0x1 << 22;
		/// The bit mask to right shift 3
		static const uint32_t MASK_R3 =
			0x1 <<  4 | 0x1 <<  5 | 0x1 <<  6 | 0x1 << 12 |
			0x1 << 13 | 0x1 << 14 | 0x1 << 20 | 0x1 << 21 |
			0x1 << 22 | 0x1 << 28 | 0x1 << 29 | 0x1 << 30;
		/// The bit mask to right shift 5
		static const uint32_t MASK_R5 =
			0x1 <<  9 | 0x1 << 10 | 0x1 << 11 | 0x1 << 17 |
			0x1 << 18 | 0x1 << 19 | 0x1 << 25 | 0x1 << 26 |
			0x1 << 27;
		/// The bit mask for the game board edges
		static const uint32_t EDGES =
			0x1 <<  7 | 0x1 <<  8 | 0x1 << 15 | 0x1 << 16 |
			0x1 << 23 | 0x1 << 24;
		/** @brief Kings row for black.  Black man will become king
		 *   when reach this row.
		 */
		static const uint32_t BLACK_KINGS_ROW =
			0x1 <<  0 | 0x1 <<  1 | 0x1 <<  2 | 0x1 <<  3;
		/** @brief Kings row for white.  White man will become king
		 *   when reach this row.
		 */
		static const uint32_t WHITE_KINGS_ROW =
			0x1 << 28 | 0x1 << 29 | 0x1 << 30 | 0x1 << 31;

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
		/// Implicit convert to 32-bit unsigned integer.
		inline operator uint32_t(void) const;

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
		friend bool operator !=(const bitboard& lhs,
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
	///
	inline bool operator ==(const bitboard& lhs, const bitboard& rhs);
	///
	inline bool operator !=(const bitboard& lhs, const bitboard& rhs);

	/// Stream out the square name of @e rhs on the game board.
	std::ostream& operator <<(std::ostream& os, const bitboard& rhs);
}

#include "bitboard_i.hpp"
#endif // __BITBOARD_HPP__
// End of file
