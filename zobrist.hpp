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
 *  $Date: 2007-11-07 09:44:43 $
 *  $Revision: 1.1 $
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
		explicit zobrist(uint64_t key);
		explicit zobrist(const board& board);

		inline uint64_t key(void) const;
		void make_move(const move& move);

	private:
		static void init(void);
		static uint64_t rand64(void);

		static uint64_t _zobrist[3][32];

		enum zobrist_index
		{
			BLACK = 0,
			WHITE = 1,
			KING  = 2
		};

		uint64_t _zobrist_key;
	};
}

#include "zobrist_i.hpp"
#endif // __ZOBRIST_HPP__
// End of file
