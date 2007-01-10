/// @file intelligence.hpp

#ifndef __INTELLIGENCE_HPP_
#define __INTELLIGENCE_HPP_

#include <vector>
#include "board.hpp"
#include "move.hpp"
#include "player.hpp"

namespace checkers
{
	class intelligence
	{
	public:
		intelligence(const board& board, player player);
		move think(int depth);

	private:
		int alpha_beta_search(int depth, int alpha, int beta,
			std::vector<move>& best_moves);

		std::vector<move> generate_moves(void) const;
		void make_move(const move& move);
		void switch_player(void);
		void whether_jump(void);

		int evaluate(void);
		int evaluate_win(void);
		int evaluate_pieces_strength(void);

		static const int INFINITY = INT_MAX;
		static const int WIN = 9999;

		board _board;
		player _player;
		bool _is_jump;
	};
}
#endif // __INTELLIGENCE_HPP_
// End of file
