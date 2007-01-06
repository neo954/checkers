/// @file move.cpp

#include <cassert>
#include "move.hpp"

namespace checkers
{
	move::move(const std::string& input)
	{
		assert(4 == input.size());
		assert('a' <= input[0] && input[0] <= 'h');
		assert('1' <= input[1] && input[1] <= '8');
		assert('a' <= input[2] && input[2] <= 'h');
		assert('1' <= input[3] && input[3] <= '8');
		int orig_file = input[0] - 'a';
		int orig_rank = input[1] - '1';
		int dest_file = input[2] - 'a';
		int dest_rank = input[3] - '1';
		assert(orig_file % 2 == orig_rank % 2);
		assert(dest_file % 2 == dest_rank % 2);

		if (!(4 == input.size() &&
			'a' <= input[0] && input[0] <= 'h' &&
			'1' <= input[1] && input[1] <= '8' &&
			'a' <= input[2] && input[2] <= 'h' &&
			'1' <= input[3] && input[3] <= '8' &&
			orig_file % 2 == orig_rank % 2 &&
			dest_file % 2 == dest_rank % 2))
		{
			throw std::runtime_error("Invalid move - " + input);
		}

		this->_orig = bitboard(orig_file, orig_rank);
		this->_dest = bitboard(dest_file, dest_rank);
	}

	std::string move::to_string(void) const
	{
		assert(1 == this->_orig.bit_count());
		assert(1 == this->_dest.bit_count());

		std::pair<int, int> orig = this->_orig.to_square();
		std::pair<int, int> dest = this->_dest.to_square();

		return std::string(1, static_cast<char>(orig.first + 'a'))
			+ (static_cast<char>(orig.second + '1'))
			+ (static_cast<char>(dest.first + 'a'))
			+ (static_cast<char>(dest.second + '1'));
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
};
// End of file
