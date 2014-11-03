/* $Id: nonstdio.cpp,v 1.1 2009-04-10 18:34:23 neo Exp $

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2009, 2014 Quux Information.
                     Gong Jie <neo@quux.me>

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
/** @file nonstdio.cpp
 *  @brief Synchronous stdin/stdout.
 */

extern "C"
{
	#include <unistd.h>
}
#include "nonstdio.hpp"

namespace checkers
{
	io nio(STDIN_FILENO, STDOUT_FILENO);
}

// End of file
