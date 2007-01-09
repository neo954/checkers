/// @file board.hpp

#ifndef __BOARD_HPP__
#define __BOARD_HPP__

#include <string>
#include <vector>
#include "bitboard.hpp"
#include "move.hpp"

namespace checkers
{
	class interface;

	class board
	{
	public:
		inline board(void);
		explicit board(const std::string& input);

		std::string to_string(void) const;

		board& opening(void);

		bool is_valid_black_move(const move& move) const;
		bool is_valid_white_move(const move& move) const;
		bool is_valid_black_jump(const move& move) const;
		bool is_valid_white_jump(const move& move) const;

		void black_move(const move& move);
		void white_move(const move& move);
		void black_jump(const move& move);
		void white_jump(const move& move);

		inline bitboard get_black_pieces(void) const;
		inline bitboard get_white_pieces(void) const;

		inline bitboard get_occupied(void) const;
		inline bitboard get_not_occupied(void) const;

		inline bitboard get_black_men(void) const;
		inline bitboard get_white_men(void) const;
		inline bitboard get_black_kings(void) const;
		inline bitboard get_white_kings(void) const;

		bitboard get_black_movers(void) const;
		bitboard get_white_movers(void) const;
		bitboard get_black_jumpers(void) const;
		bitboard get_white_jumpers(void) const;

		std::vector<move> generate_black_moves(void) const;
		std::vector<move> generate_white_moves(void) const;
		std::vector<move> generate_black_jumps(void) const;
		std::vector<move> generate_white_jumps(void) const;
		
	private:
		inline void black_man_crown(void);
		inline void white_man_crown(void);

		bitboard _black_pieces;
		bitboard _white_pieces;
		bitboard _kings;
	};
};

#include "board_i.hpp"
#endif // __BOARD_HPP__
// End of file
