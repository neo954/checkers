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
/** @file intelligence.cpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date $Date: 2007-11-01 16:50:07 $
 *  @version $Revision: 1.16 $
 */

#include <iomanip>
#include "intelligence.hpp"

namespace checkers
{
	/**
	 *  @return TIMEOUT when timeout
	 */
	int intelligence::alpha_beta_search(std::vector<move>& best_moves,
		 int depth, int alpha, int beta, int ply)
	{
		if (0 == static_cast<int16_t>(this->_nodes))
		{
			if (!this->_reorder && this->is_timeout())
			{
				// TIMEOUT == -TIMEOUT
				return TIMEOUT;
			}
		}
		++this->_nodes;

		if (this->_board.is_winning())
		{
			return WIN - ply;
		}
		else if (this->_board.is_losing())
		{
			return -WIN + ply;
		}
		else if (0 >= depth)
		{
			best_moves.clear();
			return evaluate();
		}

		// Generate legal moves
		std::vector<move> legal_moves = this->_board.generate_moves();
		// Re-order the legal moves
		this->reorder_moves(legal_moves, ply);

		int val;
		std::vector<move> moves;

		for (std::vector<move>::const_iterator pos =
			legal_moves.begin(); pos != legal_moves.end(); ++pos)
		{
			// When capture piece in the last ply, search deeper
			if (1 == depth && pos->get_capture())
			{
				++depth;
			}

			intelligence intelligence(*this);
			val = intelligence._board.make_move(*pos) ?
				 intelligence.alpha_beta_search(moves,
					depth,     alpha,   beta, ply + 1) :
				-intelligence.alpha_beta_search(moves,
					depth - 1, -beta, -alpha, ply + 1);

			if (TIMEOUT == val)
			{
				// TIMEOUT == -TIMEOUT
				return TIMEOUT;
			}
			if (val >= beta)
			{
				return beta;
			}
			if (val > alpha)
			{
				alpha = val;
				best_moves.clear();
				best_moves.push_back(*pos);
				best_moves.insert(best_moves.end(),
					moves.begin(), moves.end());
			}
		}

		return alpha;
	}

	std::vector<move> intelligence::think(const board& board,
		int depth_limit, time_t time_limit, io* p_io)
	{
		std::vector<move> best_moves;
		int depth;
		int val;
		struct timeval start;
		struct timeval end;

		intelligence::set_timeout(time_limit);

		for (depth = 1, val = 0;
			depth <= depth_limit && val != TIMEOUT; ++depth)
		{
			best_moves.reserve(depth);

			intelligence::_nodes = 0;
			intelligence::_best_moves = best_moves;
			intelligence::_reorder = true;

			intelligence intelligence(board);
			::gettimeofday(&start, NULL);
			val = intelligence.alpha_beta_search(best_moves, depth);
			::gettimeofday(&end, NULL);

			if (NULL != p_io)
			{
				intelligence::show_think(*p_io, depth, val,
					end - start, intelligence::_nodes,
					best_moves);
			}

			if (best_moves.size() <
				static_cast<unsigned int>(depth))
			{
				break;
			}
		}

		return best_moves;
	}

	// ================================================================

	void intelligence::show_think(io& io, int depth, int val, struct timeval time,
		long int nodes, const std::vector<move>& best_moves)
	{
		std::ostringstream stream;

		if (1 == depth % 10)
		{
			stream << "  depth   value      time       nodes\n";
			stream << "  ----------------------------------------------------------------------------\n";
		}
		stream << "  " << std::setw(5) << depth;
		stream << "  ";
		if (TIMEOUT == val)
		{
			stream << "     -";
		}
		else
		{
			stream << std::setw(6) << val;
		}
		stream << ' ' << std::setw(5) << time.tv_sec << '.' <<
			std::setw(3) << std::setfill('0') <<
			(time.tv_usec / 1000) << std::setfill(' ');
		stream << ' ' << std::setw(11) << nodes;
		stream << ' ';

		std::vector<move>::size_type i;
		std::vector<move>::size_type max_size = best_moves.size();
		for (i = 0; i < max_size; ++i)
		{
			if (i > 0 && 0 == i % 8)
			{
				stream << "\n                                      ";
			}
			stream << ' ' << best_moves[i];
		}
		stream << '\n';

		io << stream.str() << io::nowait << io::flush;
	}

	std::vector<move> intelligence::_best_moves;
	bool intelligence::_reorder = false;
	long int intelligence::_nodes = 0;
	struct timeval intelligence::_deadline = {0, 0};
}

// End of file
