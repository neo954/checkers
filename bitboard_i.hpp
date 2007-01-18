/** @file bitboard_i.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#ifndef __BITBOARD_I_HPP__
#define __BITBOARD_I_HPP__

namespace checkers
{
	inline bitboard::bitboard(uint32_t x) :
		_bitboard(x)
	{
	}

	inline int bitboard::bit_count(void) const
	{
		uint32_t x = this->_bitboard;

		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0f0f0f0f;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return static_cast<int>(x & 0x0000003f);
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

	inline bitboard::operator uint32_t(void) const
	{
		return static_cast<uint32_t>(this->_bitboard);
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
