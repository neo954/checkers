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
/** @file transposition.hpp
 *  @brief
 *  $Author: neo $
 *  $Date: 2007-11-15 10:36:32 $
 *  $Revision: 1.2 $
 */

#ifndef __TRANSPOSITION_H__
#define __TRANSPOSITION_H__

#include <tr1/unordered_map>

namespace checkers
{
	template <int hashtable_size>
	class transposition
	{
	public:
	private:
		std::tr1::unordered_map<uint64_t, uint64_t> _keys;
	};
}

#include "transposition_i.hpp"
#endif // __TRANSPOSITION_HPP__
// End of file