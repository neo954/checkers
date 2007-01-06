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
};

#endif // __MOVE_I_HPP__
// End of file
