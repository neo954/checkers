/* $Id: board.cpp,v 1.45 2010-11-19 08:26:25 neo Exp $

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
/** @file board.cpp
 *  @brief Checkers game board.
 */

#include <algorithm>
#include <cstdlib>
#include <sstream>
#include "board.hpp"

namespace checkers
{
	/** @param str The position of checkers board in FEN.
	 *  A position can be stored by the FEN tag:
	 *  @verbatim [Turn]:[Color 1][[K][Square number][,]...]:[Color 2][[K][Square number][,]...] @endverbatim
	 *  @par Turn
	 *   the side to move, B for Black, W for White.
	 *  @par Color 1 and Color 2
	 *   the color for the Square numbers that follow B for Black, W, and
	 *   the sequence is unimportant.
	 *  @par K
	 *   optional before square number, indicates the piece on that square
	 *   is a king, otherwise it is a man.
	 *  @par Square number
	 *   indicates the square number occupied by a piece.  The square number
	 *   must be at least a "1".  On an 8x8 board, it has a range from 1-32.
	 *   These are comma separated, and the sequence is unimportant.
	 *  @par Examples:
	 *  @verbatim B:W18,24,27,28,K10,K15:B12,16,20,K22,K25,K29 @endverbatim
	 *  @verbatim B:W18,19,21,23,24,26,29,30,31,32:B1,2,3,4,6,7,9,10,11,12 @endverbatim
	 */
	board::board(const std::string& str) :
		_black_pieces(), _white_pieces(), _kings(), _player(),
		_zobrist()
	{
		if (str.empty())
		{
			/** @throw std::logic_error while @e str has wrong
			 *   format.
			 */
			throw std::logic_error("Error (illegal FEN,"
				" empty): " + str);
		}

		switch (str[0])
		{
		case 'B':
			this->_player = board::BLACK;
			break;
		case 'W':
			this->_player = board::WHITE;
			break;
		default:
			std::ostringstream error;
			error << "Error (illegal FEN, unexpected character"
				" `" << str[0] << "'): " << str;
			throw std::logic_error(error.str());
			break;
		}

		if (str.size() > 1 && str[1] != ':')
		{
			std::ostringstream error;
			error << "Error (illegal FEN, unexpected character"
				" `" << str[1] << "'): " << str;
			throw std::logic_error(error.str());
		}

		bitboard a_piece;
		std::string::size_type p;
		player color = board::BLACK;
		bool is_king = false;
		unsigned int i = 0;
		std::string::size_type str_size = str.size();
		for (p = 2; p < str_size; ++p)
		{
			switch (str[p])
			{
			case 'B':
				if (':' != str[p - 1])
				{
					std::ostringstream error;
					error << "Error (illegal FEN,"
						" unexpected character `B'"
						" after `" << str[p - 1] << "'"
						"): " << str;
					throw std::logic_error(error.str());
				}
				color = board::BLACK;
				break;
			case 'W':
				if (':' != str[p - 1])
				{
					std::ostringstream error;
					error << "Error (illegal FEN,"
						" unexpected character `W'"
						" after `" << str[p - 1] << "'"
						"): " << str;
					throw std::logic_error(error.str());
				}
				color = board::WHITE;
				break;
			case ':':
				if (p + 1 >= str_size ||
					('B' != str[p + 1] && 'W' != str[p + 1]))
				{
					std::ostringstream error;
					error << "Error (illegal FEN,"
						" unexpected character"
						" `" << str[p + 1] << "'"
						" after `:'): " << str;
					throw std::logic_error(error.str());
				}
				// Intentionally no break
				goto parse_piece;
			case ',':
				if (str[p - 1] < '0' || str[p - 1] > '9')
				{
					std::ostringstream error;
					error << "Error (illegal FEN,"
						" unexpected character `,'"
						" after `" << str[p - 1] << "'"
						"): " << str;
					throw std::logic_error(error.str());
				}
			parse_piece:
				if (0 == i || i > 32)
				{
					std::ostringstream error;
					error << "Error (illegal FEN,"
						" square number `" << i << "'"
						" out of range): " << str;
					throw std::logic_error(error.str());
				}
				a_piece = bitboard(0x1U << (i - 1));
				if ((this->_black_pieces | this->_white_pieces)
					& a_piece)
				{
					std::ostringstream error;
					error << "Error (illegal FEN,"
						" square number `" << i << "'"
						" repeated): " << str;
					throw std::logic_error(error.str());
				}
				if (board::BLACK == color)
				{
					this->_black_pieces |= a_piece;
				}
				else
				{
					this->_white_pieces |= a_piece;
				}
				if (is_king)
				{
					this->_kings |= a_piece;
				}
				is_king = false;
				i = 0;
				break;
			case 'K':
				if (',' != str[p - 1] && 'B' != str[p - 1] &&
					'W' != str[p - 1])
				{
					std::ostringstream error;
					error << "Error (illegal FEN,"
						" unexpected character `K'"
						" after `" << str[p - 1] << "'"
						"): " << str;
					throw std::logic_error(error.str());
				}
				is_king = true;
				break;
			case '0':
				// Fall through
			case '1':
				// Fall through
			case '2':
				// Fall through
			case '3':
				// Fall through
			case '4':
				// Fall through
			case '5':
				// Fall through
			case '6':
				// Fall through
			case '7':
				// Fall through
			case '8':
				// Fall through
			case '9':
				i *= 10;
				i += (str[p] - '0');
				break;
			default:
				std::ostringstream error;
				error << "Error (illegal FEN, unexpected"
					" character `" << str[p] << "'"
					"): " << str;
				throw std::logic_error(error.str());
				break;
			}
		} // for
		if (0 == i || i > 32)
		{
			std::ostringstream error;
			error << "Error (illegal FEN, square number"
				" `" << i << "' out of range): " << str;
			throw std::logic_error(error.str());
		}
		a_piece = bitboard(0x1U << (i - 1));
		if ((this->_black_pieces | this->_white_pieces) & a_piece)
		{
			std::ostringstream error;
			error << "Error (illegal FEN, square number"
				" `" << i << "' repeated): " << str;
			throw std::logic_error(error.str());
		}
		if (board::BLACK == color)
		{
			this->_black_pieces |= a_piece;
		}
		else
		{
			this->_white_pieces |= a_piece;
		}
		if (is_king)
		{
			this->_kings |= a_piece;
		}

		this->_zobrist = this->build_zobrist();

		assert(!(this->_black_pieces & this->_white_pieces));
		assert(((this->_black_pieces | this->_white_pieces) &
			this->_kings) == this->_kings);
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
				assert(!(this->_black_pieces &
					this->_white_pieces));
				assert(((this->_black_pieces |
					this->_white_pieces) &
					this->_kings) == this->_kings);
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

		assert(!(this->_black_pieces & this->_white_pieces));
		assert(((this->_black_pieces | this->_white_pieces) &
			this->_kings) == this->_kings);
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
				assert(!(this->_black_pieces &
					this->_white_pieces));
				assert(((this->_black_pieces |
					this->_white_pieces) &
					this->_kings) == this->_kings);
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

		assert(!(this->_black_pieces & this->_white_pieces));
		assert(((this->_black_pieces | this->_white_pieces) &
			this->_kings) == this->_kings);
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

		assert(!(this->_black_pieces & this->_white_pieces));
		assert(((this->_black_pieces | this->_white_pieces) &
			this->_kings) == this->_kings);
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

		assert(!(this->_black_pieces & this->_white_pieces));
		assert(((this->_black_pieces | this->_white_pieces) &
			this->_kings) == this->_kings);
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
		const bitboard unoccupied = this->get_unoccupied();
		const bitboard black_kings = this->get_black_kings();
		bitboard movers = (unoccupied >> 4) & this->_black_pieces;
		movers |= ((unoccupied & bitboard::MASK_R3) >> 3)
			& this->_black_pieces;
		movers |= ((unoccupied & bitboard::MASK_R5) >> 5)
			& this->_black_pieces;

		if (black_kings)
		{
			movers |= (unoccupied << 4) & black_kings;
			movers |= ((unoccupied & bitboard::MASK_L3) << 3)
				& black_kings;
			movers |= ((unoccupied & bitboard::MASK_L5) << 5)
				& black_kings;
		}
		return movers;
	}

	bitboard board::get_white_movers(void) const
	{
		const bitboard unoccupied = this->get_unoccupied();
		const bitboard white_kings = this->get_white_kings();
		bitboard movers = (unoccupied << 4) & this->_white_pieces;
		movers |= ((unoccupied & bitboard::MASK_L3) << 3)
			& this->_white_pieces;
		movers |= ((unoccupied & bitboard::MASK_L5) << 5)
			& this->_white_pieces;

		if (white_kings)
		{
			movers |= (unoccupied >> 4) & white_kings;
			movers |= ((unoccupied & bitboard::MASK_R3) >> 3)
				& white_kings;
			movers |= ((unoccupied & bitboard::MASK_R5) >> 5)
				& white_kings;
		}
		return movers;
	}

	bitboard board::get_black_jumpers(void) const
	{
		const bitboard unoccupied = this->get_unoccupied();
		const bitboard black_kings = this->get_black_kings();
		bitboard movers = bitboard(bitboard::EMPTY);
		// White pieces next to not occupied squares
		bitboard temp = (unoccupied >> 4) & this->_white_pieces;
		if (temp)
		{
			movers |= (((temp & bitboard::MASK_R3) >> 3)
				| ((temp & bitboard::MASK_R5) >> 5))
				& this->_black_pieces;
		}
  		temp = (((unoccupied & bitboard::MASK_R3) >> 3) |
			((unoccupied & bitboard::MASK_R5) >> 5))
			& this->_white_pieces;
		if (temp)
		{
			movers |= (temp >> 4) & this->_black_pieces;
		}

		if (black_kings)
		{
			temp = (unoccupied << 4) & this->_white_pieces;
      			if (temp)
			{
				movers |= (((temp & bitboard::MASK_L3) << 3)
					 | ((temp & bitboard::MASK_L5) << 5))
					 & black_kings;
			}
      			temp = (((unoccupied & bitboard::MASK_L3) << 3) |
				((unoccupied & bitboard::MASK_L5) << 5))
				& this->_white_pieces;
      			if (temp)
			{
				movers |= (temp << 4) & black_kings;
			}
      		}
		return movers;
	}

	bitboard board::get_white_jumpers(void) const
	{
		const bitboard unoccupied = this->get_unoccupied();
		const bitboard white_kings = this->get_white_kings();
		bitboard movers = bitboard(bitboard::EMPTY);
		// Black pieces next to not occupied squares
		bitboard temp = (unoccupied << 4) & this->_black_pieces;
      		if (temp)
		{
			movers |= (((temp & bitboard::MASK_L3) << 3)
				 | ((temp & bitboard::MASK_L5) << 5))
				 & this->_white_pieces;
		}
      		temp = (((unoccupied & bitboard::MASK_L3) << 3) |
			((unoccupied & bitboard::MASK_L5) << 5))
			& this->_black_pieces;
      		if (temp)
		{
			movers |= (temp << 4) & this->_white_pieces;
		}

		if (white_kings)
		{
			temp = (unoccupied >> 4) & this->_black_pieces;
			if (temp)
			{
				movers |= (((temp & bitboard::MASK_R3) >> 3)
					| ((temp & bitboard::MASK_R5) >> 5))
					& white_kings;
			}
  			temp = (((unoccupied & bitboard::MASK_R3) >> 3) |
				((unoccupied & bitboard::MASK_R5) >> 5))
				& this->_black_pieces;
			if (temp)
			{
				movers |= (temp >> 4) & white_kings;
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
		const bitboard unoccupied = this->get_unoccupied();

		while (black_movers)
		{
			src = black_movers.lsb();
			black_movers &= ~src;

			dest = (src << 4) & unoccupied; 
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::BLACK_KINGS_ROW)));
			}

			dest = (((src & bitboard::MASK_L3) << 3) |
				((src & bitboard::MASK_L5) << 5)) &
				unoccupied;
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::BLACK_KINGS_ROW)));
			}

			if (src & this->_kings)
			{
				dest = (src >> 4) & unoccupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}

				dest = (((src & bitboard::MASK_R3) >> 3) |
					((src & bitboard::MASK_R5) >> 5)) &
					unoccupied;
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
		const bitboard unoccupied = this->get_unoccupied();

		while (white_movers)
		{
			src = white_movers.lsb();
			white_movers &= ~src;

			dest = (src >> 4) & unoccupied; 
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::WHITE_KINGS_ROW)));
			}

			dest = (((src & bitboard::MASK_R3) >> 3) |
				((src & bitboard::MASK_R5) >> 5)) &
				unoccupied;
			if (dest)
			{
				moves.push_back(move(src, dest,
					bitboard(bitboard::EMPTY), false,
					!(src & this->_kings) &&
					(dest & bitboard::WHITE_KINGS_ROW)));
			}

			if (src & this->_kings)
			{
				dest = (src << 4) & unoccupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						bitboard(bitboard::EMPTY),
						false, false));
				}

				dest = (((src & bitboard::MASK_L3) << 3) |
					((src & bitboard::MASK_L5) << 5)) &
					unoccupied;
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
		const bitboard unoccupied = this->get_unoccupied();

		while (black_jumpers)
		{
			src = black_jumpers.lsb();
			black_jumpers &= ~src;

			capture = (src << 4) & this->_white_pieces;
			if (capture)
			{
				dest = (((capture & bitboard::MASK_L3) << 3) |
					((capture & bitboard::MASK_L5) << 5)) &
					unoccupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						capture, capture & this->_kings,
						!(src & this->_kings) && (dest
						& bitboard::BLACK_KINGS_ROW)));
				}
			}

			capture = (((src & bitboard::MASK_L3) << 3) |
				((src & bitboard::MASK_L5) << 5)) &
				this->_white_pieces;
			if (capture)
			{
				dest = (capture << 4) & unoccupied;
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
				capture = (src >> 4) & this->_white_pieces;
				if (capture)
				{
					dest = (((capture & bitboard::MASK_R3)
							>> 3) |
						((capture & bitboard::MASK_R5)
							>> 5)) & unoccupied;
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
					this->_white_pieces;
				if (capture)
				{
					dest = (capture >> 4) & unoccupied;
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
		const bitboard unoccupied = this->get_unoccupied();

		while (white_jumpers)
		{
			src = white_jumpers.lsb();
			white_jumpers &= ~src;

			capture = (src >> 4) & this->_black_pieces;
			if (capture)
			{
				dest = (((capture & bitboard::MASK_R3) >> 3) |
					((capture & bitboard::MASK_R5) >> 5)) &
					unoccupied;
				if (dest)
				{
					moves.push_back(move(src, dest,
						capture, capture & this->_kings,
						!(src & this->_kings) && (dest
						& bitboard::WHITE_KINGS_ROW)));
				}
			}

			capture = (((src & bitboard::MASK_R3) >> 3) |
				((src & bitboard::MASK_R5) >> 5)) &
				this->_black_pieces;
			if (capture)
			{
				dest = (capture >> 4) & unoccupied;
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
				capture = (src << 4) & this->_black_pieces;
				if (capture)
				{
					dest = (((capture & bitboard::MASK_L3)
							<< 3) |
						((capture & bitboard::MASK_L5)
							<< 5)) & unoccupied;
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
					this->_black_pieces;
				if (capture)
				{
					dest = (capture << 4) & unoccupied;
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

	/** @param str The movetext.
	 *   Movetext contains the actual moves for the game.  Moves begin with
	 *   the source square number, then a "-" or "x", finally destination
	 *   square number.
	 */ 
	move board::parse_move(const std::string& str) const
	{
		if (str.empty())
		{
			/** @throw std::logic_error while @e str has wrong
			 *   format.
			 */
			throw std::logic_error("Error (illegal move,"
				" empty): " + str);
		}

		std::string::size_type p;
		unsigned int i = 0;
		std::vector<bitboard> squares;
		bool is_jump = true;
		std::string::size_type str_size = str.size();
		for (p = 0; p < str_size; ++p)
		{
			switch (str[p])
			{
			case '-':
				if (p > 3)
				{
					std::ostringstream error;
					error << "Error (illegal move,"
						" unexpected character"
						" `-'): " << str;
					throw std::logic_error(error.str());
				}
				is_jump = false;
				// Intentionally no break
			case 'x':
				if (p > 3 && !is_jump)
				{
					std::ostringstream error;
					error << "Error (illegal move,"
						" unexpected character"
						" `x'): " << str;
					throw std::logic_error(error.str());
				}
				if (0 == i || i > 32)
				{
					std::ostringstream error;
					error << "Error (illegal move,"
						" square number `" << i << "'"
						" out of range): " << str;
					throw std::logic_error(error.str());
				}
				squares.push_back(bitboard(0x1U << (i - 1)));
				i = 0;
				break;
			case '0':
				// Intentionally no break
			case '1':
				// Intentionally no break
			case '2':
				// Intentionally no break
			case '3':
				// Intentionally no break
			case '4':
				// Intentionally no break
			case '5':
				// Intentionally no break
			case '6':
				// Intentionally no break
			case '7':
				// Intentionally no break
			case '8':
				// Intentionally no break
			case '9':
				i *= 10;
				i += (str[p] - '0');
				break;
			default:
				std::ostringstream error;
				error << "Error (illegal move,"
					" unexpected character"
					" `" << str[p] << "'): " << str;
				throw std::logic_error(error.str());
				break;
			}
		}
		if (0 == i || i > 32)
		{
			std::ostringstream error;
			error << "Error (illegal move,"
				" square number `" << i << "'"
				" out of range): " << str;
			throw std::logic_error(error.str());
		}
		squares.push_back(bitboard(0x1U << (i - 1)));

		if (squares.size() != 2)
		{
			throw std::logic_error("Error (illegal move): " + str);
		}

		std::vector<move> legal_moves = this->generate_moves();
		for (std::vector<move>::const_iterator pos =
			legal_moves.begin(); pos != legal_moves.end(); ++pos)
		{
			if (pos->get_src() == squares[0] &&
				pos->get_dest() == squares[1] &&
				pos->get_capture() == is_jump)
			{
				return *pos;
			}
		}

		throw std::logic_error("Error (illegal move): " + str);
	}

	void board::set_black_to_move(void)
	{
		if (this->is_white_to_move())
		{
			this->_player = board::BLACK;
			this->_zobrist.change_side();
		}

		assert(!(this->_black_pieces & this->_white_pieces));
		assert(((this->_black_pieces | this->_white_pieces) &
			this->_kings) == this->_kings);
		assert(this->build_zobrist() == this->_zobrist);
	}

	void board::set_white_to_move(void)
	{
		if (this->is_black_to_move())
		{
			this->_player = board::WHITE;
			this->_zobrist.change_side();
		}

		assert(!(this->_black_pieces & this->_white_pieces));
		assert(((this->_black_pieces | this->_white_pieces) &
			this->_kings) == this->_kings);
		assert(this->build_zobrist() == this->_zobrist);
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

	// ================================================================

	std::ostream& operator <<(std::ostream& os, const board& rhs)
	{
		bitboard pieces;
		bitboard a_piece;

		os << (rhs.is_black_to_move() ? 'B' : 'W') << ":W";

		unsigned int i = 0;
		pieces = rhs.get_white_men();
		while (pieces)
		{
			a_piece = pieces.lsb();
			pieces &= ~a_piece;
			if (i++)
			{
				os << ',';
			}
			os << a_piece;
		}

		pieces = rhs.get_white_kings();
		while (pieces)
		{
			a_piece = pieces.lsb();
			pieces &= ~a_piece;
			if (i++)
			{
				os << ',';
			}
			os << 'K' << a_piece;
		}

		os << ":B";

		i = 0;
		pieces = rhs.get_black_men();
		while (pieces)
		{
			a_piece = pieces.lsb();
			pieces &= ~a_piece;
			if (i++)
			{
				os << ',';
			}
			os << a_piece;
		}

		pieces = rhs.get_black_kings();
		while (pieces)
		{
			a_piece = pieces.lsb();
			pieces &= ~a_piece;
			if (i++)
			{
				os << ',';
			}
			os << 'K' << a_piece;
		}

		return os;
	}
}

// End of file
