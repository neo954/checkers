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
/** @file intelligence.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-01-21 01:40:41 $
 *  $Revision: 1.12 $
 */

#ifndef __INTELLIGENCE_HPP_
#define __INTELLIGENCE_HPP_

#include "board.hpp"
#include "io.hpp"
#include "timeval.hpp"

namespace checkers
{
	class intelligence
	{
	public:
		inline explicit intelligence(const board& board);

		int alpha_beta_search(std::vector<move>& best_moves,
			int depth, int alpha = -INFINITY, int beta = INFINITY,
			int ply = 0);
		static std::vector<move> think(const board& board,
			int depth_limit, time_t second);

	private:
		/// Print think output
		static void print(int depth, int val, struct timeval time,
			long int nodes, const std::vector<move>& best_moves);

		inline int evaluate(void);
		inline int evaluate_pieces_strength(void);
		inline int evaluate_movers(void);
		inline int evaluate_kings_row(void);
		inline int evaluate_edges(void);

		inline void reorder_moves(std::vector<move>& moves, int ply);

		inline static void set_timeout(time_t second);
		inline static bool is_timeout(void);

		static const int TIMEOUT  = INT_MIN;
		static const int INFINITY = INT_MAX;
		static const int WIN      = 65535;

		board _board;

		static std::vector<move> _best_moves;
		static bool _reorder;

		static long int _nodes;
		static struct timeval _deadline;
	};
}

#include "intelligence_i.hpp"
#endif // __INTELLIGENCE_HPP_
// End of file
