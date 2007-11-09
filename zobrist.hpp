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
/** @file zobrist.hpp
 *  @brief
 *  $Author: neo $
 *  $Date: 2007-11-09 09:55:02 $
 *  $Revision: 1.3 $
 */

#ifndef __ZOBRIST_HPP__
#define __ZOBRIST_HPP__

#include <stdint.h>
#include "board.hpp"
#include "move.hpp"

namespace checkers
{
	/** @class zobrist
	 *  @brief
	 */
	class zobrist
	{
	public:
		explicit inline zobrist(uint64_t key);
		explicit zobrist(const board& board);

		inline uint64_t key(void) const;
		void make_black_move(const move& move);
		void make_white_move(const move& move);
		void make_move(const move& move);

	private:
		static uint64_t rand64(void);

		static uint64_t _black_pieces[32];
		static uint64_t _white_pieces[32];
		static uint64_t _kings[32];
		static uint64_t _player;

		uint64_t _key;
	};
}

#include "zobrist_i.hpp"
#endif // __ZOBRIST_HPP__
// End of file
