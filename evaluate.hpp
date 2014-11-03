/* $Id: evaluate.hpp,v 1.2 2007-11-26 15:20:21 neo Exp $

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
/** @file evaluate.hpp
 *  @brief Artificial intelligence, weight of evaluate strategy.
 */

#ifndef __EVALUATE_HPP__
#define __EVALUATE_HPP__

#include "board.hpp"

namespace checkers
{
	/// Weight of evaluate strategy.
	namespace evaluate
	{
		const int WEIGHT_MAN       = 256;
		const int WEIGHT_KING      = WEIGHT_MAN * 2;
		const int WEIGHT_MOVER     = 2;
		const int WEIGHT_KINGS_ROW = 16;
		const int WEIGHT_EDGE      = 8;

		inline int win(void);
		inline int infinity(void);
		inline int unknown(void);

		int evaluate(const board& board);
		int men(const board& board);
		int kings(const board& board);
		int movers(const board& board);
		int kings_row(const board& board);
		int edges(const board& board);
	}
}

#include "evaluate_i.hpp"
#endif // __EVALUATE_HPP__
// End of file
