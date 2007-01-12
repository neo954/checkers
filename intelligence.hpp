/// @file intelligence.hpp

#ifndef __INTELLIGENCE_HPP_
#define __INTELLIGENCE_HPP_

#include "board.hpp"
#include "move.hpp"
#include "player.hpp"

namespace checkers
{
	class intelligence
	{
	public:
		intelligence(const board& board, player player);

		int alpha_beta_search(std::vector<move>& best_moves,
			int depth, int alpha = -INFINITY, int beta = INFINITY,
			int ply = 0);
		int evaluate_winning(void);

	private:

		std::vector<move> generate_moves(void) const;
		void make_move(const move& move);
		void switch_player(void);
		void whether_jump(void);

		int evaluate(void);
		int evaluate_pieces_strength(void);
		int evaluate_movers(void);
		int evaluate_kings_row(void);
		int evaluate_edges(void);

		static const int INFINITY = INT_MAX;
		static const int WIN  = 9999;

		board _board;
		player _player;
		bool _is_jump;
	};
}

#endif // __INTELLIGENCE_HPP_
// End of file
