/* $Id: intelligence_i.hpp,v 1.23 2007-11-26 15:20:21 neo Exp $

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
/** @file intelligence_i.hpp
 *  @brief Artificial intelligence, alpha-beta pruning.
 */

#ifndef __INTELLIGENCE_I_HPP__
#define __INTELLIGENCE_I_HPP__

#include "evaluate.hpp"

namespace checkers
{
	inline intelligence::intelligence(const board& board) :
		_board(board)
	{
	}

	// ================================================================

	inline void intelligence::set_timeout(time_t second)
	{
		intelligence::_deadline = timeval::now() + second;
	}

	inline bool intelligence::is_timeout(void)
	{
		return timeval::now() > intelligence::_deadline;
	}
}

#endif // __INTELLIGENCE_I_HPP__
// End of file
