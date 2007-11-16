/* $Id: intelligence.hpp,v 1.20 2007-11-16 10:19:37 neo Exp $

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
/** @file intelligence.hpp
 *  @brief
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
		enum verbose
		{
			SILENT  = 0,
			VERBOSE = 1
		};

		static void think(io& io, std::vector<move>& best_moves,
			const board& board, unsigned int depth_limit,
			time_t second, verbose show_detail);

	private:
		inline explicit intelligence(const board& board);

		/** @brief Alpha-beta pruning is a search algorithm that
		 *   reduces the number of nodes that need to be evaluated
		 *   in the search tree by the minimax algorithm.
		 *  @note This is recursive function.
		 */
		int alpha_beta_search(io& io, std::vector<move>& best_moves,
			unsigned int depth, int alpha = -INFINITY,
			int beta = INFINITY, unsigned int ply = 0);

		/// Print think output
		static void show_think(io& io, unsigned int depth, int val,
			struct timeval time, long unsigned int nodes,
			const std::vector<move>& best_moves, bool show_title);

		inline int evaluate(void);
		inline int evaluate_pieces(void);
		inline int evaluate_kings(void);
		inline int evaluate_movers(void);
		inline int evaluate_kings_row(void);
		inline int evaluate_edges(void);

		inline void reorder_moves(std::vector<move>& moves, unsigned int ply);

		inline static void set_timeout(time_t second);
		inline static bool is_timeout(void);

		/// TIMEOUT == -TIMEOUT
		static const int TIMEOUT  = INT_MIN;
		static const int INFINITY = INT_MAX;
		static const int WIN      = 65535;

		board _board;

		static std::vector<move> _best_moves;
		static bool _reorder;

		static long unsigned int _nodes;
		static struct timeval _deadline;
	};
}

#include "intelligence_i.hpp"
#endif // __INTELLIGENCE_HPP_
// End of file
