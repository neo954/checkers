// @file intelligence.cpp

#include "intelligence.hpp"

namespace checkers
{
	intelligence::intelligence(const board& board, player player) :
		_board(board), _player(player)
	{
		this->whether_jump();
	}

	int intelligence::alpha_beta_search(int depth, int alpha, int beta,
		std::vector<move>& best_moves)
	{
		if (0 == depth)
		{
			best_moves.clear();
			return evaluate();
		}

		// Generate legal moves
		std::vector<move> legal_moves = this->generate_moves();
		int val;
		std::vector<move> moves;

		for (std::vector<move>::const_iterator pos =
			legal_moves.begin(); pos != legal_moves.end(); ++pos)
		{
			intelligence intelligence(*this);

			intelligence.make_move(*pos);
			val = (this->_player == intelligence._player) ?
				 intelligence.alpha_beta_search(depth - 1,
					alpha,   beta, moves) :
				-intelligence.alpha_beta_search(depth - 1,
					-beta, -alpha, moves);

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

	// ================================================================

	std::vector<move> intelligence::generate_moves(void) const
	{
		return (BLACK == this->_player) ?
			(this->_is_jump ?
				this->_board.generate_black_jumps() :
				this->_board.generate_black_moves()) :
			(this->_is_jump ?
				this->_board.generate_white_jumps() :
				this->_board.generate_white_moves());
	}

	void intelligence::make_move(const move& move)
	{
		if (this->_is_jump)
		{
			if (BLACK == this->_player)
			{
				this->_board.black_jump(move);
				if (this->_board.get_black_jumpers())
				{
					return;
				}
			}
			else // WHITE
			{
				this->_board.white_jump(move);
				if (this->_board.get_white_jumpers())
				{
					return;
				}
			}
		}
		else
		{
			if (BLACK == this->_player)
			{
				this->_board.black_move(move);
			}
			else
			{
				this->_board.white_move(move);
			}
		}

		this->switch_player();
		this->whether_jump();
	}

	void intelligence::switch_player(void)
	{
		this->_player = (BLACK == this->_player) ? WHITE : BLACK;
	}

	void intelligence::whether_jump(void)
	{
		this->_is_jump = (BLACK == this->_player) ?
			this->_board.get_black_jumpers() :
			this->_board.get_white_jumpers();
	}

	/**
	 *  @return >0 Black is ahead in game
	 *  @return <0 White is ahead in game
	 */
	int intelligence::evaluate(void)
	{
		int val; 

		val = this->evaluate_win();
		if (val != 0)
		{
			return val;
		}

		return this->evaluate_pieces_strength();
	}

	int intelligence::evaluate_win(void)
	{
		if (!this->_is_jump)
		{
			if (BLACK == this->_player)
			{
				if (!this->_board.get_black_movers())
				{
					return -WIN;
				}
			}
			else // WHITE
			{
				if (!this->_board.get_white_movers())
				{
					return WIN;
				}
			}
		}

		return 0;
	}

	/**
	 *  A  man get 16 points
	 *  A king get 16 + 8 = 24 points
	 */
	int intelligence::evaluate_pieces_strength(void)
	{
		return (this->_board.get_black_pieces().bit_count() -
			this->_board.get_white_pieces().bit_count()) * 16 + (
			this->_board.get_black_kings().bit_count() -
			this->_board.get_white_kings().bit_count())  * 8;
	}
}
// End of file
