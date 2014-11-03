/* $Id: absearch_i.hpp,v 1.1 2009-08-09 13:32:12 neo Exp $

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007, 2008, 2009 Quux Information.
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
/** @file intelligence_i.hpp
 *  @brief Artificial intelligence, alpha-beta pruning.
 */

#ifndef __ABSEARCH_I_HPP__
#define __ABSEARCH_I_HPP__

#include "evaluate.hpp"

namespace checkers
{
	inline absearch::absearch(const board& board) :
		_board(board)
	{
	}

	// ================================================================

	inline void absearch::set_timeout(time_t second)
	{
		absearch::_deadline = timeval::now() + second;
	}

	inline bool absearch::is_timeout(void)
	{
		return timeval::now() > absearch::_deadline;
	}
}

#endif // __ABSEARCH_I_HPP__
// End of file
