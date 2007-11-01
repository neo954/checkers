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
/** @file move.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-11-01 10:15:05 $
 *  $Revision: 1.12 $
 */

#ifndef __MOVE_HPP__
#define __MOVE_HPP__

#include "bitboard.hpp"

namespace checkers
{
	class move
	{
	public:
		inline move(bitboard orig, bitboard dest, bitboard capture,
			bool is_capture_a_king, bool will_crown);
		explicit move(const std::string& str);

		inline bitboard get_orig(void) const;
		inline bitboard get_dest(void) const;
		inline bitboard get_capture(void) const;

		inline bool is_capture_a_king(void) const;
		inline bool will_crown(void) const;

		bitboard is_valid_on_black_man(void) const;
		bitboard is_valid_on_white_man(void) const;
		bitboard is_valid_on_king(void) const;

		inline static bool is_valid(const std::string& str);

		friend bool operator ==(const move& lhs, const move& rhs);
		friend bool operator !=(const move& lhs, const move& rhs);

		friend std::ostream& operator <<(std::ostream& os,
			const move& rhs);

	private:
		bitboard _orig;
		bitboard _dest;
		bitboard _capture;
		bool _is_capture_a_king;
		bool _will_crown;
	};

	inline bool operator ==(const move& lhs, const move& rhs);
	inline bool operator !=(const move& lhs, const move& rhs);

	std::ostream& operator <<(std::ostream& os, const move& rhs);
}

#include "move_i.hpp"
#endif // __MOVE_HPP__
// End of file
