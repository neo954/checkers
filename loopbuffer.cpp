/* $Id: loopbuffer.cpp,v 1.21 2010-11-19 08:26:25 neo Exp $

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
/** @file loopbuffer.cpp
 *  @brief Loop buffer.
 */

extern "C"
{
	#include <unistd.h>
}
#include <cerrno>
#include <cassert>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include "loopbuffer.hpp"

namespace checkers
{
	loopbuffer::loopbuffer(unsigned int max_size) :
		_max_size(max_size), _front(max_size - 2),
		_rear(max_size - 2), _lines(0), _eof(false)
	{
		this->_buffer = new char[this->_max_size];
	}

	loopbuffer::~loopbuffer(void)
	{
		delete[] this->_buffer;
	}

	char loopbuffer::front(void) const
	{
		assert(!this->is_empty());		

		return this->_buffer[this->_front];
	}

	void loopbuffer::pop_front(void)
	{
		assert(!this->is_empty());		

		if ('\n' == this->front())
		{
			--this->_lines;
		}
		this->_front = (this->_front + 1) % this->_max_size;
	}

	void loopbuffer::read(int fd)
	{
		char c;
		ssize_t n;

		for (;;)
		{
			n = ::read(fd, &c, 1);
			if (n < 0)
			{
				if (EAGAIN == errno || EWOULDBLOCK == errno)
				{
					break;
				}
				std::ostringstream error;
				error << "read() failed while read from fd - "
					<< fd;
				/** @throw std::runtime_error when read()
				 *   failed.
				 */
				throw std::runtime_error(error.str());
			}
			else if (0 == n)
			{
				this->_eof = true;
				break;
			}
			else
			{
				this->push_back(c);
			}
		}
	}

	void loopbuffer::write(int fd)
	{
		char c;
		ssize_t n;
		while (!this->is_empty())
		{
			c = this->front();
			n = ::write(fd, &c, 1);
			if (n < 0)
			{
				if (EAGAIN == errno || EWOULDBLOCK == errno)
				{
					break;
				}
				std::ostringstream error;
				error << "write() failed while write to fd - "
					<< fd;
				/** @throw std::runtime_error when write()
				 *   failed.
				 */
				throw std::runtime_error(error.str());
			}
			else if (0 == n)
			{
				std::ostringstream error;
				error << "write() returns 0 while write to fd"
					" - " << fd;
				/** @throw std::runtime_error when write()
				 *   returns 0.  This should not happen.
				 */
				throw std::runtime_error(error.str());
			}
			else
			{
				this->pop_front();
			}
		}
	}

	/** @return a entire line from the loop buffer, include the newline
	 *   character.  A empty string will be returned if no new line
	 *   available.
	 */
	std::string loopbuffer::getline(void)
	{
		char c;
		std::string line;

		if (this->_lines)
		{
			do
			{
				c = this->front();
				/** @warning This function is not
				 *   exception-safe, and may cause data lose if
				 *   std::string::operator +=(char) fail and
				 *   throw an exception.
				 */
				line += c;
				this->pop_front();
			} while ('\n' != c);
		}

		return line;
	}

	void loopbuffer::push_back(char c)
	{
		if (this->is_full())
		{
			this->enlarge();
		}

		if ('\n' == c)
		{
			++this->_lines;
		}
		this->_buffer[this->_rear] = c;
		this->_rear = (this->_rear + 1) % this->_max_size;
	}

	void loopbuffer::push_back(const std::string& str)
	{
		for (std::string::const_iterator pos = str.begin();
			pos != str.end(); ++pos)
		{
			this->push_back(*pos);
		}	
	}

	void loopbuffer::push_back(const char* s)
	{
		for (; *s != '\0'; ++s)
		{
			this->push_back(*s);
		}
	}

	// ================================================================

	void loopbuffer::enlarge(void)
	{
		assert(this->is_full());

		const int multiple = 2;
		char* buffer = new char[this->_max_size * multiple];

		if (this->_front < this->_rear)
		{
			std::memcpy(buffer, this->_buffer + this->_front,
				this->_rear - this->_front);
		}
		else
		{
			std::memcpy(buffer, this->_buffer + this->_front,
				this->_max_size - this->_front);
			std::memcpy(buffer + this->_max_size - this->_front,
				this->_buffer, this->_rear);
		}

		delete[] this->_buffer;
		this->_buffer = buffer;
		this->_front = 0;
		this->_rear = this->_max_size - 1;
		this->_max_size *= multiple;
	}
}

// End of file
