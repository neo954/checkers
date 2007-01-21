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
/** @file io.cpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-01-21 01:40:41 $
 *  $Revision: 1.8 $
 */

#include <sys/select.h>
#include <stdexcept>
#include "io.hpp"

namespace checkers
{
	io& cio = io::init();

	io::io(void) :
		_read_buf(), _write_buf(), _wait(true)
	{
		// Set stdin and stdout nonblock I/O
		this->setfl(STDIN_FILENO,  O_NONBLOCK);
		this->setfl(STDOUT_FILENO, O_NONBLOCK);
	}

	io::~io(void)
	{
		while (!this->_write_buf.is_empty())
		{
			*this << io::flush;
		}
	}

	io& io::flush(io& io)
	{
		const unsigned long int wait = 10000;
		fd_set read_set;
		fd_set write_set;
		int n;

		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_SET(STDIN_FILENO,  &read_set);
		FD_SET(STDOUT_FILENO, &write_set);

		if (io._wait)
		{
			usleep(wait);
		}
		n = select(std::max(STDIN_FILENO, STDOUT_FILENO) + 1,
			&read_set, &write_set, NULL, NULL);
		if (n < 0)
		{
			throw std::runtime_error("System call select() failed");
		}

		if (FD_ISSET(STDIN_FILENO, &read_set))
		{
			io._read_buf.read(STDIN_FILENO);
		}
		if (FD_ISSET(STDOUT_FILENO, &write_set))
		{
			io._write_buf.write(STDOUT_FILENO);
		}

		return io;
	}

	// ================================================================

	/**
	 *  @param fd is the open file descriptor
	 *  @param flags are file status flags to turn on
	 */
	void io::setfl(int fd, int flags)
	{
		int val;

		if ((val = fcntl(fd, F_GETFL, 0)) < 0)
		{
			throw std::runtime_error("System call fcntl(..., F_GETFL, ...) failed");
		}

		// Turn on flags
		val |= flags;

		if (fcntl(fd, F_SETFL, val) < 0)
		{
			throw std::runtime_error("System call fcntl(..., F_SETFL, ...) failed");
		}
	}
}

// End of file
