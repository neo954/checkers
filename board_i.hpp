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
/** @file board_i.hpp
 *  @brief Checkers game board.
 */

#ifndef __BOARD_I_HPP__
#define __BOARD_I_HPP__

namespace checkers
{
	/**  Reset all pieces to initial position and set the player has dark
	 *   pieces makes the next move.
	 */ 
	inline board::board(void) :
		_black_pieces(bitboard::BLACK_PIECES_INIT),
		_white_pieces(bitboard::WHITE_PIECES_INIT),
		_kings(bitboard::EMPTY), _player(board::BLACK), _zobrist(0x0UL)
	{
		this->_zobrist = this->build_zobrist();
	}

	inline bitboard board::get_black_pieces(void) const
	{
		return this->_black_pieces;
	}

	inline bitboard board::get_white_pieces(void) const
	{
		return this->_white_pieces;
	}

	inline bitboard board::get_kings(void) const
	{
		return this->_kings;
	}

	inline bitboard board::get_occupied(void) const
	{
		return this->_black_pieces | this->_white_pieces;
	}

	inline bitboard board::get_unoccupied(void) const
	{
		return ~this->get_occupied();
	}

	inline bitboard board::get_black_men(void) const
	{
		return this->_black_pieces & ~this->_kings;
	}

	inline bitboard board::get_white_men(void) const
	{
		return this->_white_pieces & ~this->_kings;
	}

	inline bitboard board::get_black_kings(void) const
	{
		return this->_black_pieces & this->_kings;
	}

	inline bitboard board::get_white_kings(void) const
	{
		return this->_white_pieces & this->_kings;
	}

	/** @return Whether the player has dark pieces will make the next move.
	 */
	inline bool board::is_black_to_move(void) const
	{
		return board::BLACK == this->_player;
	}

	/** @return Whether the player has light pieces will make the next move.
	 */
	inline bool board::is_white_to_move(void) const
	{
		return board::WHITE == this->_player;
	}

	/** @return The Zobrist key.
	 */
	inline zobrist board::get_zobrist(void) const
	{
		return this->_zobrist;
	} 
}

#endif // __BOARD_I_HPP__
// End of file
