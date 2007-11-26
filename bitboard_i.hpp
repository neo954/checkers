/* $Id: bitboard_i.hpp,v 1.20 2007-11-26 15:20:21 neo Exp $

   This file is a part of ponder, a English/American checkers game.

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
 *  @brief Bitboard.
 */

#ifndef __BITBOARD_I_HPP__
#define __BITBOARD_I_HPP__

namespace checkers
{
	inline bitboard::bitboard(uint32_t x) :
		_bitboard(x)
	{
	}

	inline bitboard bitboard::lsb(void) const
	{
		return bitboard(this->_bitboard & (-this->_bitboard));
	}

	inline bitboard bitboard::operator <<(int rhs) const
	{
		return bitboard(this->_bitboard << rhs);
	}

	inline bitboard bitboard::operator >>(int rhs) const
	{
		return bitboard(this->_bitboard >> rhs);
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
		return bitboard(~this->_bitboard);
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
		return this->_bitboard;
	}

	inline bitboard operator |(const bitboard& lhs, const bitboard& rhs)
	{
		return bitboard(lhs._bitboard | rhs._bitboard);
	}

	inline bitboard operator |(uint32_t lhs, const bitboard& rhs)
	{
		return bitboard(lhs | rhs._bitboard);
	}

	inline bitboard operator |(const bitboard& lhs, uint32_t rhs)
	{
		return bitboard(lhs._bitboard | rhs);
	}

	inline bitboard operator &(const bitboard& lhs, const bitboard& rhs)
	{
		return bitboard(lhs._bitboard & rhs._bitboard);
	}

	inline bitboard operator &(uint32_t lhs, const bitboard& rhs)
	{
		return bitboard(lhs & rhs._bitboard);
	}

	inline bitboard operator &(const bitboard& lhs, uint32_t rhs)
	{
		return bitboard(lhs._bitboard & rhs);
	}

	inline bitboard operator ^(const bitboard& lhs, const bitboard& rhs)
	{
		return bitboard(lhs._bitboard ^ rhs._bitboard);
	}

	inline bitboard operator ^(uint32_t lhs, const bitboard& rhs)
	{
		return bitboard(lhs ^ rhs._bitboard);
	}

	inline bitboard operator ^(const bitboard& lhs, uint32_t rhs)
	{
		return bitboard(lhs._bitboard ^ rhs);
	}

	inline bool operator ==(const bitboard& lhs, const bitboard& rhs)
	{
		return lhs._bitboard == rhs._bitboard;
	}

	inline bool operator !=(const bitboard& lhs, const bitboard& rhs)
	{
		return !(lhs == rhs);
	}
}

#endif // __BITBOARD_I_HPP__
// End of file
