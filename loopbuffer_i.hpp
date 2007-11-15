/* $Id: loopbuffer_i.hpp,v 1.10 2007-11-15 17:41:45 neo Exp $

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
/** @file loopbuffer_i.hpp
 *  @brief
 */

#ifndef __LOOPBUFFER_I_HPP__
#define __LOOPBUFFER_I_HPP__

namespace checkers
{
	inline loopbuffer::loopbuffer(int max_size) :
		_max_size(max_size), _front(max_size - 2),
		_rear(max_size - 2), _lines(0), _eof(false)
	{
		this->_buffer = new char[this->_max_size];
	}

	inline loopbuffer::~loopbuffer(void)
	{
		delete[] this->_buffer;
	}

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
