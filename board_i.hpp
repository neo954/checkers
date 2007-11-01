/* This file is a part of ponder, a English/American checkers game.

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
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date $Date: 2007-11-01 16:50:07 $
 *  @version $Revision: 1.11 $
 */

#ifndef __BOARD_I_HPP__
#define __BOARD_I_HPP__

namespace checkers
{
	inline board::board(void) :
		_black_pieces(bitboard::EMPTY), _white_pieces(bitboard::EMPTY),
		_kings(bitboard::EMPTY), _player(BLACK)
	{
	}

	/// @return Whether the same player move one more
	inline bool board::make_move(const move& move)
	{
		assert(this->is_valid_move(move));

		return this->is_black_move() ?
			this->make_black_move(move) :
			this->make_white_move(move);
	}

	inline bitboard board::get_black_pieces(void) const
	{
		return this->_black_pieces;
	}

	inline bitboard board::get_white_pieces(void) const
	{
		return this->_white_pieces;
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

	inline bool board::is_black_move(void) const
	{
		return BLACK == this->_player;
	}

	inline bool board::is_white_move(void) const
	{
		return WHITE == this->_player;
	}

	inline std::vector<move> board::generate_moves(void) const
	{
		return (this->is_black_move()) ?
			(this->get_black_jumpers() ?
				this->generate_black_jumps() :
				this->generate_black_moves()) :
			(this->get_white_jumpers() ?
				this->generate_white_jumps() :
				this->generate_white_moves());
	}

	inline void board::set_black(void)
	{
		this->_player = BLACK;
	}

	inline void board::set_white(void)
	{
		this->_player = WHITE;
	}

	inline bool board::is_winning(void) const
	{
		return this->is_black_move() ?
			!this->get_white_pieces() :
			!this->get_black_pieces();
	}

	inline bool board::is_losing(void) const
	{
		return this->is_black_move() ?
			!(this->get_black_jumpers() ||
			  this->get_black_movers()) :
			!(this->get_white_jumpers() ||
			  this->get_white_movers());
	}
}

#endif // __BOARD_I_HPP__
// End of file
