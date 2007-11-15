/* This file is a part of textual checkers, a English/American checkers

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
/** @file pipe.cpp
 *  @brief
 *  $Author: neo $
 *  $Date: 2007-11-15 10:36:31 $
 *  $Revision: 1.4 $
 */

#include "pipe.hpp"

namespace checkers
{
	std::pair<int, int> pipe_open(const std::string& path)
	{
		int fd[2][2];
		pid_t pid;
		size_t i;
	
		for (i = 0; i < sizeof(fd) / sizeof(fd[0]); ++i)
		{
			if (pipe(fd[i]) < 0)
			{
				/** @throw std::runtime_error when pipe()
				 *   failed.
				 */
				throw std::runtime_error(
					std::string("pipe() failed: ")
					+ std::strerror(errno));
			}
		}
		if ((pid = fork()) < 0)
		{
			/// @throw std::runtime_error when fork() failed.
			throw std::runtime_error(
				std::string("fork() failed: ")
				+ std::strerror(errno));
		}
		else if (0 == pid)
		{
			// Child
			close(fd[0][1]);
			close(fd[1][0]);
			if (STDIN_FILENO != fd[0][0])
			{
				if (dup2(fd[0][0], STDIN_FILENO)
					!= STDIN_FILENO)
				{
					throw std::runtime_error(
						std::string("dup2() failed: ")
						+ std::strerror(errno));
				}
			}
			if (STDOUT_FILENO != fd[1][1])
			{
				if (dup2(fd[1][1], STDOUT_FILENO)
					!= STDOUT_FILENO)
				{
					throw std::runtime_error(
						std::string("dup2() failed: ")
						+ std::strerror(errno));
				}
			}
			if (execl(path.c_str(), path.c_str(), NULL) < 0)
			{
				throw std::runtime_error(
					std::string("execl() failed: ")
					+ std::strerror(errno));
			}
		}
		// Parent
		close(fd[0][0]);
		close(fd[1][1]);
		return std::pair<int, int>(fd[1][0], fd[0][1]);
	}
}
// End of file
