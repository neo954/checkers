/* $Id: move.cpp,v 1.19 2007-11-28 17:17:21 neo Exp $

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
/** @file move.cpp
 *  @brief Move.
 */

#include <cassert>
#include <ostream>
#include "move.hpp"

namespace checkers
{
	std::ostream& operator <<(std::ostream& os, const move& rhs)
	{
		assert(1 == rhs.get_src().count());
		assert(1 == rhs.get_dest().count());

		os << rhs.get_src() << (rhs.get_capture() ? 'x' : '-')
			<< rhs.get_dest();

		return os;
	}

	std::ostream& operator <<(std::ostream& os,
		const std::vector<move>& rhs)
	{
		for (std::vector<move>::const_iterator pos = rhs.begin();
			pos != rhs.end(); ++pos)
		{
			if (pos != rhs.begin())
			{
				if (pos->get_src() == (pos - 1)->get_dest())
				{
					assert((pos - 1)->get_capture());
					assert(pos->get_capture());
					os << 'x' << pos->get_dest();
					continue;
				}
				else
				{
					os << ' ';
				}
			}
			os << *pos;
		}

		return os;
	}
}

// End of file
