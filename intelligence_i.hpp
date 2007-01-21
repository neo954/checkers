/* This file is a part of textual checkers, a English/American checkers
   game.

   Copyright (c) 2006, 2007 Mamiyami Information.
                     Gong Jie <neo@mamiyami.com>

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
/** @file intelligence_i.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-01-21 01:40:41 $
 *  $Revision: 1.7 $
 */

#ifndef __INTELLIGENCE_I_HPP__
#define __INTELLIGENCE_I_HPP__

namespace checkers
{
	inline intelligence::intelligence(const board& board) :
		_board(board)
	{
	}

	// ================================================================

	/**
	 *  @return >0 when the current player is ahead in game, and
	 *  @return <0 when the current player is behind in game
	 */
	inline int intelligence::evaluate(void)
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
	inline int intelligence::evaluate_pieces_strength(void)
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

	inline int intelligence::evaluate_movers(void)
	{
		return this->_board.is_black_move() ?
			(this->_board.get_black_movers().bit_count() -
			 this->_board.get_white_movers().bit_count()) :
			(this->_board.get_white_movers().bit_count() -
			 this->_board.get_black_movers().bit_count());
	}

	inline int intelligence::evaluate_kings_row(void)
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

	inline int intelligence::evaluate_edges(void)
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

	inline void intelligence::reorder_moves(std::vector<move>& moves, int ply)
	{
		if (!this->_reorder)
		{
			return;
		}
		if (static_cast<std::vector<move>::size_type>(ply)
			>= this->_best_moves.size())
		{
			this->_reorder = false;
			return;
		}

		std::vector<move>::iterator pos = std::find(moves.begin(),
			moves.end(), this->_best_moves[ply]);
		if (moves.end() == pos)
		{
			this->_reorder = false;
			return;
		}

		std::swap(moves[0], *pos);
	}

	inline void intelligence::set_timeout(time_t second)
	{
		::gettimeofday(&intelligence::_deadline, NULL);
		intelligence::_deadline += second;
	}

	inline bool intelligence::is_timeout(void)
	{
		struct timeval now;
		::gettimeofday(&now, NULL);
		return now > intelligence::_deadline;
	}
}

#endif // __INTELLIGENCE_I_HPP__
// End of file
