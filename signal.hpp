/* $Id: signal.hpp,v 1.11 2007-11-23 15:18:09 neo Exp $

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
/** @file signal.hpp
 *  @brief Signal handling, examine and change signal action.
 */

#ifndef __SIGNAL_H__
#define __SIGNAL_H__

#include <csignal>

namespace checkers
{
	typedef void (*sighandler_t)(int);
	/// Install a new signal handler for the signal with number @e signum.
	sighandler_t signal(int signum, sighandler_t handler);

	typedef void (*sigaction_t)(int, siginfo_t*, void*);
	/** @overload sighandler_t signal(int, sighandler_t)
	 */
	sigaction_t signal(int signum, sigaction_t handler);

	/// Print out debug information and make a core dump while crash.
	void crash_handler(int signum, siginfo_t* siginfo, void* context);
}

#endif // __SIGNAL_HPP__
// End of file
