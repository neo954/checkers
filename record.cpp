/* $Id: record.cpp,v 1.1 2007-11-26 06:49:05 neo Exp $

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
/** @file record.cpp
 *  @brief Record for alpha-beta search.
 */

#include "record.hpp"

namespace checkers
{
	int record::get_val(zobrist zobrist, unsigned int depth, int alpha,
			int beta, std::vector<move>& best_moves) const
	{
		if (this->_zobrist == zobrist)
		{
			if (this->_depth >= depth)
			{
				if (EXACT == this->_flag)
				{
					best_moves = this->_best_moves;
					return this->_val;
				}
				if (ALPHA == this->_flag && this->_val <= alpha)
				{
					best_moves = this->_best_moves;
					return alpha;
				}
				if (BETA  == this->_flag && this->_val >= beta)
				{
					best_moves = this->_best_moves;
					return beta;
				}
			}
		}
		/** @retval evaluate::unknown() while an effective value is not
		 *   found in the hash table.
		 */
		return evaluate::unknown();
	}
}

// End of file
