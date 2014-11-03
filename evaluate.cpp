/* $Id$

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007 Quux Information.
                     Gong Jie <neo@quux.me>

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
/** @file evaluate.cpp
 *  @brief Artificial intelligence, weight of evaluate strategy.
 */

#include "evaluate.hpp"

namespace checkers
{
	/**
	 *  @retval >0 when the current player is ahead in game
	 *  @retval <0 when the current player is behind in game
	 */
	int evaluate::evaluate(const board& board)
	{
		return men(board) * evaluate::WEIGHT_MAN +
			kings(board) * evaluate::WEIGHT_KING +
			movers(board) * evaluate::WEIGHT_MOVER +
			kings_row(board) * evaluate::WEIGHT_KINGS_ROW +
			edges(board) * evaluate::WEIGHT_EDGE;
	}

	int evaluate::men(const board& board)
	{
		return board.is_black_to_move() ?
			(board.get_black_men().count() -
			board.get_white_men().count()) :
			(board.get_white_men().count() -
			board.get_black_men().count());
	}

	int evaluate::kings(const board& board)
	{
		return board.is_black_to_move() ?
			 (board.get_black_kings().count() -
			  board.get_white_kings().count()) :
			 (board.get_white_kings().count() -
			  board.get_black_kings().count());
	}

	int evaluate::movers(const board& board)
	{
		return board.is_black_to_move() ?
			(board.get_black_movers().count() -
			 board.get_white_movers().count()) :
			(board.get_white_movers().count() -
			 board.get_black_movers().count());
	}

	int evaluate::kings_row(const board& board)
	{
		return board.is_black_to_move() ?
			((board.get_black_pieces() &
				bitboard::WHITE_KINGS_ROW).count() -
			 (board.get_white_pieces() &
				bitboard::BLACK_KINGS_ROW).count()) :
			((board.get_white_pieces() &
				bitboard::BLACK_KINGS_ROW).count() -
			 (board.get_black_pieces() &
				bitboard::WHITE_KINGS_ROW).count());
	}

	int evaluate::edges(const board& board)
	{
		return board.is_black_to_move() ?
			((board.get_black_pieces() & bitboard::EDGES).count() -
			 (board.get_white_pieces() & bitboard::EDGES).count()) :
			((board.get_white_pieces() & bitboard::EDGES).count() -
			 (board.get_black_pieces() & bitboard::EDGES).count());
	}
}

// End of file
