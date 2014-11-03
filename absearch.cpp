/* $Id$

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
/** @file absearch.cpp
 *  @brief Artificial intelligence, alpha-beta pruning.
 */

#include <algorithm>
#include <iomanip>
#include "absearch.hpp"
#include "nonstdio.hpp"

namespace checkers
{
	int absearch::alpha_beta_search(std::vector<move>& best_moves,
		unsigned int depth, int alpha, int beta, unsigned int ply)
	{
		if (0 == this->_nodes % (2 ^ 16))
		{
			nio << io::flush;
			if (this->is_timeout() || nio.lines_to_read() ||
				nio.eof())
			{
	 			/// @retval absearch::unknown() when timeout
				return evaluate::unknown();
			}
		}
		++this->_nodes;

		// The default flag type is ALPHA
		record::hash_flag flag = record::ALPHA;
		// Try to get the evalute record from the hash table
		int val = this->probe_hash(depth, alpha, beta, best_moves);

		if (evaluate::unknown() != val)
		{
			if (evaluate::win() == val)
			{
				return evaluate::win() - ply;
			}
			else if (-evaluate::win() == val)
			{
				return -evaluate::win() + ply;
			}
			else
			{
				return val;
			}
		}
		else if (this->_board.is_winning())
		{
			this->record_hash(depth, evaluate::win(),
				record::EXACT);
			return evaluate::win() - ply;
		}
		else if (this->_board.is_losing())
		{
			this->record_hash(depth, -evaluate::win(),
				record::EXACT);
			return -evaluate::win() + ply;
		}
		else if (0 == depth)
		{
			best_moves.clear();
			val = evaluate::evaluate(this->_board);
			this->record_hash(depth, val, record::EXACT);
			return val;
		}

		// Generate all the legal moves
		std::vector<move> legal_moves = this->_board.generate_moves();
		// Optimize the order of legal moves
		this->optimize_moves(legal_moves, ply);

		std::vector<move> deeper_moves;

		for (std::vector<move>::const_iterator pos =
			legal_moves.begin(); pos != legal_moves.end(); ++pos)
		{
			// While capture piece in the last ply, search deeper
			if (1 == depth && pos->get_capture())
			{
				++depth;
			}

			absearch absearch(*this);
			val = absearch._board.make_move(*pos) ?
				 absearch.alpha_beta_search(deeper_moves,
					depth,     alpha,   beta, ply + 1) :
				-absearch.alpha_beta_search(deeper_moves,
					depth - 1, -beta, -alpha, ply + 1);

			if (evaluate::unknown() == val)
			{
				return val;
			}
			if (val >= beta)
			{
				this->record_hash(depth, beta, record::BETA);
				return beta;
			}
			if (val > alpha)
			{
				flag = record::EXACT;
				alpha = val;
				best_moves.clear();
				best_moves.push_back(*pos);
				best_moves.insert(best_moves.end(),
					deeper_moves.begin(),
					deeper_moves.end());
			}
		}

		if (record::EXACT == flag)
		{
			this->record_hash(depth, alpha, flag, best_moves);
		}
		else
		{
			this->record_hash(depth, alpha, flag);
		}
		return alpha;
	}

	/** @return Timeout or not.
	 */ 
	bool absearch::think(std::vector<move>& best_moves,
		const board& board, unsigned int depth_limit, time_t time_limit,
		bool verbose)
	{
		unsigned int i;
		unsigned int depth;
		int val = 0;
		struct timeval start;
		struct timeval end;

		absearch::set_timeout(time_limit);

		for (i = 0, depth = std::max(best_moves.size(),
			static_cast<std::vector<move>::size_type>(1U)), val = 0;
			depth <= depth_limit && val != evaluate::unknown();
			++i, ++depth)
		{
			absearch::_nodes = 0;
			absearch::_best_moves = best_moves;
			absearch::_optimize_move = true;

			absearch absearch(board);
			start = timeval::now();
			val = absearch.alpha_beta_search(best_moves,
				depth);
			end = timeval::now();

			if (verbose)
			{
				nio << absearch::thinking_detail(depth,
					val, end - start, absearch::_nodes,
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
		return val == evaluate::unknown();
	}

	// ================================================================

	std::string absearch::thinking_detail(unsigned int depth, int val,
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
		if (evaluate::unknown() == val)
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

		// Print out the moves.
		for (std::vector<move>::size_type i = 0;
			i < best_moves.size(); ++i)
		{
			if (i > 0 && 0 == i % 6)
			{
				stream << "\n"
					"                                     ";
			}
			stream << ' ' << best_moves[i];
		}
		stream << '\n';

		return stream.str();
	}

	// ================================================================

	void absearch::optimize_moves(std::vector<move>& moves,
		unsigned int ply)
	{
		if (!this->_optimize_move)
		{
			return;
		}
		if (ply >= this->_best_moves.size())
		{
			this->_optimize_move = false;
			return;
		}

		std::vector<move>::iterator pos = std::find(moves.begin(),
			moves.end(), this->_best_moves[ply]);
		if (moves.end() == pos)
		{
			this->_optimize_move = false;
			return;
		}

		std::swap(moves[0], *pos);
	}

	/** @param depth
	 *  @param alpha
	 *  @param beta
	 *  @param best_moves
	 *  @return value found in the hash table.
	 */ 
	int absearch::probe_hash(unsigned int depth, int alpha, int beta,
		std::vector<move>& best_moves) const
	{
		std::vector<record>::iterator pos = absearch::_hash.begin()
			+ (this->_board.get_zobrist().key()
			% absearch::hash_size);

		if (pos->get_zobrist() == this->_board.get_zobrist())
		{
			return pos->get_val(depth, alpha, beta, best_moves);
		}
		/** @retval evaluate::unknown() while an effective value is not
		 *   found in the hash table.
		 */
		return evaluate::unknown();
	}

	void absearch::record_hash(unsigned int depth, int val,
		record::hash_flag flag)
	{
		std::vector<record>::iterator pos = absearch::_hash.begin()
			+ (this->_board.get_zobrist().key()
			% absearch::hash_size);

		*pos = record(this->_board.get_zobrist(), depth, val, flag);
	}

	void absearch::record_hash(unsigned int depth, int val,
		record::hash_flag flag, const std::vector<move>& best_moves)
	{
		std::vector<record>::iterator pos = absearch::_hash.begin()
			+ (this->_board.get_zobrist().key()
			% absearch::hash_size);

		*pos = record(this->_board.get_zobrist(), depth, val, flag,
			best_moves);
	}

	std::vector<move> absearch::_best_moves;
	bool absearch::_optimize_move = false;
	long unsigned int absearch::_nodes = 0;
	struct timeval absearch::_deadline = { 0, 0 };
	std::vector<record> absearch::_hash(absearch::hash_size);
}

// End of file
