/// @file move.cpp

#include <cassert>
#include "move.hpp"

namespace checkers
{
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
};
// End of file
