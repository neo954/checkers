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
/** @file loopbuffer.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date $Date: 2007-11-01 16:50:07 $
 *  @version $Revision: 1.8 $
 */

#ifndef __LOOPBUFFER_HPP__
#define __LOOPBUFFER_HPP__

namespace checkers
{
	class loopbuffer
	{
	public:
		explicit inline loopbuffer(int max_size = 512);
		inline ~loopbuffer(void);

		inline int max_size(void) const;
		inline int size(void) const;
		inline bool is_full(void) const;
		inline bool is_empty(void) const;
		char front(void) const;
		void pop_front(void);
		bool read(int fd);
		bool write(int fd);

		void getline(std::string& str);
		void push_back(char c);
		void push_back(const std::string& str);
		void push_back(const char* s);

	private:
		loopbuffer(const loopbuffer& rhs);
		loopbuffer& operator=(const loopbuffer& rhs);

		void enlarge(void);

		char* _buffer;
		int _max_size;
		int _front;
		int _rear;
		int _lines;
	};
}

#include "loopbuffer_i.hpp"
#endif // __LOOPBUFFER_HPP__
// End of file
