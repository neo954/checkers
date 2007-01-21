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
/** @file io.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-01-21 01:40:41 $
 *  $Revision: 1.8 $
 */

#ifndef __IO_HPP__
#define __IO_HPP__

#include <fcntl.h>
#include <string>
#include "loopbuffer.hpp"

namespace checkers
{
	class io
	{
	public:
                inline static io& init(void);

		template<typename T>
		io& operator <<(const T& rhs);
		inline io& operator <<(char rhs);
		inline io& operator <<(const std::string& rhs);
		inline io& operator <<(const char* rhs);
		inline io& operator <<(io& (*op)(io&));

		inline io& get_line(std::string& str);

		static io& wait(io& io);
		static io& nowait(io& io);
		static io& flush(io& io);
		static io& endl(io& io);

	private:
		io(void);
		~io(void);
		/// Define but not implement, to prevent object copy.
		io(const io& rhs);
		/// Define but not implement, to prevent object copy.
		io& operator=(const io& rhs) const;

		loopbuffer _read_buf;
		loopbuffer _write_buf;
		bool _wait;

		void setfl(int fd, int flags);
	};

	extern io& cio;
}

#include "io_i.hpp"
#endif // __IO_HPP__
// End of file
