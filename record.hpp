/* $Id: record.hpp,v 1.2 2007-11-26 08:13:14 neo Exp $

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
/** @file record.hpp
 *  @brief Record for alpha-beta search.
 */

#ifndef __RECORD_HPP_
#define __RECORD_HPP_

#include <vector>
#include "evaluate.hpp"
#include "move.hpp"
#include "zobrist.hpp"

namespace checkers
{
	class record
	{
	public:
		enum hash_flag
		{
			/// The value of the node is exactly the value saved.
			EXACT = 0,
			/// The value of the node is at most the value saved.
			ALPHA,
			/// The value of the node is at least the value saved.
			BETA
		};

		inline record(void);
		inline record(zobrist zobrist, unsigned int depth, int val,
			hash_flag flag);
		inline record(zobrist zobrist, unsigned int depth, int val,
			hash_flag flag, const std::vector<move>& best_moves);

		inline zobrist get_zobrist(void) const;
		int get_val(unsigned int depth, int alpha, int beta,
			std::vector<move>& best_moves) const;

	private:
		zobrist _zobrist;
		unsigned int _depth;
		int _val;
		hash_flag _flag;
		std::vector<move> _best_moves;
	};
}

#include "record_i.hpp"
#endif // __RECORD_HPP__
// End of file
