/// @file move_i.hpp

#ifndef __MOVE_I_HPP__
#define __MOVE_I_HPP__

#include <cassert>
#include <stdexcept>
#include "bitboard.hpp"

namespace checkers
{
	inline move::move(bitboard orig, bitboard dest) :
		_orig(orig), _dest(dest)
	{
		assert(1 == this->_orig.bit_count());
		assert(1 == this->_dest.bit_count());
		
		if (!(1 == this->_orig.bit_count() &&
			1 == this->_dest.bit_count()))
		{
			throw std::runtime_error("Invalid move");
		}
	}

	inline bitboard move::get_orig(void) const
	{
		return this->_orig;
	}

	inline bitboard move::get_dest(void) const
	{
		return this->_dest;
	}

	inline bool move::is_valid(const std::string& str)
	{
		return (4 == str.size() &&
			'a' <= str[0] && str[0] <= 'h' &&
			'1' <= str[1] && str[1] <= '8' &&
			'a' <= str[2] && str[2] <= 'h' &&
			'1' <= str[3] && str[3] <= '8' &&
			(str[0] - 'a') % 2 == (str[1] - '1') % 2 &&
			(str[2] - 'a') % 2 == (str[3] - '1') % 2);
	}
};

#endif // __MOVE_I_HPP__
// End of file
