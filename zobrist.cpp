/* $Id: zobrist.cpp,v 1.9 2009-11-08 13:41:26 neo Exp $

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007, 2008, 2009 Mamiyami Information.
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
/** @file zobrist.cpp
 *  @brief The Zobrist key.
 */

#include "zobrist.hpp"

namespace checkers
{
	uint64_t zobrist::rand64(void)
	{
		return uint64_t(rand32()) << 32 | uint64_t(rand32());
	}

	uint64_t zobrist::_black_pieces[] =
	{
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
	};
	uint64_t zobrist::_white_pieces[] =
	{
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
	};
	uint64_t zobrist::_kings[] =
	{
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
		rand64(), rand64(), rand64(), rand64(),
	};
	uint64_t zobrist::_change_side = rand64();
}

// End of file
