/* $Id: board.hpp,v 1.31 2007-11-28 17:17:21 neo Exp $

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
/** @file board.hpp
 *  @brief Checkers game board.
 */

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <vector>
#include "move.hpp"
#include "zobrist.hpp"

namespace checkers
{

	class board
	{
	public:
		/// Default constructor.
		inline board(void);
		/// Construct from an user input string.
		explicit board(const std::string& input);

		/** @brief Reset all pieces to initial position and set the
		 *   player has dark pieces makes the next move.
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
		bool make_move(const move& move);
		/// Undo one move.
		void undo_move(const move& move);

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

		/** @brief Check if the player has dark pieces will make the
		 *   next move.
		 */
		inline bool is_black_to_move(void) const;
		/** @brief Check if the player has light pieces will make the
		 *   next move.
		 */
		inline bool is_white_to_move(void) const;

		/// Get Zobrist key.
		inline zobrist get_zobrist(void) const;

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
		std::vector<move> generate_moves(void) const;
		/// Generate a legal move based on user input @e str.
		move generate_move(const std::string& str) const;

		/// Set the player has dark pieces makes the next move.
		void set_black_to_move(void);
		/// Set the player has light pieces makes the next move.
		void set_white_to_move(void);
		/// Check whether the current player is winning.
		bool is_winning(void) const;
		/// Check whether the current player is losing.
		bool is_losing(void) const;
		
	private:
		enum player
		{
			/// Player has dark pieces.
			BLACK = 1,
			/// Player has light pieces.
			WHITE = -BLACK
		};

		/// Build Zobrist key.
		zobrist build_zobrist(void);

		/// All the dark pieces on the game board.
		bitboard _black_pieces;
		/// All the light pieces on the game board.
		bitboard _white_pieces;
		/** @brief All the kings on the game board for both dark side
		 *   and light side.
		 */
		bitboard _kings;
		/// The player on move
		player _player;
		/// The Zobrist key
		zobrist _zobrist;
	};

	/// Stream out the current game board.
	std::ostream& operator <<(std::ostream& os, const board& rhs);
}

#include "board_i.hpp"
#endif // __BOARD_HPP__
// End of file
