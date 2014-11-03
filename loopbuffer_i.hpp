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
/** @file loopbuffer_i.hpp
 *  @brief Loop buffer.
 */

#ifndef __LOOPBUFFER_I_HPP__
#define __LOOPBUFFER_I_HPP__

namespace checkers
{
	inline int loopbuffer::max_size(void) const
	{
		return this->_max_size;
	}

	inline bool loopbuffer::is_full(void) const
	{
		return (this->_rear + 1) % this->_max_size == this->_front;
	}

	inline bool loopbuffer::is_empty(void) const
	{
		return this->_front == this->_rear;
	}

	inline int loopbuffer::lines(void) const
	{
		return this->_lines;
	}

	inline bool loopbuffer::eof(void) const
	{
		return this->_eof;
	}
}

#endif // __LOOPBUFFER_I_HPP__
// End of file
