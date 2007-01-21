/* This file is a part of textual checkers, a English/American checkers
   game.

   Copyright (c) 2006, 2007 Mamiyami Information.
                     Gong Jie <neo@mamiyami.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
 */
/** @file bitboard_i.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-01-21 01:40:41 $
 *  $Revision: 1.8 $
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
		return lhs._bitboard ^ rhs;
	}
}

#endif // __BITBOARD_I_HPP__
// End of file
