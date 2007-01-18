/** @file move.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include "bitboard.hpp"

namespace checkers
{
	class move
	{
	public:
		inline move(bitboard orig, bitboard dest);
		explicit move(const std::string& str);

		inline bitboard get_orig(void) const;
		inline bitboard get_dest(void) const;

		inline bool is_jump(void) const;
		inline bool is_move(void) const;

		bitboard is_valid_on_black_man(void) const;
		bitboard is_valid_on_white_man(void) const;
		bitboard is_valid_on_king(void) const;

		bitboard get_black_man_jump_capture(void) const;
		bitboard get_white_man_jump_capture(void) const;
		bitboard get_king_jump_capture(void) const;

		inline static bool is_valid(const std::string& str);

		friend bool operator ==(const move& lhs, const move& rhs);
		friend bool operator !=(const move& lhs, const move& rhs);

		friend std::ostream& operator <<(std::ostream& os,
			const move& rhs);

	private:
		bitboard _orig;
		bitboard _dest;
	};

	inline bool operator ==(const move& lhs, const move& rhs);
	inline bool operator !=(const move& lhs, const move& rhs);

	std::ostream& operator <<(std::ostream& os, const move& rhs);
}

#include "move_i.hpp"
#endif // __MOVE_HPP__
// End of file
