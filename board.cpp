/* $Id: board.cpp,v 1.35 2007-11-26 15:20:21 neo Exp $

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
/** @file board.cpp
 *  @brief Checkers game board.
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
			square = bitboard(0x1U << i);
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
				++i;
				break;
			case '/':
			default:
				break;
			}
		}	

		// Black crown
		this->_kings |= this->_black_pieces & bitboard::BLACK_KINGS_ROW;
		// White crown
		this->_kings |= this->_white_pieces & bitboard::WHITE_KINGS_ROW;

		++p;
		if (p < input.size() && 'w' == input[p])
		{
			this->_player = board::WHITE;
		}
		else
		{
			this->_player = board::BLACK;
		}

		this->_zobrist = this->build_zobrist();
	}

	void board::opening(void)
	{
		this->_black_pieces = bitboard(bitboard::BLACK_PIECES_INIT);
		this->_white_pieces = bitboard(bitboard::WHITE_PIECES_INIT);
		this->_kings  = bitboard(bitboard::EMPTY);
		this->_player = board::BLACK;
		this->_zobrist = this->build_zobrist();
	}

	bool board::is_valid_move(const move& move) const
	{
		std::vector<class move> legal_moves = this->generate_moves();

		return legal_moves.end() !=
			std::find(legal_moves.begin(), legal_moves.end(), move);
	}

	/** @return whether the player has dark pieces move once more.
	 */
	bool board::make_black_move(const move& move)
	{
		this->_black_pieces &= ~move.get_src();
		this->_zobrist.change_black_piece(move.get_src());

		this->_black_pieces |= move.get_dest();
		this->_zobrist.change_black_piece(move.get_dest());

		if (this->_kings & move.get_src())
		{
			this->_kings &= ~move.get_src();
			this->_zobrist.change_king(move.get_src());

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
				/** @retval true when the player has dark
				 *   pieces jumps once more (Capture multiple
				 *   opposing pieces in a single turn).
				 */
				return true;
			}
		}

		this->_player = board::WHITE;
		this->_zobrist.change_side();

		assert(this->build_zobrist() == this->_zobrist);
		/// @retval false when change side.
		return false;
	}

	/** @return whether the player has light pieces move once more.
	 */
	bool board::make_white_move(const move& move)
	{
		this->_white_pieces &= ~move.get_src();
		this->_zobrist.change_white_piece(move.get_src());

		this->_white_pieces |= move.get_dest();
		this->_zobrist.change_white_piece(move.get_dest());

		if (this->_kings & move.get_src())
		{
			this->_kings &= ~move.get_src();
			this->_zobrist.change_king(move.get_src());

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
				/** @retval true when the player has light
				 *   pieces jumps once more (Capture multiple
				 *   opposing pieces in a single turn).
				 */
				return true;
			}
		}

		this->_player = board::BLACK;
		this->_zobrist.change_side();

		assert(this->build_zobrist() == this->_zobrist);
		/// @retval false when change side.
		return false;
	}

	void board::undo_black_move(const move& move)
	{
		if (this->is_white_to_move())
		{
			this->_player = board::BLACK;
			this->_zobrist.change_side();
		}

		if (move.get_capture())
		{
			if (move.will_capture_a_king())
			{
				this->_kings |= move.get_capture();
				this->_zobrist.change_king(move.get_capture());
			}

			this->_white_pieces |= move.get_capture();
			this->_zobrist.change_white_piece(move.get_capture());
		}

		if (move.will_crown())
		{
			this->_kings &= ~move.get_dest();
			this->_zobrist.change_king(move.get_dest());
		}

		if (this->_kings & move.get_dest())
		{
			this->_kings &= ~move.get_dest();
			this->_zobrist.change_king(move.get_dest());

			this->_kings |= move.get_src();
			this->_zobrist.change_king(move.get_src());
		}

		this->_black_pieces &= ~move.get_dest();
		this->_zobrist.change_black_piece(move.get_dest());

		this->_black_pieces |= move.get_src();
		this->_zobrist.change_black_piece(move.get_src());

		assert(this->build_zobrist() == this->_zobrist);
	}

	void board::undo_white_move(const move& move)
	{
		if (this->is_black_to_move())
		{
			this->_player = board::WHITE;
			this->_zobrist.change_side();
		}

		if (move.get_capture())
		{
			if (move.will_capture_a_king())
			{
				this->_kings |= move.get_capture();
				this->_zobrist.change_king(move.get_capture());
			}
			this->_black_pieces |= move.get_capture();
			this->_zobrist.change_black_piece(move.get_capture());
		}

		if (move.will_crown())
		{
			this->_kings &= ~move.get_dest();
			this->_zobrist.change_king(move.get_dest());
		}

		if (this->_kings & move.get_dest())
		{
			this->_kings &= ~move.get_dest();
			this->_zobrist.change_king(move.get_dest());

			this->_kings |= move.get_src();
			this->_zobrist.change_king(move.get_src());
		}

		this->_white_pieces &= ~move.get_dest();
		this->_zobrist.change_white_piece(move.get_dest());

		this->_white_pieces |= move.get_src();
		this->_zobrist.change_white_piece(move.get_src());

		assert(this->build_zobrist() == this->_zobrist);
	}

	/** @return whether the same player move once more.
	 */
	bool board::make_move(const move& move)
	{
		assert(this->is_valid_move(move));

		/** @retval true if multiple opposing pieces may be captured in
		 *   a single turn, and this move is not the last move in this
		 *   turn.  The same player will make move once more.
		 *  @retval false if the other player will make the next move.
		 */
		return this->is_black_to_move() ?
			this->make_black_move(move) :
			this->make_white_move(move);
	}

	void board::undo_move(const move& move)
	{
		if (move.get_dest() & this->_black_pieces)
		{
			this->undo_black_move(move);
		}
		else
		{
			this->undo_white_move(move);
		}

		assert(this->is_valid_move(move));
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
		moves.reserve(42);
		bitboard black_movers = this->get_black_movers();
		bitboard src;
		bitboard dest;
		const bitboard not_occupied = this->get_not_occupied();

		while (black_movers)
		{
			src = black_movers.lsb();
			black_movers &= ~src;

			dest = (src >> 4) & not_occupied; 
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::BLACK_KINGS_ROW)));
			}

			dest = (((src & bitboard::MASK_R3) >> 3) |
				((src & bitboard::MASK_R5) >> 5)) &
				not_occupied;
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::BLACK_KINGS_ROW)));
			}

			if (src & this->_kings)
			{
				dest = (src << 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}

				dest = (((src & bitboard::MASK_L3) << 3) |
					((src & bitboard::MASK_L5) << 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
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
		moves.reserve(42);
		bitboard white_movers = this->get_white_movers();
		bitboard src;
		bitboard dest;
		const bitboard not_occupied = this->get_not_occupied();

		while (white_movers)
		{
			src = white_movers.lsb();
			white_movers &= ~src;

			dest = (src << 4) & not_occupied; 
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::WHITE_KINGS_ROW)));
			}

			dest = (((src & bitboard::MASK_L3) << 3) |
				((src & bitboard::MASK_L5) << 5)) &
				not_occupied;
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::WHITE_KINGS_ROW)));
			}

			if (src & this->_kings)
			{
				dest = (src >> 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}

				dest = (((src & bitboard::MASK_R3) >> 3) |
					((src & bitboard::MASK_R5) >> 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
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
		moves.reserve(42);
		bitboard black_jumpers = this->get_black_jumpers();
		bitboard src;
		bitboard dest;
		bitboard capture;
		const bitboard not_occupied = this->get_not_occupied();

		while (black_jumpers)
		{
			src = black_jumpers.lsb();
			black_jumpers &= ~src;

			capture = (src >> 4) & this->_white_pieces;
			if (capture)
			{
				dest = (((capture & bitboard::MASK_R3) >> 3) |
					((capture & bitboard::MASK_R5) >> 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						capture, capture & this->_kings,
						!(src & this->_kings) && (dest
						& bitboard::BLACK_KINGS_ROW)));
				}
			}

			capture = (((src & bitboard::MASK_R3) >> 3) |
				((src & bitboard::MASK_R5) >> 5)) &
				this->_white_pieces;
			if (capture)
			{
				dest = (capture >> 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						capture, capture & this->_kings,
						!(src & this->_kings) && (dest
						& bitboard::BLACK_KINGS_ROW)));
				}
			}

			if (src & this->_kings)
			{
				capture = (src << 4) & this->_white_pieces;
				if (capture)
				{
					dest = (((capture & bitboard::MASK_L3)
							<< 3) |
						((capture & bitboard::MASK_L5)
							<< 5)) & not_occupied;
					if (dest)
					{
						moves.push_back(move(
							src, dest, capture,
							capture & this->_kings,
							false));
					}
				}

				capture = (((src & bitboard::MASK_L3) << 3) |
					((src & bitboard::MASK_L5) << 5)) &
					this->_white_pieces;
				if (capture)
				{
					dest = (capture << 4) & not_occupied;
					if (dest)
					{
						moves.push_back(move(
							src, dest, capture,
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
		moves.reserve(42);
		bitboard white_jumpers = this->get_white_jumpers();
		bitboard src;
		bitboard dest;
		bitboard capture;
		const bitboard not_occupied = this->get_not_occupied();

		while (white_jumpers)
		{
			src = white_jumpers.lsb();
			white_jumpers &= ~src;

			capture = (src << 4) & this->_black_pieces;
			if (capture)
			{
				dest = (((capture & bitboard::MASK_L3) << 3) |
					((capture & bitboard::MASK_L5) << 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						capture, capture & this->_kings,
						!(src & this->_kings) && (dest
						& bitboard::WHITE_KINGS_ROW)));
				}
			}

			capture = (((src & bitboard::MASK_L3) << 3) |
				((src & bitboard::MASK_L5) << 5)) &
				this->_black_pieces;
			if (capture)
			{
				dest = (capture << 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						capture, capture & this->_kings,
						!(src & this->_kings) && (dest
						& bitboard::WHITE_KINGS_ROW)));
				}
			}

			if (src & this->_kings)
			{
				capture = (src >> 4) & this->_black_pieces;
				if (capture)
				{
					dest = (((capture & bitboard::MASK_R3)
							>> 3) |
						((capture & bitboard::MASK_R5)
							>> 5)) & not_occupied;
					if (dest)
					{
						moves.push_back(move( 
							src, dest, capture,
							capture & this->_kings,
							false));
					}
				}

				capture = (((src & bitboard::MASK_R3) >> 3) |
					((src & bitboard::MASK_R5) >> 5)) &
					this->_black_pieces;
				if (capture)
				{
					dest = (capture >> 4) & not_occupied;
					if (dest)
					{
						moves.push_back(move(
							src, dest, capture,
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
			/** @throw std::logic_error when @e str has wrong
			 *   length.
			 */
			throw std::logic_error("Error (illegal move,"
				" wrong length): " + str);
		}

		bitboard src(str[0], str[1]);
		bitboard dest(str[2], str[3]);
		bitboard capture = bitboard(bitboard::EMPTY);
		if (2 == abs(str[2] - str[0]))
		{
			capture = bitboard((str[0] + str[2]) / 2,
				(str[1] + str[3]) / 2);
		}
		
		bool will_capture_a_king = capture & this->_kings;
		bool will_crown = !(src & this->_kings) &&
			(this->is_black_to_move() ?
				dest & bitboard::BLACK_KINGS_ROW :
				dest & bitboard::WHITE_KINGS_ROW);

		move move(src, dest, capture, will_capture_a_king, will_crown);
		if (!this->is_valid_move(move))
		{	
			/** @throw std::logic_error when @e str is an
			 *   illegal move.
			 */
			throw std::logic_error("Error (illegal move,"
				" violation rules): " + str);
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
			square = bitboard(0x1U << i);
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
		os << (rhs.is_black_to_move() ? 'b' : 'w');

		return os;
	}

	std::vector<move> board::generate_moves(void) const
	{
		return (this->is_black_to_move()) ?
			(this->get_black_jumpers() ?
				this->generate_black_jumps() :
				this->generate_black_moves()) :
			(this->get_white_jumpers() ?
				this->generate_white_jumps() :
				this->generate_white_moves());
	}

	void board::set_black_to_move(void)
	{
		if (this->is_white_to_move())
		{
			this->_player = board::BLACK;
			this->_zobrist.change_side();
		}
	}

	void board::set_white_to_move(void)
	{
		if (this->is_black_to_move())
		{
			this->_player = board::WHITE;
			this->_zobrist.change_side();
		}
	}

	/** @return game over or not.
	 *  @retval true when the current player is winning.
	 *  @retval false in other situation.
	 */
	bool board::is_winning(void) const
	{
		return this->is_black_to_move() ?
			!this->get_white_pieces() :
			!this->get_black_pieces();
	}

	/** @return game over or not.
	 *  @retval true when the current player is losing.
	 *  @retval false in other situation.
	 */
	bool board::is_losing(void) const
	{
		return this->is_black_to_move() ?
			!(this->get_black_jumpers() ||
			  this->get_black_movers()) :
			!(this->get_white_jumpers() ||
			  this->get_white_movers());
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

		if (this->is_white_to_move())
		{
			zobrist.change_side();
		}

		return zobrist;
	}
}

// End of file
