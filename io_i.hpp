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
/** @file io_i.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date $Date: 2007-11-01 16:50:07 $
 *  @version $Revision: 1.9 $
 */

#ifndef __IO_I_HPP__
#define __IO_I_HPP__

#include <sstream>

namespace checkers
{
	template<typename T>
	io& io::operator <<(const T& rhs)
	{
		std::ostringstream stream;
		stream.setf(std::ios::fixed);
		stream << rhs;
		this->_write_buf.push_back(stream.str());
		return *this;
	}

	inline io& io::operator <<(char rhs)
	{
		this->_write_buf.push_back(rhs);
		return *this;
	}

	inline io& io::operator <<(const std::string& rhs)
	{
		this->_write_buf.push_back(rhs);
		return *this;
	}

	inline io& io::operator <<(const char* rhs)
	{
		this->_write_buf.push_back(rhs);
		return *this;
	}

	inline io& io::operator <<(io& (*op)(io&))
	{
		return (*op)(*this);
	}

	inline void io::getline(std::string& str)
	{
		this->_read_buf.getline(str);
	}

	inline bool io::state(void) const
	{
		return this->_state;
	}

	inline io& io::wait(io& io)
	{
		io._wait = true;
		return io;
	}

	inline io& io::nowait(io& io)
	{
		io._wait = false;
		return io;
	}

	inline io& io::endl(io& io)
	{
		io << '\n' << io::flush;
		return io;
	}
}

#endif // __IO_I_HPP
// End of file
