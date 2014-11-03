/* $Id: io.hpp,v 1.19 2010-11-19 08:26:25 neo Exp $

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
/** @file io.hpp
 *  @brief Synchronous input/output multiplexing.
 */

#ifndef __IO_HPP__
#define __IO_HPP__

extern "C"
{
	#include <fcntl.h>
}
#include <string>
#include "loopbuffer.hpp"

namespace checkers
{
	class io
	{
	public:
		io(int in_fd, int out_fd);
		explicit io(std::pair<int, int> fds);
		~io(void);

		template<typename T>
		io& operator <<(const T& rhs);
		io& operator <<(char rhs);
		io& operator <<(const std::string& rhs);
		io& operator <<(const char* rhs);
		io& operator <<(io& (*op)(io&));

		/// Get a new line from read buffer
		io& operator >>(std::string& rhs);

		inline int lines_to_read(void);
		inline bool eof(void) const;

		static io& flush(io& io);
		static inline io& endl(io& io);

	private:
		/// Define but not implement, to prevent object copy.
		io(const io& rhs);
		/// Define but not implement, to prevent object copy.
		io& operator=(const io& rhs) const;

		loopbuffer _read_buf;
		loopbuffer _write_buf;
		int _in_fd;
		int _out_fd;

		void setfl(int fd, int flags);
	};
}

#include "io_i.hpp"
#endif // __IO_HPP__
// End of file
