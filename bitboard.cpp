/// @file bitboard.cpp

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

		if (!(0 <= file && file < 8 &&
			0 <= rank && rank < 8 &&
			file % 2 == rank % 2))
		{
			std::ostringstream error;
			error << "Invalid bitboard - file: " << file
				<< " rank: " << rank;
			throw std::runtime_error(error.str());
		}
	}

	std::pair<int, int> bitboard::to_square(void) const
	{
		assert(1 == this->bit_count());

		if (1 != this->bit_count())
		{
			throw std::runtime_error("Illegal bitboard::to_square() call");
		}

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
		}

		return std::pair<int, int>(file, rank);
	}

	int bitboard::bit_count(void) const
	{
		uint32_t x = this->_bitboard;

		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0f0f0f0f;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return static_cast<int>(x & 0x0000003f);
	}
}

// End of file
