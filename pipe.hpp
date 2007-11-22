/* $Id: pipe.hpp,v 1.6 2007-11-22 16:30:56 neo Exp $

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
/** @file pipe.hpp
 *  @brief Create pipe.
 */

#ifndef __PIPE_HPP__
#define __PIPE_HPP__

extern "C"
{
	#include <unistd.h>
}
#include <cerrno>
#include <stdexcept>

namespace checkers
{
	std::pair<int, int> pipe_open(const std::string& path);
}

#endif // __PIPE_HPP__
// End of file
