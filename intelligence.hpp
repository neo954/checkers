/* $Id: intelligence.hpp,v 1.25 2007-11-26 06:49:05 neo Exp $

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
 *  @brief Artificial intelligence, alpha-beta pruning.
 */

#ifndef __INTELLIGENCE_HPP__
#define __INTELLIGENCE_HPP__

#include "board.hpp"
#include "io.hpp"
#include "record.hpp"
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

		static bool think(io& io, std::vector<move>& best_moves,
			const board& board, unsigned int depth_limit,
			time_t second, verbose show_detail);

		static const unsigned int hash_size = 1024 * 1024;

	private:
		inline explicit intelligence(const board& board);

		/** @brief Alpha-beta pruning is a search algorithm that
		 *   reduces the number of nodes that need to be evaluated
		 *   in the search tree by the minimax algorithm.
		 *  @note This is recursive function.
		 */
		int alpha_beta_search(io& io, std::vector<move>& best_moves,
			unsigned int depth,
			int alpha = -evaluate::infinity(),
			int beta = evaluate::infinity(),
			unsigned int ply = 0);

		/// Print the detail information of thinking.
		static void show_think(io& io, unsigned int depth, int val,
			struct timeval time, long unsigned int nodes,
			const std::vector<move>& best_moves, bool show_title);

		inline int evaluate(void);
		inline int evaluate_pieces(void);
		inline int evaluate_kings(void);
		inline int evaluate_movers(void);
		inline int evaluate_kings_row(void);
		inline int evaluate_edges(void);

		inline void optimize_moves(std::vector<move>& moves,
			unsigned int ply);

		inline static void set_timeout(time_t second);
		inline static bool is_timeout(void);

		int probe_hash(unsigned int depth, int alpha, int beta,
			std::vector<move>& best_moves) const;
		void record_hash(unsigned int depth, int val,
			record::hash_flag flag);
		void record_hash(unsigned int depth, int val,
			record::hash_flag flag,
			const std::vector<move>& best_moves);

		board _board;

		static std::vector<move> _best_moves;
		static bool _optimize_move;

		static long unsigned int _nodes;
		static struct timeval _deadline;

		static std::vector<record> _hash;
	};
}

#include "intelligence_i.hpp"
#endif // __INTELLIGENCE_HPP__
// End of file
