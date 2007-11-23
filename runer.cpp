/* $Id: runer.cpp,v 1.7 2007-11-23 15:18:09 neo Exp $

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
/** @file runer.cpp
 *  @brief
 */

#include <iostream>
#include <stdexcept>
#include "io.hpp"
#include "signal.hpp"
#include "pipe.hpp"

void usage(void)
{
	std::cerr
		<< "Usage: runer --black PROGRAM --white PROGRAM [--depth DEPTH]\n"
		<< std::flush;
}

int main(int argc, char* argv[])
{
	try
	{
		checkers::signal(SIGINT,  SIG_IGN);
		checkers::signal(SIGQUIT, SIG_IGN);

		std::string black;
		std::string white;
		int depth = 1;
		int moves_limit = 299;
		int i = 0;
	
		while (++i < argc)
		{
			if ("--black" == std::string(argv[i]))
			{
				if (++i < argc)
				{
					black = argv[i];
				}
			}
			else if ("--white" == std::string(argv[i]))
			{
				if (++i < argc)
				{
					white = argv[i];
				}
			}
			else if ("--depth" == std::string(argv[i]))
			{
				if (++i < argc)
				{
					depth = ::strtol(argv[i], NULL, 10);
					if (depth < 0 || depth > 999)
					{
						std::cerr <<
							"Error: Invalid depth"
							<< std::endl;
						exit(255);
					}
				}
			}
		}
	
		if (black.empty() || white.empty())
		{
			usage();
			exit(255);
		}

		checkers::io io(STDIN_FILENO, STDOUT_FILENO);
		checkers::io io_black(checkers::pipe_open(black));
		checkers::io io_white(checkers::pipe_open(white));
		std::string line_black;
		std::string line_white;
		std::string move;
		int moves = 0;
	
		io_black << "st 999\nsd " << depth << '\n';
		io_white << "st 999\nsd " << depth << '\n';
	
		io_black << "go\n";
	
		for (;;)
		{
			line_black.erase();
			line_white.erase();
			for (;;)
			{
				io_black << checkers::io::flush;
				io_black >> line_black;
				io_white << checkers::io::flush;
				io_white >> line_white;

				if (line_black.empty() && line_white.empty())
				{
					usleep(500);
				}
				else
				{
					break;
				}
			}
			
			if (!line_black.empty())
			{
				switch (line_black[0])
				{
				case 'm':
					move = line_black.substr(5, 4);
					io << "Black move " << move << '\n'
						<< checkers::io::flush;
					io_white << move << '\n' << checkers::io::flush;
					++moves;
					if (moves > moves_limit)
					{
						io << "RESULT 1/2-1/2 {Draw}\n"
							<< checkers::io::flush;
						return 0;
					}
					break;
				case 'R':
					io << line_black << '\n' << checkers::io::flush;
					return 0;
				case 'E':
					io << "Black " << line_black << '\n'
						<< checkers::io::flush;
					return 255;
				case ' ':
					break;
				default:
					io << "Black " << line_black << '\n'
						<< checkers::io::flush;
					break;
				}
			}
	
			if (!line_white.empty())
			{
				switch (line_white[0])
				{
				case 'm':
					move = line_white.substr(5, 4);
					io << "White move " << move << '\n'
						<< checkers::io::flush;
					io_black << move << '\n' << checkers::io::flush;
					++moves;
					if (moves > moves_limit)
					{
						io << "RESULT 1/2-1/2 {Draw}\n"
							<< checkers::io::flush;
						return 0;
					}
					break;
				case 'R':
					io << line_white << '\n' << checkers::io::flush;
					return 0;
					break;
				case 'E':
					io << "White " << line_black << '\n'
						<< checkers::io::flush;
					return 255;
				case ' ':
					break;
				default:
					io << "White " << line_black << '\n'
						<< checkers::io::flush;
					return 255;
				}
			}
		}
	} // try
	catch (std::exception& e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(255);
	}

	return 0;
}

// End of file
