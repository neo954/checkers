/* $Id: ponder.cpp,v 1.14 2007-11-22 16:30:56 neo Exp $

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
/** @file ponder.cpp
 *  @brief The main program of the checers engine.
 */

#include <iostream>
#include "engine.hpp"
#include "signal.hpp"

/** @mainpage Ponder, a English/American Checkers Game
 *
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date   2006-2007
 *
 *  @section sec_intro Introduction
 *
 *   English/American checkers is played by two people, on opposite sides of a
 *   playing board, alternating moves.  One player has dark pieces, and the
 *   other has light pieces.  The player with the dark pieces makes the first
 *   move unless stated otherwise.  Pieces move diagonally and pieces of the
 *   opponent are captured by jumping over them.  The playable surface consists
 *   only of the dark squares.  A piece may only move into an unoccupied square.
 *   Capturing is mandatory.  A piece that is captured is removed from the
 *   board.  The player who has no pieces left or cannot move anymore has lost
 *   the game unless otherwise stated.
 *
 *   Uncrowned pieces ("men") move one step diagonally forwards and capture
 *   other pieces by making two steps in the same direction, jumping over the
 *   opponent's piece on the intermediate square.  Multiple opposing pieces may
 *   be captured in a single turn provided this is done by successive jumps made
 *   by a single piece; these jumps do not need to be in the same direction but
 *   may zigzag.  Men can only capture forwards.
 *
 *   When men reach the crownhead or kings row (the farthest row forward), they
 *   become kings, marked by placing an additional piece on top of the first,
 *   and acquire additional powers including the ability to move backwards (and
 *   capture backwards, in variants in which they cannot already do so).
 *
 *   http://en.wikipedia.org/wiki/Checkers
 *
 *  @subsection subsec_board Starting Position on a 8 x 8 Checkers Board
 *
 *  @verbatim
        A   B   C   D   E   F   G   H
      +---+---+---+---+---+---+---+---+
   8  |   |(b)|   |(b)|   |(b)|   |(b)|  8  Black
      +---+---+---+---+---+---+---+---+
   7  |(b)|   |(b)|   |(b)|   |(b)|   |  7
      +---+---+---+---+---+---+---+---+
   6  |   |(b)|   |(b)|   |(b)|   |(b)|  6
      +---+---+---+---+---+---+---+---+
   5  | \ |   | \ |   | \ |   | \ |   |  5
      +---+---+---+---+---+---+---+---+
   4  |   | \ |   | \ |   | \ |   | \ |  4
      +---+---+---+---+---+---+---+---+
   3  |(w)|   |(w)|   |(w)|   |(w)|   |  3
      +---+---+---+---+---+---+---+---+
   2  |   |(w)|   |(w)|   |(w)|   |(w)|  2
      +---+---+---+---+---+---+---+---+
   1  |(w)|   |(w)|   |(w)|   |(w)|   |  1  White
      +---+---+---+---+---+---+---+---+
        A   B   C   D   E   F   G   H    @endverbatim

 *
 */
int main(void)
{
	try
	{
		checkers::signal(SIGINT,  SIG_IGN);
		checkers::signal(SIGQUIT, SIG_IGN);
		checkers::signal(SIGABRT, &checkers::crash_handler);
		checkers::signal(SIGBUS,  &checkers::crash_handler);
		checkers::signal(SIGFPE,  &checkers::crash_handler);
		checkers::signal(SIGILL,  &checkers::crash_handler);
		checkers::signal(SIGSEGV, &checkers::crash_handler);
		checkers::signal(SIGTRAP, &checkers::crash_handler);

		checkers::engine::init().run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}

// End of file
