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
		inline explicit intelligence(const board& board);

		int alpha_beta_search(std::vector<move>& best_moves,
			int depth, int alpha = -INFINITY, int beta = INFINITY,
			int ply = 0);
		inline void init_best_moves(const std::vector<move>& moves);

	private:
		inline int evaluate(void);
		inline int evaluate_pieces_strength(void);
		inline int evaluate_movers(void);
		inline int evaluate_kings_row(void);
		inline int evaluate_edges(void);

		inline void reorder_moves(std::vector<move>& moves, int ply);

		static const int INFINITY = INT_MAX;
		static const int WIN      = 65535;

		board _board;

		static std::vector<move> _best_moves;
		static bool _reorder;
	};
}

#include "intelligence_i.hpp"
#endif // __INTELLIGENCE_HPP_
// End of file
