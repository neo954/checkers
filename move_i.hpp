/// @file move_i.hpp

#ifndef __MOVE_I_HPP__
#define __MOVE_I_HPP__

#include <cassert>
#include <stdexcept>

namespace checkers
{
	inline move::move(bitboard orig, bitboard dest) :
		_orig(orig), _dest(dest)
	{
		assert(1 == this->_orig.bit_count());
		assert(1 == this->_dest.bit_count());
		assert(this->is_jump() || this->is_move());
	}

	inline bitboard move::get_orig(void) const
	{
		return this->_orig;
	}

	inline bitboard move::get_dest(void) const
	{
		return this->_dest;
	}

	inline bool move::is_jump(void) const
	{
		return (this->_orig > this->_dest) ?
			((this->_orig >> 7 | this->_orig >> 9) & this->_dest) :
			((this->_orig << 7 | this->_orig << 9) & this->_dest);
	}

	inline bool move::is_move(void) const
	{
		return (this->_orig > this->_dest) ?
			((this->_orig >> 3 | this->_orig >> 4 |
			  this->_orig >> 5) & this->_dest) :
			((this->_orig << 3 | this->_orig << 4 |
			  this->_orig << 5) & this->_dest);
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
}

#endif // __MOVE_I_HPP__
// End of file
