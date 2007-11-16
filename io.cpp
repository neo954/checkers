/* $Id: io.cpp,v 1.16 2007-11-16 10:19:37 neo Exp $

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
/** @file io.cpp
 *  @brief
 */

#include <sys/select.h>
#include <stdexcept>
#include "io.hpp"

namespace checkers
{
	io::io(int in_fd, int out_fd) :
		_read_buf(), _write_buf(), _in_fd(in_fd), _out_fd(out_fd)
	{
		// Set stdin and stdout nonblock I/O
		this->setfl(this->_in_fd,  O_NONBLOCK);
		this->setfl(this->_out_fd, O_NONBLOCK);
	}

	io::io(std::pair<int, int> fds) :
		_read_buf(), _write_buf(),
		_in_fd(fds.first), _out_fd(fds.second)
	{
		// Set stdin and stdout nonblock I/O
		this->setfl(this->_in_fd,  O_NONBLOCK);
		this->setfl(this->_out_fd, O_NONBLOCK);
	}

	io::~io(void)
	{
		while (!this->_write_buf.is_empty())
		{
			io::flush(*this);
		}
	}

	io& io::flush(io& io)
	{
		fd_set read_set;
		fd_set write_set;

		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_SET(io._in_fd,  &read_set);
		FD_SET(io._out_fd, &write_set);

		if (select(std::max(io._in_fd, io._out_fd) + 1,
			&read_set, &write_set, NULL, NULL) < 0)
		{
			/// @throw std::runtime_error when select() failed.
			throw std::runtime_error("select() failed");
		}

		if (FD_ISSET(io._in_fd, &read_set))
		{
			io._read_buf.read(io._in_fd);
		}
		if (FD_ISSET(io._out_fd, &write_set))
		{
			io._write_buf.write(io._out_fd);
		}

		return io;
	}

	// ================================================================

	/** @param fd is the open file descriptor
	 *  @param flags are file status flags to turn on
	 */
	void io::setfl(int fd, int flags)
	{
		int val;

		if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		{
			/// @throw std::runtime_error when fcntl() failed.
			throw std::runtime_error("fcntl() failed");
		}

		// Turn on flags
		val |= flags;

		if (fcntl(fd, F_SETFL, val) < 0)
		{
			/// @throw std::runtime_error when fcntl() failed.
			throw std::runtime_error("fcntl() failed");
		}
	}
}

// End of file
