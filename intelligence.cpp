// @file intelligence.cpp

#include "io.hpp"
#include "intelligence.hpp"

namespace checkers
{
	intelligence::intelligence(const board& board) :
		_board(board)
	{
	}

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

	// ================================================================

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

	/**
	 *  A  man get 1 points
	 *  A king get 2 points
	 */
	int intelligence::evaluate_pieces_strength(void)
	{
		return this->_board.is_black_move() ?
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
		return this->_board.is_black_move() ?
			(this->_board.get_black_movers().bit_count() -
			 this->_board.get_white_movers().bit_count()) :
			(this->_board.get_white_movers().bit_count() -
			 this->_board.get_black_movers().bit_count());
	}

	int intelligence::evaluate_kings_row(void)
	{
		return this->_board.is_black_move() ?
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
		return this->_board.is_black_move() ?
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
