/* $Id: intelligence_x.hpp,v 1.4 2007-11-15 17:41:45 neo Exp $

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
/** @file intelligence_x.hpp
 *  @brief
 */

#ifndef __INTELLIGENCE_X_HPP__
#define __INTELLIGENCE_X_HPP__

namespace checkers
{
	namespace evaluate
	{
		const int WEIGHT_PIECES    = 256;
		const int WEIGHT_KINGS     = 256;
		const int WEIGHT_MOVERS    = 2;
		const int WEIGHT_KINGS_ROW = 16;
		const int WEIGHT_EDGES     = 8;
	}
}

#endif // __INTELLIGENCE_X_HPP__
// End of file
