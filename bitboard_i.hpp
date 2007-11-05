/* This file is a part of ponder, a English/American checkers game.

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
 *  $Author: neo $
 *  $Date: 2007-11-05 17:39:53 $
 *  $Revision: 1.13 $
 */

#ifndef __BITBOARD_I_HPP__
#define __BITBOARD_I_HPP__

namespace checkers
{
	inline bitboard::bitboard(uint32_t x) :
		_bitboard(x)
	{
	}

	inline unsigned int bitboard::bitcount(void) const
	{
		uint32_t x = this->_bitboard;

		x = x - ((x >> 1) & 0x55555555);
		x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
		x = (x + (x >> 4)) & 0x0f0f0f0f;
		x = x + (x >> 8);
		x = x + (x >> 16);
		return static_cast<int>(x & 0x0000003f);
	}

	inline unsigned int bitboard::ntz(void) const
	{
		uint32_t x = this->_bitboard;

		if (0 == x)
		{
			return 32;
		}

		unsigned int n = 1;

		if (0 == (x & 0x0000ffff))
		{
			n += 16;
			x >>= 16;
		}
		if (0 == (x & 0x000000ff))
		{
			n += 8;
			x >>= 8;
		}
		if (0 == (x & 0x0000000f))
		{
			n += 4;
			x >>= 4;
		}
		if (0 == (x & 0x00000003))
		{
			n += 2;
			x >>= 2;
		}
		return n - (x & 1);
	}

	inline bitboard bitboard::lsb(void) const
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
		return this->_bitboard;
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
