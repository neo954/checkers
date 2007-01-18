/** @file move.cpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#include <cassert>
#include <ostream>
#include "move.hpp"

namespace checkers
{
	move::move(const std::string& str)
	{
		assert(move::is_valid(str));

		this->_orig = bitboard(str[0] - 'a', str[1] - '1');
		this->_dest = bitboard(str[2] - 'a', str[3] - '1');
	}

	bitboard move::is_valid_on_black_man(void) const
	{
		assert(1 == this->_orig.bit_count());
		assert(1 == this->_dest.bit_count());

		bitboard is_valid;

		is_valid  = (this->_dest << 4) & this->_orig;
		is_valid |= ((this->_dest & bitboard::MASK_L3) << 3)
			& this->_orig;
		is_valid |= ((this->_dest & bitboard::MASK_L5) << 5)
			& this->_orig;

		return is_valid;
	}

	bitboard move::is_valid_on_white_man(void) const
	{
		assert(1 == this->_orig.bit_count());
		assert(1 == this->_dest.bit_count());

		bitboard is_valid;

		is_valid  = (this->_dest >> 4) & this->_orig;
		is_valid |= ((this->_dest & bitboard::MASK_R3) >> 3)
			& this->_orig;
		is_valid |= ((this->_dest & bitboard::MASK_R5) >> 5)
			& this->_orig;

		return is_valid;
	}

	bitboard move::is_valid_on_king(void) const
	{
		bitboard is_valid = this->is_valid_on_black_man();
		return is_valid ? is_valid : this->is_valid_on_white_man();
	}

	bitboard move::get_black_man_jump_capture(void) const
	{
		assert(1 == this->_orig.bit_count());
		assert(1 == this->_dest.bit_count());

		bitboard capture;

		capture = this->_dest << 4;
		if ((((capture & bitboard::MASK_L3) << 3)
			| ((capture & bitboard::MASK_L5) << 5))
			& this->_orig)
		{
			return capture;
		}

		capture = ((this->_dest & bitboard::MASK_L3) << 3)
			| ((this->_dest & bitboard::MASK_L5) << 5);
		if ((capture << 4) & this->_orig)
		{
			return capture;
		}

		return bitboard::EMPTY;
	}

	bitboard move::get_white_man_jump_capture(void) const
	{
		assert(1 == this->_orig.bit_count());
		assert(1 == this->_dest.bit_count());

		bitboard capture;

		capture = this->_dest >> 4;
		if ((((capture & bitboard::MASK_R3) >> 3)
			| ((capture & bitboard::MASK_R5) >> 5))
			& this->_orig)
		{
			return capture;
		}
	
		capture = ((this->_dest & bitboard::MASK_R3) >> 3)
			| ((this->_dest & bitboard::MASK_R5) >> 5);
		if ((capture >> 4) & this->_orig)
		{
			return capture;
		}

		return bitboard::EMPTY;
	}

	bitboard move::get_king_jump_capture(void) const
	{
		bitboard capture = this->get_black_man_jump_capture();
		return capture ? capture : this->get_white_man_jump_capture();
	}

	std::ostream& operator <<(std::ostream& os, const move& rhs)
	{
		assert(1 == rhs._orig.bit_count());
		assert(1 == rhs._dest.bit_count());

		std::pair<int, int> orig = rhs._orig.to_square();
		std::pair<int, int> dest = rhs._dest.to_square();

		os << static_cast<char>(orig.first + 'a')
			<< static_cast<char>(orig.second + '1')
			<< static_cast<char>(dest.first + 'a')
			<< static_cast<char>(dest.second + '1');

		return os;
	}
}

// End of file
