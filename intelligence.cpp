/// @file intelligence.cpp

#include "intelligence.hpp"

namespace checkers
{
	int intelligence::alpha_beta_search(std::vector<move>& best_moves,
		 int depth, int alpha, int beta, int ply)
	{
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
			if (1 == depth && pos->is_jump())
			{
				++depth;
			}

			intelligence intelligence(*this);
			val = intelligence._board.make_move(*pos) ?
				 intelligence.alpha_beta_search(moves,
					depth,     alpha,   beta, ply + 1) :
				-intelligence.alpha_beta_search(moves,
					depth - 1, -beta, -alpha, ply + 1);

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

	// ================================================================

	std::vector<move> intelligence::_best_moves;
	bool intelligence::_reorder = false;
	int intelligence::_nodes = 0;
}

// End of file
