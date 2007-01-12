// @file intelligence.cpp

#include "io.hpp"
#include "intelligence.hpp"

namespace checkers
{
	intelligence::intelligence(const board& board, player player) :
		_board(board), _player(player)
	{
		this->whether_jump();
	}

	int intelligence::alpha_beta_search(std::vector<move>& best_moves,
		 int depth, int alpha, int beta, int ply)
	{
		int winning = this->evaluate_winning();
		if (winning > 0)
		{
			return winning - ply;
		}
		else if (winning < 0)
		{
			return winning + ply;
		}
		else if (0 >= depth)
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
			val = -intelligence.alpha_beta_search(moves, depth - 1,
					-beta, -alpha, ply + 1);

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
				if (!this->_board.black_jump(move) &&
					this->_board.get_black_jumpers())
				{
					return;
				}
			}
			else // WHITE
			{
				if (!this->_board.white_jump(move) &&
					this->_board.get_white_jumpers())
				{
					return;
				}
			}
		}
		else
		{
			if (BLACK == this->_player)
			{
				(void)this->_board.black_move(move);
			}
			else
			{
				(void)this->_board.white_move(move);
			}
		}

		this->switch_player();
		this->whether_jump();
	}

	void intelligence::switch_player(void)
	{
		this->_player = (BLACK == this->_player) ? WHITE : BLACK;
	}

	/** Decide the current step is jump or not.
	 *  And update this->_is_jump.
	 */
	void intelligence::whether_jump(void)
	{
		this->_is_jump = (BLACK == this->_player) ?
			this->_board.get_black_jumpers() :
			this->_board.get_white_jumpers();
	}

	/**
	 *  @return >0 when the current player is ahead in game, and
	 *  @return <0 when the current player is behind in game
	 */
	int intelligence::evaluate(void)
	{
		return this->evaluate_pieces_strength() * 256 +
			this->evaluate_movers() * 2 +
			this->evaluate_kings_row() * 16 +
			this->evaluate_edges() * 8;
	}

	int intelligence::evaluate_winning(void)
	{
		if (!this->_is_jump)
		{
			if (BLACK == this->_player)
			{
				if (!this->_board.get_white_pieces())
				{
					return WIN;
				}
				if (!this->_board.get_black_movers())
				{
					return -WIN;
				}
			}
			else // WHITE
			{
				if (!this->_board.get_black_pieces())
				{
					return WIN;
				}
				if (!this->_board.get_white_movers())
				{
					return -WIN;
				}
			}
		}

		return 0;
	}

	/**
	 *  A  man get 1 points
	 *  A king get 2 points
	 */
	int intelligence::evaluate_pieces_strength(void)
	{
		return (BLACK == this->_player) ?
			((this->_board.get_black_pieces().bit_count() -
			  this->_board.get_white_pieces().bit_count()) +
			 (this->_board.get_black_kings().bit_count() -
			  this->_board.get_white_kings().bit_count())) :
			((this->_board.get_white_pieces().bit_count() -
			  this->_board.get_black_pieces().bit_count()) +
			 (this->_board.get_white_kings().bit_count() -
			  this->_board.get_black_kings().bit_count()));
	}

	int intelligence::evaluate_movers(void)
	{
		return (BLACK == this->_player) ?
			(this->_board.get_black_movers() -
			 this->_board.get_white_movers()) :
			(this->_board.get_white_movers() -
			 this->_board.get_black_movers());
	}

	int intelligence::evaluate_kings_row(void)
	{
		return (BLACK == this->_player) ?
			((this->_board.get_black_pieces() &
				bitboard::WHITE_KINGS_ROW).bit_count() -
			 (this->_board.get_white_pieces() &
				bitboard::BLACK_KINGS_ROW).bit_count()) :
			((this->_board.get_white_pieces() &
				bitboard::BLACK_KINGS_ROW).bit_count() -
			 (this->_board.get_black_pieces() &
				bitboard::WHITE_KINGS_ROW).bit_count());
	}

	int intelligence::evaluate_edges(void)
	{
		return (BLACK == this->_player) ?
			((this->_board.get_black_pieces() &
				bitboard::EDGES).bit_count() -
			 (this->_board.get_white_pieces() &
				bitboard::EDGES).bit_count()) :
			((this->_board.get_white_pieces() &
				bitboard::EDGES).bit_count() -
			 (this->_board.get_black_pieces() &
				bitboard::EDGES).bit_count());
			
	}
}

// End of file
