/* $Id: evaluate_i.hpp,v 1.1 2007-11-26 06:49:05 neo Exp $

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
/** @file evaluate_i.hpp
 *  @brief Artificial intelligence, weight of evaluate strategy.
 */

#ifndef __EVALUATE_I_HPP__
#define __EVALUATE_I_HPP__

#include <limits>

namespace checkers
{
	inline int evaluate::win(void)
	{
		return evaluate::WEIGHT_PIECES * 256;
	}

	inline int evaluate::infinity(void)
	{
		return std::numeric_limits<int>::max();
	}

	inline int evaluate::unknown(void)
	{
		return std::numeric_limits<int>::min();
	}
}

#endif // __EVALUATE_I_HPP__
// End of file
