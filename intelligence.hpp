/// @file intelligence.hpp

#ifndef __INTELLIGENCE_HPP_
#define __INTELLIGENCE_HPP_

#include "board.hpp"
#include "move.hpp"

namespace checkers
{
	class intelligence
	{
	public:
		intelligence(const board& board);

		int alpha_beta_search(std::vector<move>& best_moves,
			int depth, int alpha = -INFINITY, int beta = INFINITY,
			int ply = 0);

	private:
		int evaluate(void);
		int evaluate_pieces_strength(void);
		int evaluate_movers(void);
		int evaluate_kings_row(void);
		int evaluate_edges(void);

		static const int INFINITY = INT_MAX;
		static const int WIN      = 65535;

		board _board;
	};
}

#endif // __INTELLIGENCE_HPP_
// End of file
