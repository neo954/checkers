/* This file is a part of ponder, a English/American checkers game.

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
/** @file board.cpp
 *  @brief
 *  $Author: neo $
 *  $Date: 2007-11-13 15:48:27 $
 *  $Revision: 1.23 $
 */

#include <cstdlib>
#include "board.hpp"

namespace checkers
{
	board::board(const std::string& input)
	{
		int i;
		bitboard square;
		std::string::size_type p;

		for (i = 0, p = 0; i < 32 && p < input.size(); ++p)
		{
			square = bitboard(0x1 << i);
			switch (input[p])
			{
			case 'B':
				this->_kings |= square;
				// Intentionally no break
			case 'b':
				this->_black_pieces |= square;
				++i;
				break;
			case 'W':
				this->_kings |= square;
				// Intentionally no break
			case 'w':
				this->_white_pieces |= square;
				// Intentionally no break
			case '0':
				// Intentionally no break
			default:
				++i;
				break;
			}
		}	

		p += 2;
		if (p < input.size() && 'b' == input[p])
		{
			this->_player = BLACK;
		}
		else
		{
			this->_player = WHITE;
		}

		this->_zobrist = this->build_zobrist();
	}

	void board::opening(void)
	{
		this->_black_pieces = bitboard(bitboard::BLACK_PIECES_INIT);
		this->_white_pieces = bitboard(bitboard::WHITE_PIECES_INIT);
		this->_kings  = bitboard(bitboard::EMPTY);
		this->_player = BLACK;
		this->_zobrist = this->build_zobrist();
	}

	bool board::is_valid_move(const move& move) const
	{
		std::vector<class move> legal_moves = this->generate_moves();

		return legal_moves.end() !=
			std::find(legal_moves.begin(), legal_moves.end(), move);
	}

	/// @return Whether the same player move one more
	bool board::make_black_move(const move& move)
	{
		this->_black_pieces &= ~move.get_orig();
		this->_zobrist.change_black_piece(move.get_orig());

		this->_black_pieces |= move.get_dest();
		this->_zobrist.change_black_piece(move.get_dest());

		if (this->_kings & move.get_orig())
		{
			this->_kings &= ~move.get_orig();
			this->_zobrist.change_king(move.get_orig());

			this->_kings |= move.get_dest();
			this->_zobrist.change_king(move.get_dest());
		}

		if (move.will_crown())
		{
			this->_kings |= move.get_dest();
			this->_zobrist.change_king(move.get_dest());
		}

		if (move.get_capture())
		{
			this->_white_pieces &= ~move.get_capture();
			this->_zobrist.change_white_piece(move.get_capture());

			if (move.will_capture_a_king())
			{
				this->_kings &= ~move.get_capture();
				this->_zobrist.change_king(move.get_capture());
			}

			if (!move.will_crown() &&
				(move.get_dest() & this->get_black_jumpers()))
			{
				assert(this->build_zobrist() == this->_zobrist);
				return true;
			}
		}

		this->_player = WHITE;
		this->_zobrist.change_side();

		assert(this->build_zobrist() == this->_zobrist);
		return false;
	}

	/// @return Whether the same player move one more
	bool board::make_white_move(const move& move)
	{
		this->_white_pieces &= ~move.get_orig();
		this->_zobrist.change_white_piece(move.get_orig());

		this->_white_pieces |= move.get_dest();
		this->_zobrist.change_white_piece(move.get_dest());

		if (this->_kings & move.get_orig())
		{
			this->_kings &= ~move.get_orig();
			this->_zobrist.change_king(move.get_orig());

			this->_kings |= move.get_dest();
			this->_zobrist.change_king(move.get_dest());
		}

		if (move.will_crown())
		{
			this->_kings |= move.get_dest();
			this->_zobrist.change_king(move.get_dest());
		}

		if (move.get_capture())
		{
			this->_black_pieces &= ~move.get_capture();
			this->_zobrist.change_black_piece(move.get_capture());

			if (move.will_capture_a_king())
			{
				this->_kings &= ~move.get_capture();
				this->_zobrist.change_king(move.get_capture());
			}

			if (!move.will_crown() &&
				(move.get_dest() & this->get_white_jumpers()))
			{
				assert(this->build_zobrist() == this->_zobrist);
				return true;
			}
		}

		this->_player = BLACK;
		this->_zobrist.change_side();

		assert(this->build_zobrist() == this->_zobrist);
		return false;
	}

	void board::undo_black_move(const move& move)
	{
		if (move.get_capture())
		{
			if (move.will_capture_a_king())
			{
				this->_kings |= move.get_capture();
			}
			this->_white_pieces |= move.get_capture();
		}

		if (move.will_crown())
		{
			this->_kings &= ~move.get_dest();
		}

		if (this->_kings & move.get_dest())
		{
			this->_kings &= ~move.get_dest();
			this->_kings |= move.get_orig();
		}
		this->_black_pieces &= ~move.get_dest();
		this->_black_pieces |= move.get_orig();

		this->_player = BLACK;

		assert(this->build_zobrist() == this->_zobrist);
	}

	void board::undo_white_move(const move& move)
	{
		if (move.get_capture())
		{
			if (move.will_capture_a_king())
			{
				this->_kings |= move.get_capture();
			}
			this->_black_pieces |= move.get_capture();
		}

		if (move.will_crown())
		{
			this->_kings &= ~move.get_dest();
		}

		if (this->_kings & move.get_dest())
		{
			this->_kings &= ~move.get_dest();
			this->_kings |= move.get_orig();
		}
		this->_white_pieces &= ~move.get_dest();
		this->_white_pieces |= move.get_orig();

		this->_player = WHITE;

		assert(this->build_zobrist() == this->_zobrist);
	}

	bitboard board::get_black_movers(void) const
	{
		const bitboard not_occupied = this->get_not_occupied();
		const bitboard black_kings = this->get_black_kings();
		bitboard movers = (not_occupied << 4) & this->_black_pieces;
		movers |= ((not_occupied & bitboard::MASK_L3) << 3)
			& this->_black_pieces;
		movers |= ((not_occupied & bitboard::MASK_L5) << 5)
			& this->_black_pieces;

		if (black_kings)
		{
			movers |= (not_occupied >> 4) & black_kings;
			movers |= ((not_occupied & bitboard::MASK_R3) >> 3)
				& black_kings;
			movers |= ((not_occupied & bitboard::MASK_R5) >> 5)
				& black_kings;
		}
		return movers;
	}

	bitboard board::get_white_movers(void) const
	{
		const bitboard not_occupied = this->get_not_occupied();
		const bitboard white_kings = this->get_white_kings();
		bitboard movers = (not_occupied >> 4) & this->_white_pieces;
		movers |= ((not_occupied & bitboard::MASK_R3) >> 3)
			& this->_white_pieces;
		movers |= ((not_occupied & bitboard::MASK_R5) >> 5)
			& this->_white_pieces;

		if (white_kings)
		{
			movers |= (not_occupied << 4) & white_kings;
			movers |= ((not_occupied & bitboard::MASK_L3) << 3)
				& white_kings;
			movers |= ((not_occupied & bitboard::MASK_L5) << 5)
				& white_kings;
		}
		return movers;
	}

	bitboard board::get_black_jumpers(void) const
	{
		const bitboard not_occupied = this->get_not_occupied();
		const bitboard black_kings = this->get_black_kings();
		bitboard movers = bitboard(bitboard::EMPTY);
		// White pieces next to not occupied squares
		bitboard temp = (not_occupied << 4) & this->_white_pieces;
		if (temp)
		{
			movers |= (((temp & bitboard::MASK_L3) << 3)
				| ((temp & bitboard::MASK_L5) << 5))
				& this->_black_pieces;
		}
  		temp = (((not_occupied & bitboard::MASK_L3) << 3) |
			((not_occupied & bitboard::MASK_L5) << 5))
			& this->_white_pieces;
		if (temp)
		{
			movers |= (temp << 4) & this->_black_pieces;
		}

		if (black_kings)
		{
			temp = (not_occupied >> 4) & this->_white_pieces;
      			if (temp)
			{
				movers |= (((temp & bitboard::MASK_R3) >> 3)
					 | ((temp & bitboard::MASK_R5) >> 5))
					 & black_kings;
			}
      			temp = (((not_occupied & bitboard::MASK_R3) >> 3) |
				((not_occupied & bitboard::MASK_R5) >> 5))
				& this->_white_pieces;
      			if (temp)
			{
				movers |= (temp >> 4) & black_kings;
			}
      		}
		return movers;
	}

	bitboard board::get_white_jumpers(void) const
	{
		const bitboard not_occupied = this->get_not_occupied();
		const bitboard white_kings = this->get_white_kings();
		bitboard movers = bitboard(bitboard::EMPTY);
		// Black pieces next to not occupied squares
		bitboard temp = (not_occupied >> 4) & this->_black_pieces;
      		if (temp)
		{
			movers |= (((temp & bitboard::MASK_R3) >> 3)
				 | ((temp & bitboard::MASK_R5) >> 5))
				 & this->_white_pieces;
		}
      		temp = (((not_occupied & bitboard::MASK_R3) >> 3) |
			((not_occupied & bitboard::MASK_R5) >> 5))
			& this->_black_pieces;
      		if (temp)
		{
			movers |= (temp >> 4) & this->_white_pieces;
		}

		if (white_kings)
		{
			temp = (not_occupied << 4) & this->_black_pieces;
			if (temp)
			{
				movers |= (((temp & bitboard::MASK_L3) << 3)
					| ((temp & bitboard::MASK_L5) << 5))
					& white_kings;
			}
  			temp = (((not_occupied & bitboard::MASK_L3) << 3) |
				((not_occupied & bitboard::MASK_L5) << 5))
				& this->_black_pieces;
			if (temp)
			{
				movers |= (temp << 4) & white_kings;
			}
		}
		return movers;
	}

	std::vector<move> board::generate_black_moves(void) const
	{
		std::vector<move> moves;
		moves.reserve(48);
		bitboard black_movers = this->get_black_movers();
		bitboard orig;
		bitboard dest;
		const bitboard not_occupied = this->get_not_occupied();

		while (black_movers)
		{
			orig = black_movers.lsb();
			black_movers &= ~orig;

			dest = (orig >> 4) & not_occupied; 
			if (dest)
			{
				moves.push_back(move(orig, dest,
					bitboard(bitboard::EMPTY), false,
					!(orig & this->_kings) &&
					(dest & bitboard::BLACK_KINGS_ROW)));
			}

			dest = (((orig & bitboard::MASK_R3) >> 3) |
				((orig & bitboard::MASK_R5) >> 5)) &
				not_occupied;
			if (dest)
			{
				moves.push_back(move(orig, dest,
					bitboard(bitboard::EMPTY), false,
					!(orig & this->_kings) &&
					(dest & bitboard::BLACK_KINGS_ROW)));
			}

			if (orig & this->_kings)
			{
				dest = (orig << 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}

				dest = (((orig & bitboard::MASK_L3) << 3) |
					((orig & bitboard::MASK_L5) << 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}
			}
		}

		return moves;
	}

	std::vector<move> board::generate_white_moves(void) const
	{
		std::vector<move> moves;
		moves.reserve(48);
		bitboard white_movers = this->get_white_movers();
		bitboard orig;
		bitboard dest;
		const bitboard not_occupied = this->get_not_occupied();

		while (white_movers)
		{
			orig = white_movers.lsb();
			white_movers &= ~orig;

			dest = (orig << 4) & not_occupied; 
			if (dest)
			{
				moves.push_back(move(orig, dest,
					bitboard(bitboard::EMPTY), false,
					!(orig & this->_kings) &&
					(dest & bitboard::WHITE_KINGS_ROW)));
			}

			dest = (((orig & bitboard::MASK_L3) << 3) |
				((orig & bitboard::MASK_L5) << 5)) &
				not_occupied;
			if (dest)
			{
				moves.push_back(move(orig, dest,
					bitboard(bitboard::EMPTY), false,
					!(orig & this->_kings) &&
					(dest & bitboard::WHITE_KINGS_ROW)));
			}

			if (orig & this->_kings)
			{
				dest = (orig >> 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}

				dest = (((orig & bitboard::MASK_R3) >> 3) |
					((orig & bitboard::MASK_R5) >> 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}
			}
		}

		return moves;
	}

	std::vector<move> board::generate_black_jumps(void) const
	{
		std::vector<move> moves;
		moves.reserve(48);
		bitboard black_jumpers = this->get_black_jumpers();
		bitboard orig;
		bitboard dest;
		bitboard capture;
		const bitboard not_occupied = this->get_not_occupied();

		while (black_jumpers)
		{
			orig = black_jumpers.lsb();
			black_jumpers &= ~orig;

			capture = (orig >> 4) & this->_white_pieces;
			if (capture)
			{
				dest = (((capture & bitboard::MASK_R3) >> 3) |
					((capture & bitboard::MASK_R5) >> 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						capture, capture & this->_kings,
						!(orig & this->_kings) && (dest
						& bitboard::BLACK_KINGS_ROW)));
				}
			}

			capture = (((orig & bitboard::MASK_R3) >> 3) |
				((orig & bitboard::MASK_R5) >> 5)) &
				this->_white_pieces;
			if (capture)
			{
				dest = (capture >> 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						capture, capture & this->_kings,
						!(orig & this->_kings) && (dest
						& bitboard::BLACK_KINGS_ROW)));
				}
			}

			if (orig & this->_kings)
			{
				capture = (orig << 4) & this->_white_pieces;
				if (capture)
				{
					dest = (((capture & bitboard::MASK_L3)
							<< 3) |
						((capture & bitboard::MASK_L5)
							<< 5)) & not_occupied;
					if (dest)
					{
						moves.push_back(move(
							orig, dest, capture,
							capture & this->_kings,
							false));
					}
				}

				capture = (((orig & bitboard::MASK_L3) << 3) |
					((orig & bitboard::MASK_L5) << 5)) &
					this->_white_pieces;
				if (capture)
				{
					dest = (capture << 4) & not_occupied;
					if (dest)
					{
						moves.push_back(move(
							orig, dest, capture,
							capture & this->_kings,
							false));
					}
				}
			}
		}

		return moves;
	}

	std::vector<move> board::generate_white_jumps(void) const
	{
		std::vector<move> moves;
		moves.reserve(48);
		bitboard white_jumpers = this->get_white_jumpers();
		bitboard orig;
		bitboard dest;
		bitboard capture;
		const bitboard not_occupied = this->get_not_occupied();

		while (white_jumpers)
		{
			orig = white_jumpers.lsb();
			white_jumpers &= ~orig;

			capture = (orig << 4) & this->_black_pieces;
			if (capture)
			{
				dest = (((capture & bitboard::MASK_L3) << 3) |
					((capture & bitboard::MASK_L5) << 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						capture, capture & this->_kings,
						!(orig & this->_kings) && (dest
						& bitboard::WHITE_KINGS_ROW)));
				}
			}

			capture = (((orig & bitboard::MASK_L3) << 3) |
				((orig & bitboard::MASK_L5) << 5)) &
				this->_black_pieces;
			if (capture)
			{
				dest = (capture << 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest,
						capture, capture & this->_kings,
						!(orig & this->_kings) && (dest
						& bitboard::WHITE_KINGS_ROW)));
				}
			}

			if (orig & this->_kings)
			{
				capture = (orig >> 4) & this->_black_pieces;
				if (capture)
				{
					dest = (((capture & bitboard::MASK_R3)
							>> 3) |
						((capture & bitboard::MASK_R5)
							>> 5)) & not_occupied;
					if (dest)
					{
						moves.push_back(move( 
							orig, dest, capture,
							capture & this->_kings,
							false));
					}
				}

				capture = (((orig & bitboard::MASK_R3) >> 3) |
					((orig & bitboard::MASK_R5) >> 5)) &
					this->_black_pieces;
				if (capture)
				{
					dest = (capture >> 4) & not_occupied;
					if (dest)
					{
						moves.push_back(move(
							orig, dest, capture,
							capture & this->_kings,
							false));
					}
				}
			}
		}

		return moves;
	}

	move board::generate_move(const std::string& str) const
	{
		if (4 != str.length())
		{
			throw std::logic_error("Error (illegal move,"
				" wrong command length): " + str);
		}

		bitboard orig(str[0], str[1]);
		bitboard dest(str[2], str[3]);
		bitboard capture = bitboard(bitboard::EMPTY);
		if (2 == abs(str[2] - str[0]))
		{
			capture = bitboard((str[0] + str[2]) / 2,
				(str[1] + str[3]) / 2);
		}
		
		bool will_capture_a_king = capture & this->_kings;
		bool will_crown = !(orig & this->_kings) &&
			(this->is_black_move() ?
				dest & bitboard::BLACK_KINGS_ROW :
				dest & bitboard::WHITE_KINGS_ROW);

		move move(orig, dest, capture, will_capture_a_king, will_crown);
		if (!this->is_valid_move(move))
		{	
			throw std::logic_error("Error (illegal move): " + str);
		}

		return move;
	}

	std::ostream& operator <<(std::ostream& os, const board& rhs)
	{
		int i;
		bitboard square;

		for (i = 0; i < 32; ++i)
		{
			if (i > 0 && 0 == i % 4)
			{
				os << '/';
			}
			square = bitboard(0x1 << i);
			if (rhs._black_pieces & square)
			{
				if (rhs._kings & square)
				{
					os << 'B';
				}
				else
				{
					os << 'b';
				}
			}
			else if (rhs._white_pieces & square)
			{
				if (rhs._kings & square)
				{
					os << 'W';
				}
				else
				{
					os << 'w';
				}
			}
			else
			{
				os << '0';
			}
		}
		os << ' ';
		os << (rhs.is_black_move() ? 'b' : 'w');

		return os;
	}

	// ================================================================

	zobrist board::build_zobrist(void)
	{
		zobrist zobrist;

		bitboard pieces;
		bitboard piece;

		pieces = this->get_black_pieces();
		while (pieces)
		{
			piece = pieces.lsb();
			pieces &= ~piece;

			zobrist.change_black_piece(piece);
		}

		pieces = this->get_white_pieces();
		while (pieces)
		{
			piece = pieces.lsb();
			pieces &= ~piece;

			zobrist.change_white_piece(piece);
		}

		pieces = this->get_kings();
		while(pieces)
		{
			piece = pieces.lsb();
			pieces &= ~piece;

			zobrist.change_king(piece);
		}

		if (this->is_white_move())
		{
			zobrist.change_side();
		}

		return zobrist;
	}
}

// End of file
