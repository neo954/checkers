/* $Id$

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007 Quux Information.
                     Gong Jie <neo@quux.me>

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
/** @file move_i.hpp
 *  @brief Move.
 */

#ifndef __MOVE_I_HPP__
#define __MOVE_I_HPP__

#include <cassert>
#include <stdexcept>

namespace checkers
{
	inline move::move(bitboard src, bitboard dest, bitboard capture,
		bool will_capture_a_king, bool will_crown) :
		_src(src), _dest(dest), _capture(capture),
		_will_capture_a_king(will_capture_a_king),
		_will_crown(will_crown)
	{
		assert(1 == this->_src.count());
		assert(1 == this->_dest.count());
		assert(!"Error (illegal move, rules violation)" ||
		(0 == this->_capture.count() &&
		(this->_dest ==   (this->_src << 4) ||
		 this->_dest == (((this->_src & bitboard::MASK_L3) << 3) |
				 ((this->_src & bitboard::MASK_L5) << 5)) ||
		 this->_dest ==   (this->_src >> 4) ||
		 this->_dest == (((this->_src & bitboard::MASK_R3) >> 3) |
				 ((this->_src & bitboard::MASK_R5) >> 5)))) ||
		(1 == this->_capture.count() &&
	       ((this->_capture ==    (this->_src << 4) &&
		 this->_dest == (((this->_capture & bitboard::MASK_L3) << 3) |
				 ((this->_capture & bitboard::MASK_L5) << 5))) ||
		(this->_capture ==  (((this->_src & bitboard::MASK_L3) << 3) |
				     ((this->_src & bitboard::MASK_L5) << 5)) &&
		 this->_dest ==   (this->_capture << 4)) ||
		(this->_capture ==    (this->_src >> 4) &&
		 this->_dest == (((this->_capture & bitboard::MASK_R3) >> 3) |
				 ((this->_capture & bitboard::MASK_R5) >> 5))) ||
		(this->_capture ==  (((this->_src & bitboard::MASK_R3) >> 3) |
				     ((this->_src & bitboard::MASK_R5) >> 5)) &&
		 this->_dest ==   (this->_capture >> 4)))));
		assert(!"Error (illegal move, could not capture a king)" ||
			(this->_will_capture_a_king && this->_capture) ||
			!this->_will_capture_a_king);
		assert(!"Error (illegal move, could not crown)" ||
			(this->_will_crown && (this->_dest &
			(bitboard::BLACK_KINGS_ROW | bitboard::WHITE_KINGS_ROW)))
			|| !this->_will_crown);
	}

	inline bitboard move::get_src(void) const
	{
		return this->_src;
	}

	inline bitboard move::get_dest(void) const
	{
		return this->_dest;
	}

	inline bitboard move::get_capture(void) const
	{
		return this->_capture;
	}

	inline bool move::will_capture_a_king(void) const
	{
		return this->_will_capture_a_king;
	}

	inline bool move::will_crown(void) const
	{
		return this->_will_crown;
	}

	inline bool operator ==(const move& lhs, const move& rhs)
	{
		return  lhs._src == rhs._src && lhs._dest == rhs._dest &&
			lhs._capture == rhs._capture &&
			lhs._will_capture_a_king == rhs._will_capture_a_king &&
			lhs._will_crown == rhs._will_crown;
	}

	inline bool operator !=(const move& lhs, const move& rhs)
	{
		return !(lhs == rhs);
	}
}

#endif // __MOVE_I_HPP__
// End of file
