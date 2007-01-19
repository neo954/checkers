/** @file board.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 *  @date $Date: 2007-01-19 14:40:56 $
 *  @version $Revision: 1.8 $
 */

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <ostream>
#include <vector>
#include "move.hpp"

namespace checkers
{

	class board
	{
	public:
		enum player
		{
			BLACK = 1,
			WHITE = -BLACK
		};

		inline board(void);
		explicit board(const std::string& input);

		/** Reset all pieces to initial position and set the current
		 *  player to black */
		void opening(void);

		bool is_valid_black_move(const move& move) const;
		bool is_valid_white_move(const move& move) const;
		bool is_valid_black_jump(const move& move) const;
		bool is_valid_white_jump(const move& move) const;

		/// Check if move is legal based on current situation
		bool is_valid_move(const move& move) const;

		void make_black_move(const move& move);
		void make_white_move(const move& move);
		bool make_black_jump(const move& move);
		bool make_white_jump(const move& move);

		/// Move piece
		inline bool make_move(const move& move);

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
		/// Crown black men on kings row
		inline bitboard black_man_crown(void);
		/// Crown white men on kings row
		inline bitboard white_man_crown(void);

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
