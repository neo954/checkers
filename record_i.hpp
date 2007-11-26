/* $Id: record_i.hpp,v 1.2 2007-11-26 08:13:14 neo Exp $

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
/** @file record_i.hpp
 *  @brief Record for alpha-beta search.
 */

#ifndef __RECORD_I_HPP__
#define __RECORD_I_HPP__

namespace checkers
{
	inline record::record(void) :
		_zobrist(), _depth(0U), _val(0), _flag(EXACT), _best_moves()
	{
	}

	inline record::record(zobrist zobrist, unsigned int depth, int val,
		hash_flag flag) :
		_zobrist(zobrist), _depth(depth), _val(val), _flag(flag),
		_best_moves()
	{
	}

	inline record::record(zobrist zobrist, unsigned int depth, int val,
		hash_flag flag, const std::vector<move>& best_moves) :
		_zobrist(zobrist), _depth(depth), _val(val), _flag(flag),
		_best_moves(best_moves)
	{
	}

	inline zobrist record::get_zobrist(void) const
	{
		return this->_zobrist;
	}
}

#endif // __RECORD_I_HPP__
// End of file
