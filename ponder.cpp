/* $Id$

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007, 2008, 2009 Quux Information.
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
/** @file ponder.cpp
 *  @brief The main program of the checers engine.
 */

#include <iostream>
#include "engine.hpp"
#include "signal.hpp"

/** @mainpage Ponder, a English/American Checkers Game
 *
 *  @author Gong Jie <neo@quux.me>
 *  @date   2006 - 2009, 2014
 *
 *  @section sec_intro Introduction
 *
 *   English draughts, also called American checkers or "straight checkers",
 *   commonly called checkers in the U.S., but commonly called draughts in
 *   some other countries, is a form of the draughts board game played on an
 *   8×8 board with 12 pieces on each side that may only move and capture
 *   forward.
 *
 *  @section sec_rules Rules
 *
 *   As in all draughts variants, English draughts is played by two people,
 *   on opposite sides of a playing board, alternating moves.  One player has
 *   dark pieces, and the other has light pieces. Pieces move diagonally and
 *   pieces of the opponent are captured by jumping over them.
 *
 *   The rules of this variant of draughts are:
 *
 *  @subsection subsec_board Board
 *
 *   The board is an 8x8 grid, with alternating dark and light squares, called
 *   a checkerboard (in the US, in reference to its checkered pattern, also the
 *   source of the name checkers).  The playable surface consists of the 32
 *   dark squares only.  A consequence of this is that, from each player's
 *   perspective, the left and right corners encourage different strategies.
 *
 *  @subsection subsec_pieces Pieces
 *
 *   The pieces are usually made of wood and are flat and cylindrical.  They
 *   are invariably split into one darker and one lighter color.  Traditionally,
 *   these colors are red and white.  There are two kinds of pieces: "men" and
 *   "kings".  Kings are differentiated as consisting of two normal pieces of
 *   the same color, stacked one on top of the other.  Often indentations are
 *   added to the pieces to aid stacking.
 *
 *  @subsection subsec_start Starting Position
 *
 *   Each player starts with 12 pieces on the three rows closest to their own
 *   side, as shown in the diagram.  The row closest to each player is called
 *   the "crownhead" or "kings row".  The black (darker color) side moves first.
 *
 *  @subsection subsec_move How to Move
 *
 *   There are two ways to move a piece: simply sliding a piece diagonally
 *   forwards (also diagonally backwards in the case of kings) to an adjacent
 *   and unoccupied dark square, or "jumping" one of the opponent's pieces.  In
 *   this case, one piece "jumps over" the other, provided there is a vacant
 *   square on the opposite side for it to land on.  Again, a man (uncrowned
 *   piece) can only jump diagonally forwards, and a king can also move
 *   diagonally backwards.  A piece that is jumped is captured and removed from
 *   the board.  Multiple-jump moves are possible if, when the jumping piece
 *   lands, there is another piece that can be jumped.  Jumping is mandatory and
 *   cannot be passed up to make a non-jumping move, nor can fewer than the
 *   maximum jumps possible be taken in a multiple-jump move.  When there is
 *   more than one way for a player to jump, one may choose which sequence to
 *   make, not necessarily the sequence that will result in the most amount of
 *   captures.  However, one must make all the captures in that sequence.
 *   (Under traditional draughts rules jumping is not mandatory.  If it is not
 *   done, the opponent may either force the move to be reversed, huff the
 *   piece or carry on regardless.)
 *
 *  @subsection subsec_kings Kings
 *
 *   If a player's piece moves into the kings row on the opposing player's side
 *   of the board, that piece is said to be "crowned" (or often "kinged" in the
 *   US), becoming a "king" and gaining the ability to move both forwards and
 *   backwards.  If a player's piece jumps into the kings row, the move
 *   terminates (it cannot jump out (as in a multiple-jump move) until that move
 *   has ended and the piece has been crowned).
 *
 *  @subsection subsec_ends How the Game Ends
 *
 *   A player wins by capturing all of the opposing player's pieces, or by
 *   leaving the opposing player with no legal moves.
 *
 *   http://en.wikipedia.org/wiki/English_draughts
 *
 *  @verbatim
     +---+---+---+---+---+---+---+---+
     |   |(w)|   |(w)|   |(w)|   |(w)| White
     +---+32-+---+31-+---+30-+---+29-+
     |(w)|   |(w)|   |(w)|   |(w)|   |
     +28-+---+27-+---+26-+---+25-+---+
     |   |(w)|   |(w)|   |(w)|   |(w)|
     +---+24-+---+23-+---+22-+---+21-+
     | \ |   | \ |   | \ |   | \ |   |
     +20-+---+19-+---+18-+---+17-+---+
     |   | \ |   | \ |   | \ |   | \ |
     +---+16-+---+15-+---+14-+---+13-+
     |(b)|   |(b)|   |(b)|   |(b)|   |
     +12-+---+11-+---+10-+---+9--+---+
     |   |(b)|   |(b)|   |(b)|   |(b)|
     +---+8--+---+7--+---+6--+---+5--+
     |(b)|   |(b)|   |(b)|   |(b)|   | Black
     +4--+---+3--+---+2--+---+1--+---+

     English draughts board with all pieces on starting position @endverbatim
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
	catch (...)
	{
		std::cerr << "Fatal error: unknown exception!" << std::endl;
		return 1;
	}

	return 0;
}

// End of file
