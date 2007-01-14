// @file intelligence.cpp

#include "io.hpp"
#include "intelligence.hpp"

namespace checkers
{
	int intelligence::alpha_beta_search(std::vector<move>& best_moves,
		 int depth, int alpha, int beta, int ply)
	{
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
		int val;
		std::vector<move> moves;

		for (std::vector<move>::const_iterator pos =
			legal_moves.begin(); pos != legal_moves.end(); ++pos)
		{
			intelligence intelligence(*this);

			val = intelligence._board.make_move(*pos) ?
				 intelligence.alpha_beta_search(moves,
					depth - 1, alpha,   beta, ply + 1) :
				-intelligence.alpha_beta_search(moves,
					depth - 1, -beta, -alpha, ply + 1);

			if (val >= beta) {
				return beta;
			}
			if (val > alpha) {
				alpha = val;
				best_moves.clear();
				best_moves.push_back(*pos);
				best_moves.insert(best_moves.end(),
					moves.begin(), moves.end());
			}
		}

		return alpha;
	}
}

// End of file
