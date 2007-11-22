/* $Id: board_i.hpp,v 1.22 2007-11-22 16:30:55 neo Exp $

   This file is a part of ponder, a English/American checkers game.

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
/** @file board_i.hpp
 *  @brief Checkers game board.
 */

#ifndef __BOARD_I_HPP__
#define __BOARD_I_HPP__

namespace checkers
{
	inline board::board(void) :
		_black_pieces(bitboard::EMPTY), _white_pieces(bitboard::EMPTY),
		_kings(bitboard::EMPTY), _player(board::BLACK), _zobrist(0x0UL)
	{
	}

	/** @return whether the same player move once more.
	 */
	inline bool board::make_move(const move& move)
	{
		assert(this->is_valid_move(move));

		/** @retval true if multiple opposing pieces may be captured in
		 *   a single turn, and this move is not the last move in this
		 *   turn.  The same player will make move once more.
		 *  @retval false if the other player will make the next move.
		 */
		return this->is_black_to_move() ?
			this->make_black_move(move) :
			this->make_white_move(move);
	}

	inline void board::undo_move(const move& move)
	{
		if (move.get_dest() & this->_black_pieces)
		{
			this->undo_black_move(move);
		}
		else
		{
			this->undo_white_move(move);
		}

		assert(this->is_valid_move(move));
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

	inline bitboard board::get_not_occupied(void) const
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

	inline std::vector<move> board::generate_moves(void) const
	{
		return (this->is_black_to_move()) ?
			(this->get_black_jumpers() ?
				this->generate_black_jumps() :
				this->generate_black_moves()) :
			(this->get_white_jumpers() ?
				this->generate_white_jumps() :
				this->generate_white_moves());
	}

	inline void board::set_black_to_move(void)
	{
		this->_player = board::BLACK;
	}

	inline void board::set_white_to_move(void)
	{
		this->_player = board::WHITE;
	}

	/** @return game over or not.
	 *  @retval true when the current player is winning.
	 *  @retval false in other situation.
	 */
	inline bool board::is_winning(void) const
	{
		return this->is_black_to_move() ?
			!this->get_white_pieces() :
			!this->get_black_pieces();
	}

	/** @return game over or not.
	 *  @retval true when the current player is losing.
	 *  @retval false in other situation.
	 */
	inline bool board::is_losing(void) const
	{
		return this->is_black_to_move() ?
			!(this->get_black_jumpers() ||
			  this->get_black_movers()) :
			!(this->get_white_jumpers() ||
			  this->get_white_movers());
	}
}

#endif // __BOARD_I_HPP__
// End of file
