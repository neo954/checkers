/// @file bitboard_i.hpp

#ifndef __BITBOARD_I_HPP__
#define __BITBOARD_I_HPP__

namespace checkers
{
	inline bitboard::bitboard(uint32_t x) :
		_bitboard(x)
	{
	}

	inline bitboard bitboard::get_lsb(void) const
	{
		return this->_bitboard & (-this->_bitboard);
	}

	inline bitboard bitboard::operator <<(int rhs) const
	{
		return this->_bitboard << rhs;
	}

	inline bitboard bitboard::operator >>(int rhs) const
	{
		return this->_bitboard >> rhs;
	}

	inline bitboard& bitboard::operator <<=(int rhs)
	{
		this->_bitboard <<= rhs;
		return *this;
	}

	inline bitboard& bitboard::operator >>=(int rhs)
	{
		this->_bitboard >>= rhs;
		return *this;
	}

	inline bitboard bitboard::operator ~(void) const
	{
		return ~this->_bitboard;
	}

	inline bitboard& bitboard::operator |=(const bitboard& rhs)
	{
		this->_bitboard |= rhs._bitboard;
		return *this;
	}

	inline bitboard& bitboard::operator &=(const bitboard& rhs)
	{
		this->_bitboard &= rhs._bitboard;
		return *this;
	}

	inline bitboard& bitboard::operator ^=(const bitboard& rhs)
	{
		this->_bitboard ^= rhs._bitboard;
		return *this;
	}

	inline bitboard::operator bool(void) const
	{
		return static_cast<bool>(this->_bitboard);
	}

	inline bitboard operator |(const bitboard& lhs, const bitboard& rhs)
	{
		return lhs._bitboard | rhs._bitboard;
	}

	inline bitboard operator |(uint32_t lhs, const bitboard& rhs)
	{
		return lhs | rhs._bitboard;
	}

	inline bitboard operator |(const bitboard& lhs, uint32_t rhs)
	{
		return lhs._bitboard | rhs;
	}

	inline bitboard operator &(const bitboard& lhs, const bitboard& rhs)
	{
		return lhs._bitboard & rhs._bitboard;
	}

	inline bitboard operator &(uint32_t lhs, const bitboard& rhs)
	{
		return lhs & rhs._bitboard;
	}

	inline bitboard operator &(const bitboard& lhs, uint32_t rhs)
	{
		return lhs._bitboard & rhs;
	}

	inline bitboard operator ^(const bitboard& lhs, const bitboard& rhs)
	{
		return lhs._bitboard ^ rhs._bitboard;
	}

	inline bitboard operator ^(uint32_t lhs, const bitboard& rhs)
	{
		return lhs ^ rhs._bitboard;
	}

	inline bitboard operator ^(const bitboard& lhs, uint32_t rhs)
	{
		return lhs._bitboard & rhs;
	}
}

#endif // __BITBOARD_I_HPP__
// End of file
