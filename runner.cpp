/* $Id: runner.cpp 562 2014-11-03 06:47:50Z neo $

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007 Quux Information.
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
/** @file runner.cpp
 *  @brief
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include "io.hpp"
#include "signal.hpp"
#include "pipe.hpp"

void usage(void)
{
	std::cerr
		<< "Usage: runner --black PROGRAM --white PROGRAM [--time SECOND]\n"
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
		int second = 10;
		int moves_limit = 999;
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
			else if ("--time" == std::string(argv[i]))
			{
				if (++i < argc)
				{
					second = std::strtol(argv[i], NULL, 10);
					if (second < 0 || second > 999)
					{
						std::cerr <<
							"Error: Invalid time"
							<< std::endl;
						std::exit(255);
					}
				}
			}
		}
	
		if (black.empty() || white.empty())
		{
			usage();
			std::exit(255);
		}

		checkers::io io(STDIN_FILENO, STDOUT_FILENO);
		checkers::io io_black(checkers::pipe_open(black));
		checkers::io io_white(checkers::pipe_open(white));
		std::string line_black;
		std::string line_white;
		std::string move;
		int moves = 0;
	
		io_black << "st " << second << '\n';
		io_white << "st " << second << '\n';
	
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
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					move = line_black.substr(5, 4);
					io << "Black move " << move << '\n'
						<< checkers::io::flush;
					io_white << move << '\n' << checkers::io::flush;
					++moves;
					if (moves > moves_limit)
					{
						io << "***** 1/2-1/2 (Draw) ***\n"
							<< checkers::io::flush;
						return 0;
					}
					break;
				case '*':
					io << line_black << '\n' << checkers::io::flush;
					return 0;
				case 'E':
					io << "Black: " << line_black << '\n'
						<< checkers::io::flush;
					return 255;
				case ' ':
					break;
				default:
					io << "Black: " << line_black << '\n'
						<< checkers::io::flush;
					break;
				}
			}
	
			if (!line_white.empty())
			{
				switch (line_white[0])
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					move = line_white.substr(5, 4);
					io << "White move " << move << '\n'
						<< checkers::io::flush;
					io_black << move << '\n' << checkers::io::flush;
					++moves;
					if (moves > moves_limit)
					{
						io << "***** 1/2-1/2 (Draw) ***\n"
							<< checkers::io::flush;
						return 0;
					}
					break;
				case '*':
					io << line_white << '\n' << checkers::io::flush;
					return 0;
					break;
				case 'E':
					io << "White: " << line_white << '\n'
						<< checkers::io::flush;
					return 255;
				case ' ':
					break;
				default:
					io << "White: " << line_white << '\n'
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
