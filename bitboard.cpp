/// @file bitboard.cpp

#include <cassert>
#include <stdexcept>
#include "bitboard.hpp"

namespace checkers
{
	bitboard::bitboard(int file, int rank)
	{
		assert(0 <= file && file < 8);
		assert(0 <= rank && rank < 8);
		assert(file % 2 == rank % 2);

		if (!((0 <= file && file < 8) || (0 <= rank && rank < 8) ||
			(file % 2 == rank % 2)))
		{
			throw std::runtime_error("invalid bitboard");
		}

		this->_bitboard = 0x1 << (rank * 4 + file / 2);
	}

	uint32_t bitboard::bit_count(void) const
	{
		uint32_t x = this->_bitboard;

		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0f0f0f0f;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return x & 0x0000003f;
	}
};
// End of file
