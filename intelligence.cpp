/* $Id: intelligence.cpp,v 1.30 2007-11-25 18:59:18 neo Exp $

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
/** @file intelligence.cpp
 *  @brief Artificial intelligence, alpha-beta pruning.
 */

#include <iomanip>
#include "intelligence.hpp"

namespace checkers
{
	int intelligence::alpha_beta_search(io& io,
		std::vector<move>& best_moves, unsigned int depth, int alpha,
		int beta, unsigned int ply)
	{
		if (0 == this->_nodes % 2 ^ 16)
		{
			io << io::flush;
			if (this->is_timeout() || io.lines_to_read() ||
				io.eof())
			{
	 			/// @retval intelligence::unknown() when timeout
				return intelligence::unknown();
			}
		}
		++this->_nodes;

		hash_flag flag = ALPHA;
		int val = this->probe_hash(depth, alpha, beta, best_moves);

		if (intelligence::unknown() != val)
		{
			if (intelligence::win() == val)
			{
				return intelligence::win() - ply;
			}
			else if (-intelligence::win() == val)
			{
				return -intelligence::win() + ply;
			}
			else
			{
				return val;
			}
		}
		else if (this->_board.is_winning())
		{
			this->record_hash(depth, intelligence::win(), EXACT);
			return intelligence::win() - ply;
		}
		else if (this->_board.is_losing())
		{
			this->record_hash(depth, -intelligence::win(), EXACT);
			return -intelligence::win() + ply;
		}
		else if (0 == depth)
		{
			best_moves.clear();
			val = this->evaluate();
			this->record_hash(depth, val, EXACT);
			return val;
		}

		// Generate legal moves
		std::vector<move> legal_moves = this->_board.generate_moves();
		// Optimize the order of legal moves
		this->optimize_moves(legal_moves, ply);

		std::vector<move> moves;

		for (std::vector<move>::const_iterator pos =
			legal_moves.begin(); pos != legal_moves.end(); ++pos)
		{
			// While capture piece in the last ply, search deeper
			if (1 == depth && pos->get_capture())
			{
				++depth;
			}

			intelligence intelligence(*this);
			val = intelligence._board.make_move(*pos) ?
				 intelligence.alpha_beta_search(io, moves,
					depth,     alpha,   beta, ply + 1) :
				-intelligence.alpha_beta_search(io, moves,
					depth - 1, -beta, -alpha, ply + 1);

			if (intelligence::unknown() == val)
			{
				return val;
			}
			if (val >= beta)
			{
				this->record_hash(depth, beta, BETA);
				return beta;
			}
			if (val > alpha)
			{
				flag = EXACT;
				alpha = val;
				best_moves = moves;
				best_moves.push_back(*pos);
			}
		}

		this->record_hash(depth, alpha, flag, best_moves);
		return alpha;
	}

	/** @return Timeout or not.
	 */ 
	bool intelligence::think(io& io, std::vector<move>& best_moves,
		const board& board, unsigned int depth_limit, time_t time_limit,
		verbose show_detail)
	{
		unsigned int i;
		unsigned int depth;
		int val = 0;
		struct timeval start;
		struct timeval end;

		intelligence::set_timeout(time_limit);

		for (i = 0, depth = std::max(best_moves.size(),
			static_cast<std::vector<move>::size_type>(1U)), val = 0;
			depth <= depth_limit && val != intelligence::unknown();
			++i, ++depth)
		{
			intelligence::_nodes = 0;
			intelligence::_best_moves = best_moves;
			intelligence::_optimize_move = true;

			intelligence intelligence(board);
			start = timeval::now();
			val = intelligence.alpha_beta_search(io, best_moves,
				depth);
			end = timeval::now();

			if (intelligence::VERBOSE == show_detail)
			{
				intelligence::show_think(io, depth, val,
					end - start, intelligence::_nodes,
					best_moves, !(i % 8));
			}

			if (best_moves.size() < depth)
			{
				break;
			}
		}

		/** @retval true while timeout.
		 *  @retval false while reach specified search depth or game
		 *   end.
		 */
		return val == intelligence::unknown();
	}

	// ================================================================

	void intelligence::show_think(io& io, unsigned int depth, int val,
		struct timeval time, long unsigned int nodes,
		const std::vector<move>& best_moves, bool show_title)
	{
		std::ostringstream stream;

		if (show_title)
		{
			stream <<
				"  depth   value      time       nodes\n"
				"  ------------------------------------------"
					"----------------------------------\n";
		}
		stream << "  " << std::setw(5) << depth;
		stream << "  ";
		if (intelligence::unknown() == val)
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

		unsigned int i;
		std::vector<move>::const_reverse_iterator pos;
		for (pos = best_moves.rbegin(), i = 0;
			pos != best_moves.rend(); ++pos, ++i)
		{
			if (i && 0 == i % 8)
			{
				stream << "\n"
				"                                      ";
			}
			stream << ' ' << *pos;
		}
		stream << '\n';

		io << stream.str() << io::flush;
	}

	// ================================================================

	int intelligence::probe_hash(unsigned int depth, int alpha, int beta,
		std::vector<move>& best_moves) const
	{
		std::vector<intelligence::record>::iterator pos = 
			intelligence::_hash.begin() +
			(this->_board.get_zobrist().key() %
			intelligence::hash_size);
		if (pos->key == this->_board.get_zobrist())
		{
			if (pos->depth >= depth)
			{
				if (EXACT == pos->flag)
				{
					best_moves = pos->best_moves;
					return pos->val;
				}
				if (ALPHA == pos->flag && pos->val <= alpha)
				{
					best_moves = pos->best_moves;
					return alpha;
				}
				if (BETA  == pos->flag && pos->val >= beta)
				{
					best_moves = pos->best_moves;
					return beta;
				}
			}
		}
		return intelligence::unknown();
	}

	void intelligence::record_hash(unsigned int depth, int val,
		hash_flag flag)
	{
		std::vector<intelligence::record>::iterator pos = 
			intelligence::_hash.begin() +
			(this->_board.get_zobrist().key() %
			intelligence::hash_size);

		pos->key = this->_board.get_zobrist();
		pos->depth = depth;
		pos->flag = flag;
		pos->val = val;
		pos->best_moves.clear();
	}

	void intelligence::record_hash(unsigned int depth, int val,
		hash_flag flag, const std::vector<move>& best_moves)
	{
		std::vector<intelligence::record>::iterator pos = 
			intelligence::_hash.begin() +
			(this->_board.get_zobrist().key() %
			intelligence::hash_size);

		pos->key = this->_board.get_zobrist();
		pos->depth = depth;
		pos->flag = flag;
		pos->val = val;
		pos->best_moves = best_moves;
	}

	std::vector<move> intelligence::_best_moves;
	bool intelligence::_optimize_move = false;
	long unsigned int intelligence::_nodes = 0;
	struct timeval intelligence::_deadline = { 0, 0 };
	std::vector<intelligence::record> intelligence::_hash(
		intelligence::hash_size);
}

// End of file
