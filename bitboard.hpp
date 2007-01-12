/// @file bitboard.hpp

#ifndef __BITBOARD_HPP__
#define __BITBOARD_HPP__

#include <stdint.h>
#include <string>

namespace checkers
{
/*       A   B   C   D   E   F   G   H
 *     +---+---+---+---+---+---+---+---+
 *  8  |   | 28|   | 29|   | 30|   | 31|  8  Black
 *     +---+---+---+---+---+---+---+---+
 *  7  | 24|   | 25|   | 26|   | 27|   |  7
 *     +---+---+---+---+---+---+---+---+
 *  6  |   | 20|   | 21|   | 22|   | 23|  6
 *     +---+---+---+---+---+---+---+---+
 *  5  | 16|   | 17|   | 18|   | 19|   |  5
 *     +---+---+---+---+---+---+---+---+
 *  4  |   | 12|   | 13|   | 14|   | 15|  4
 *     +---+---+---+---+---+---+---+---+
 *  3  |  8|   |  9|   | 10|   | 11|   |  3
 *     +---+---+---+---+---+---+---+---+
 *  2  |   |  4|   |  5|   |  6|   |  7|  2
 *     +---+---+---+---+---+---+---+---+
 *  1  |  0|   |  1|   |  2|   |  3|   |  1  White
 *     +---+---+---+---+---+---+---+---+
 *       A   B   C   D   E   F   G   H
 */

	class bitboard
	{
	public:
		inline bitboard(uint32_t x = 0);
		bitboard(int file, int rank);

		std::pair<int, int> to_square(void) const;
		int bit_count(void) const;

		inline bitboard get_lsb(void) const;

		static const uint32_t EMPTY = 0x0;

		static const uint32_t BLACK_PIECES_INIT =
			0x1 << 20 | 0x1 << 21 | 0x1 << 22 | 0x1 << 23 |
			0x1 << 24 | 0x1 << 25 | 0x1 << 26 | 0x1 << 27 |
			0x1 << 28 | 0x1 << 29 | 0x1 << 30 | 0x1 << 31;
		static const uint32_t WHITE_PIECES_INIT =
			0x1 <<  0 | 0x1 <<  1 | 0x1 <<  2 | 0x1 <<  3 |
			0x1 <<  4 | 0x1 <<  5 | 0x1 <<  6 | 0x1 <<  7 |
			0x1 <<  8 | 0x1 <<  9 | 0x1 << 10 | 0x1 << 11;

		static const uint32_t MASK_L3 =
			0x1 <<  1 | 0x1 <<  2 | 0x1 <<  3 | 0x1 <<  9 |
			0x1 << 10 | 0x1 << 11 | 0x1 << 17 | 0x1 << 18 |
			0x1 << 19 | 0x1 << 25 | 0x1 << 26 | 0x1 << 27;
		static const uint32_t MASK_L5 =
			0x1 <<  4 | 0x1 <<  5 | 0x1 <<  6 | 0x1 << 12 |
			0x1 << 13 | 0x1 << 14 | 0x1 << 20 | 0x1 << 21 |
			0x1 << 22;

		static const uint32_t MASK_R3 =
			0x1 <<  4 | 0x1 <<  5 | 0x1 <<  6 | 0x1 << 12 |
			0x1 << 13 | 0x1 << 14 | 0x1 << 20 | 0x1 << 21 |
			0x1 << 22 | 0x1 << 28 | 0x1 << 29 | 0x1 << 30;
		static const uint32_t MASK_R5 =
			0x1 <<  9 | 0x1 << 10 | 0x1 << 11 | 0x1 << 17 |
			0x1 << 18 | 0x1 << 19 | 0x1 << 25 | 0x1 << 26 |
			0x1 << 27;

		static const uint32_t EDGES =
			0x1 <<  7 | 0x1 <<  8 | 0x1 << 15 | 0x1 << 16 |
			0x1 << 23 | 0x1 << 24;

		static const uint32_t BLACK_KINGS_ROW =
			0x1 <<  0 | 0x1 <<  1 | 0x1 <<  2 | 0x1 <<  3;

		static const uint32_t WHITE_KINGS_ROW =
			0x1 << 28 | 0x1 << 29 | 0x1 << 30 | 0x1 << 31;

		inline bitboard operator <<(int rhs) const;
		inline bitboard operator >>(int rhs) const;

		inline bitboard& operator <<=(int rhs);
		inline bitboard& operator >>=(int rhs);

		inline bitboard operator ~(void) const;
		inline bitboard& operator |=(const bitboard& rhs);
		inline bitboard& operator &=(const bitboard& rhs);
		inline bitboard& operator ^=(const bitboard& rhs);

		inline operator bool(void) const;

		friend bitboard operator |(const bitboard& lhs, const bitboard& rhs);
		friend bitboard operator |(uint32_t lhs, const bitboard& rhs);
		friend bitboard operator |(const bitboard& lhs, uint32_t rhs);
		friend bitboard operator &(const bitboard& lhs, const bitboard& rhs);
		friend bitboard operator &(uint32_t, const bitboard& rhs);
		friend bitboard operator &(const bitboard& lhs, uint32_t rhs);
		friend bitboard operator ^(const bitboard& lhs, const bitboard& rhs);
		friend bitboard operator ^(uint32_t, const bitboard& rhs);
		friend bitboard operator ^(const bitboard& lhs, uint32_t rhs);

	private:
		uint32_t _bitboard;
	};
	
	inline bitboard operator |(const bitboard& lhs, const bitboard& rhs);
	inline bitboard operator |(uint32_t lhs, const bitboard& rhs);
	inline bitboard operator |(const bitboard& lhs, uint32_t rhs);
	inline bitboard operator &(const bitboard& lhs, const bitboard& rhs);
	inline bitboard operator &(uint32_t, const bitboard& rhs);
	inline bitboard operator &(const bitboard& lhs, uint32_t rhs);
	inline bitboard operator ^(const bitboard& lhs, const bitboard& rhs);
	inline bitboard operator ^(uint32_t, const bitboard& rhs);
	inline bitboard operator ^(const bitboard& lhs, uint32_t rhs);
}

#include "bitboard_i.hpp"
#endif // __BITBOARD_HPP__
// End of file
