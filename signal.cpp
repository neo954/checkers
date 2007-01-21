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
/** @file signal.cpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-01-21 14:31:39 $
 *  $Revision: 1.2 $
 */

#include <cerrno>
#include <csignal>
#include <stdexcept>
#include "signal.hpp"

namespace checkers
{
	sighandler_t signal(int signum, sighandler_t handler)
	{
		struct sigaction action;
		struct sigaction old_action;

		action.sa_handler = handler;
		// Block sigs of type being handled
		sigemptyset(&action.sa_mask);
		// Restart syscalls if possible
		action.sa_flags = SA_RESTART;

		if (::sigaction(signum, &action, &old_action) < 0) {
			throw std::runtime_error(std::string("Signal error: ")
				+ std::strerror(errno));
		}
		return (old_action.sa_handler);
	}
}

// End of file
