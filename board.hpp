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
/** @file board.hpp
 *  @brief
 *  $Author: neo $
 *  $Date: 2007-11-13 15:48:27 $
 *  $Revision: 1.20 $
 */

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <vector>
#include "move.hpp"
#include "player.hpp"
#include "zobrist.hpp"

namespace checkers
{

	class board
	{
	public:
		inline board(void);
		explicit board(const std::string& input);

		/** @brief Reset all pieces to initial position and set the
		 *   the player has dark pieces on move.
		 */
		void opening(void);

		/// Check if move is legal based on current situation
		bool is_valid_move(const move& move) const;

		/// Make a move by the player has dark pieces.
		bool make_black_move(const move& move);
		/// Make a move by the player has light pieces.
		bool make_white_move(const move& move);

		/// Undo a move made by the player has dark pieces.
		void undo_black_move(const move& move);
		/// Undo a move made by the player has light pieces.
		void undo_white_move(const move& move);

		/// Make one move.
		inline bool make_move(const move& move);
		/// Undo one move.
		inline void undo_move(const move& move);

		/// Get all dark pieces on the game board.
		inline bitboard get_black_pieces(void) const;
		/// Get all light pieces on the game board.
		inline bitboard get_white_pieces(void) const;
		/// Get all kings on the game board.
		inline bitboard get_kings(void) const;

		/// Get all occupied squares on the game board.
		inline bitboard get_occupied(void) const;
		/// Get all not occupied squares on the game board.
		inline bitboard get_not_occupied(void) const;

		/// Get all dark men on the game board.
		inline bitboard get_black_men(void) const;
		/// Get all white men on the game board.
		inline bitboard get_white_men(void) const;
		/// Get all dark kings on the game board.
		inline bitboard get_black_kings(void) const;
		/// Get all light kings on the game board.
		inline bitboard get_white_kings(void) const;

		/// Check if the player on move is black
		inline bool is_black_move(void) const;
		/// Check if the player on move is white
		inline bool is_white_move(void) const;

		/// Get all dark pieces, which can move.
		bitboard get_black_movers(void) const;
		/// Get all light pieces, which can move.
		bitboard get_white_movers(void) const;
		/** @brief Get all dark pieces, which can jump and capture an
		 *   enemy piece.
		 */
		bitboard get_black_jumpers(void) const;
		/** @brief Get all light pieces, which can jump and capture an
		 *   enemy piece.
		 */
		bitboard get_white_jumpers(void) const;

		/// Generate all legal moves for dark pieces.
		std::vector<move> generate_black_moves(void) const;
		/// Generate all legal moves for light pieces.
		std::vector<move> generate_white_moves(void) const;
		/// Generate all legal jumps for dark pieces.
		std::vector<move> generate_black_jumps(void) const;
		/// Generate all legal jumps for light pieces.
		std::vector<move> generate_white_jumps(void) const;

		/// Generate all legal moves based on current situation.
		inline std::vector<move> generate_moves(void) const;
		/// Generate one legal move based on user input.
		move generate_move(const std::string& str) const;

		/// Set the player has dark pieces on move.
		inline void set_black(void);
		/// Set the player has light pieces on move.
		inline void set_white(void);
		/// Check whether the player on move is winning.
		inline bool is_winning(void) const;
		/// Check whether the player on move is losing.
		inline bool is_losing(void) const;
		
	private:
		/// Rebuild Zobrist key.
		zobrist build_zobrist(void);

		/// All the black pieces on the game board.
		bitboard _black_pieces;
		/// All the white pieces on the game board.
		bitboard _white_pieces;
		/// All the kings pieces for both black and white.
		bitboard _kings;
		/// The player on move
		player _player;
		/// The Zobrist key
		zobrist _zobrist;

		friend std::ostream& operator <<(std::ostream& os,
			const board& rhs);
	};

	std::ostream& operator <<(std::ostream& os, const board& rhs);
}

#include "board_i.hpp"
#endif // __BOARD_HPP__
// End of file
