/* $Id: move_i.hpp,v 1.18 2007-11-15 17:41:45 neo Exp $

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
/** @file move_i.hpp
 *  @brief
 */

#ifndef __MOVE_I_HPP__
#define __MOVE_I_HPP__

#include <cassert>
#include <stdexcept>

namespace checkers
{
	inline move::move(bitboard orig, bitboard dest, bitboard capture,
		bool will_capture_a_king, bool will_crown) :
		_orig(orig), _dest(dest), _capture(capture),
		_will_capture_a_king(will_capture_a_king),
		_will_crown(will_crown)
	{
		assert(1 == this->_orig.bitcount());
		assert(1 == this->_dest.bitcount());
		assert(this->_capture.bitcount() <= 1);
	}

	inline bitboard move::get_orig(void) const
	{
		return this->_orig;
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
		return  lhs._orig == rhs._orig && lhs._dest == rhs._dest &&
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
