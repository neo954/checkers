/* $Id: loopbuffer.hpp,v 1.15 2010-11-19 08:26:26 neo Exp $

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
/** @file loopbuffer.hpp
 *  @brief Loop buffer.
 */

#ifndef __LOOPBUFFER_HPP__
#define __LOOPBUFFER_HPP__

namespace checkers
{
	class loopbuffer
	{
	public:
		explicit loopbuffer(unsigned int max_size = 512);
		~loopbuffer(void);

		inline int max_size(void) const;
		inline bool is_full(void) const;
		inline bool is_empty(void) const;
		inline int lines(void) const;
		inline bool eof(void) const;

		char front(void) const;
		void pop_front(void);
		/// Read from a file descriptor.
		void read(int fd);
		/// Write to a file descriptor.
		void write(int fd);

		std::string getline(void);
		void push_back(char c);
		void push_back(const std::string& str);
		void push_back(const char* s);

	private:
		/// Define but not implement, to prevent object copy.
		loopbuffer(const loopbuffer& rhs);
		/// Define but not implement, to prevent object copy.
		loopbuffer& operator=(const loopbuffer& rhs);

		void enlarge(void);

		char* _buffer;
		unsigned int _max_size;
		unsigned int _front;
		unsigned int _rear;
		unsigned int _lines;
		/// Reach the end-of-file or not.
		bool _eof;
	};
}

#include "loopbuffer_i.hpp"
#endif // __LOOPBUFFER_HPP__
// End of file
