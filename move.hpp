/// @file move.hpp

#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include <string>
#include "bitboard.hpp"

namespace checkers
{
	class move
	{
	public:
		inline move(bitboard orig, bitboard dest);
		explicit move(const std::string& input);

		std::string to_string(void) const;

		inline bitboard get_orig(void) const;
		inline bitboard get_dest(void) const;

		bitboard is_valid_on_black_man(void) const;
		bitboard is_valid_on_white_man(void) const;
		bitboard is_valid_on_king(void) const;

		bitboard get_black_man_jump_capture(void) const;
		bitboard get_white_man_jump_capture(void) const;
		bitboard get_king_jump_capture(void) const;

	private:
		move(void);

		bitboard _orig;
		bitboard _dest;
	};
};

#include "move_i.hpp"
#endif // __MOVE_HPP__
// End of file
