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
/** @file board.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-11-01 16:31:01 $
 *  $Revision: 1.11 $
 */

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <ostream>
#include <vector>
#include "move.hpp"
#include "player.hpp"

namespace checkers
{

	class board
	{
	public:
		inline board(void);
		explicit board(const std::string& input);

		/** Reset all pieces to initial position and set the current
		 *  player to black */
		void opening(void);

		/// Check if move is legal based on current situation
		bool is_valid_move(const move& move) const;

		bool make_black_move(const move& move);
		bool make_white_move(const move& move);

		/// Move piece
		inline bool make_move(const move& move);
		/// Undo piece
		inline bool undo_move(const move& move);

		inline bitboard get_black_pieces(void) const;
		inline bitboard get_white_pieces(void) const;

		inline bitboard get_occupied(void) const;
		inline bitboard get_not_occupied(void) const;

		inline bitboard get_black_men(void) const;
		inline bitboard get_white_men(void) const;
		inline bitboard get_black_kings(void) const;
		inline bitboard get_white_kings(void) const;

		/// Check if the player on move is black
		inline bool is_black_move(void) const;
		/// Check if the player on move is white
		inline bool is_white_move(void) const;

		bitboard get_black_movers(void) const;
		bitboard get_white_movers(void) const;
		bitboard get_black_jumpers(void) const;
		bitboard get_white_jumpers(void) const;

		std::vector<move> generate_black_moves(void) const;
		std::vector<move> generate_white_moves(void) const;
		std::vector<move> generate_black_jumps(void) const;
		std::vector<move> generate_white_jumps(void) const;

		/// Generate all legal moves based on current situation
		inline std::vector<move> generate_moves(void) const;

		/// Set the player on move black
		inline void set_black(void);
		/// Set the player on move white
		inline void set_white(void);
		/// Check whether the player on move is winning
		inline bool is_winning(void) const;
		/// Check whether the player on move is losing
		inline bool is_losing(void) const;
		
	private:
		/// Black pieces
		bitboard _black_pieces;
		/// White pieces
		bitboard _white_pieces;
		/// Kings for both black and white
		bitboard _kings;
		/// The player on move
		player _player;

		friend std::ostream& operator <<(std::ostream& os,
			const board& rhs);
	};

	std::ostream& operator <<(std::ostream& os, const board& rhs);
}

#include "board_i.hpp"
#endif // __BOARD_HPP__
// End of file
