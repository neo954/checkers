/// @file board.cpp

#include <cassert>
#include "board.hpp"

namespace checkers
{
	board::board(const std::string& input)
	{
		int i;
		bitboard square;
		std::string::size_type p;

		for (i = 0, p = 0; i < 32 && p < input.size(); ++i, ++p)
		{
			square = 0x1 << i;
			switch (input[p])
			{
			case 'B':
				this->_kings |= square;
				// intentionally no break
			case 'b':
				this->_black_pieces |= square;
				break;
			case 'W':
				this->_kings |= square;
				// intentionally no break
			case 'w':
				this->_white_pieces |= square;
				break;
			default:
				--i;
				break;
			}
		}	
	}

	std::string board::to_string(void) const
	{
		std::string output;
		int i;
		bitboard square;

		output.reserve(39);
		for (i = 0; i < 32; ++i)
		{
			if (i > 0 && 0 == i % 4)
			{
				output += '/';
			}
			square = 0x1 << i;
			if (this->_black_pieces & square)
			{
				if (this->_kings & square)
				{
					output += 'B';
				}
				else
				{
					output += 'b';
				}
			}
			else if (this->_white_pieces & square)
			{
				if (this->_kings & square)
				{
					output += 'W';
				}
				else
				{
					output += 'w';
				}
			}
			else
			{
				output += '0';
			}
		}
		return output;
	}

	board& board::opening(void)
	{
		this->_black_pieces = bitboard::BLACK_PIECES_INIT;
		this->_white_pieces = bitboard::WHITE_PIECES_INIT;
		return *this;
	}

	bool board::is_valid_black_move(const move& move) const
	{
		return (this->_black_pieces & move.get_orig()) &&
			(this->get_not_occupied() & move.get_dest()) &&
			((this->_kings & move.get_orig()) ?
				move.is_valid_on_king() :
			 	move.is_valid_on_black_man());
	}

	bool board::is_valid_black_jump(const move& move) const
	{
		return (this->_black_pieces & move.get_orig()) &&
			(this->get_not_occupied() & move.get_dest()) &&
			(this->_white_pieces &
			((this->_kings & move.get_orig()) ?
			move.get_king_jump_capture() :
			move.get_black_man_jump_capture()));
	}

	bool board::is_valid_white_move(const move& move) const
	{
		return (this->_white_pieces & move.get_orig()) &&
			(this->get_not_occupied() & move.get_dest()) &&
			((this->_kings & move.get_orig()) ?
				move.is_valid_on_king() :
			 	move.is_valid_on_white_man());
	}

	bool board::is_valid_white_jump(const move& move) const
	{
		return (this->_white_pieces & move.get_orig()) &&
			(this->get_not_occupied() & move.get_dest()) &&
			(this->_black_pieces &
			((this->_kings & move.get_orig()) ?
			move.get_king_jump_capture() :
			move.get_white_man_jump_capture()));
	}

	bitboard board::black_move(const move& move)
	{
		assert(this->is_valid_black_move(move));

		if (this->_kings & move.get_orig())
		{
			this->_kings &= ~move.get_orig();
			this->_kings |= move.get_dest();
		}
		this->_black_pieces &= ~move.get_orig();
		this->_black_pieces |= move.get_dest();
		return this->black_man_crown();
	}

	bitboard board::white_move(const move& move)
	{
		assert(this->is_valid_white_move(move));

		if (this->_kings & move.get_orig())
		{
			this->_kings &= ~move.get_orig();
			this->_kings |= move.get_dest();
		}
		this->_white_pieces &= ~move.get_orig();
		this->_white_pieces |= move.get_dest();
		return this->white_man_crown();
	}

	bitboard board::black_jump(const move& move)
	{
		assert(this->is_valid_black_jump(move));

		bool is_king = this->_kings & move.get_orig();

		if (is_king)
		{
			this->_kings &= ~move.get_orig();
			this->_kings |= move.get_dest();
		}
		this->_black_pieces &= ~move.get_orig();
		this->_black_pieces |= move.get_dest();

		bitboard capture = is_king ?
			move.get_king_jump_capture() :
			move.get_black_man_jump_capture();
		this->_kings &= ~capture;
		this->_white_pieces &= ~capture;
		return this->black_man_crown();
	}

	bitboard board::white_jump(const move& move)
	{
		assert(this->is_valid_white_jump(move));

		bool is_king = this->_kings & move.get_orig();

		if (is_king)
		{
			this->_kings &= ~move.get_orig();
			this->_kings |= move.get_dest();
		}
		this->_white_pieces &= ~move.get_orig();
		this->_white_pieces |= move.get_dest();

		bitboard capture = is_king ?
			move.get_king_jump_capture() :
			move.get_white_man_jump_capture();
		this->_kings &= ~capture;
		this->_black_pieces &= ~capture;
		return this->white_man_crown();
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
		bitboard movers = 0x0;
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
		bitboard movers = 0x0;
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
		bitboard black_movers = this->get_black_movers();
		bitboard orig;
		bitboard dest;
		const bitboard not_occupied = this->get_not_occupied();

		while (black_movers)
		{
			orig = black_movers.get_lsb();
			black_movers &= ~orig;

			dest = (orig >> 4) & not_occupied; 
			if (dest)
			{
				moves.push_back(move(orig, dest));
			}

			dest = (((orig & bitboard::MASK_R3) >> 3) |
				((orig & bitboard::MASK_R5) >> 5)) &
				not_occupied;
			if (dest)
			{
				moves.push_back(move(orig, dest));
			}

			if (orig & this->_kings)
			{
				dest = (orig << 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}

				dest = (((orig & bitboard::MASK_L3) << 3) |
					((orig & bitboard::MASK_L5) << 5)) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}
			}
		}

		return moves;
	}

	std::vector<move> board::generate_white_moves(void) const
	{
		std::vector<move> moves;
		bitboard white_movers = this->get_white_movers();
		bitboard orig;
		bitboard dest;
		const bitboard not_occupied = this->get_not_occupied();

		while (white_movers)
		{
			orig = white_movers.get_lsb();
			white_movers &= ~orig;

			dest = (orig << 4) & not_occupied; 
			if (dest)
			{
				moves.push_back(move(orig, dest));
			}

			dest = (((orig & bitboard::MASK_L3) << 3) |
				((orig & bitboard::MASK_L5) << 5)) &
				not_occupied;
			if (dest)
			{
				moves.push_back(move(orig, dest));
			}

			if (orig & this->_kings)
			{
				dest = (orig >> 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}

				dest = (((orig & bitboard::MASK_R3) >> 3) |
					((orig & bitboard::MASK_R5) >> 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}
			}
		}

		return moves;
	}

	std::vector<move> board::generate_black_jumps(void) const
	{
		std::vector<move> moves;
		bitboard black_jumpers = this->get_black_jumpers();
		bitboard orig;
		bitboard dest;
		bitboard temp;
		const bitboard not_occupied = this->get_not_occupied();

		while (black_jumpers)
		{
			orig = black_jumpers.get_lsb();
			black_jumpers &= ~orig;

			temp = (orig >> 4) & this->_white_pieces;
			if (temp)
			{
				dest = (((temp & bitboard::MASK_R3) >> 3) |
					((temp & bitboard::MASK_R5) >> 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}
			}

			temp = (((orig & bitboard::MASK_R3) >> 3) |
				((orig & bitboard::MASK_R5) >> 5)) &
				this->_white_pieces;
			if (temp)
			{
				dest = (temp >> 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}
			}

			if (orig & this->_kings)
			{
				temp = (orig << 4) & this->_white_pieces;
				if (temp)
				{
					dest = (((temp & bitboard::MASK_L3)
							<< 3) |
						((temp & bitboard::MASK_L5)
							<< 5)) & not_occupied;
					if (dest)
					{
						moves.push_back(
							move(orig, dest));
					}
				}

				temp = (((orig & bitboard::MASK_L3) << 3) |
					((orig & bitboard::MASK_L5) << 5)) &
					this->_white_pieces;
				if (temp)
				{
					dest = (temp << 4) & not_occupied;
					if (dest)
					{
						moves.push_back(
							move(orig, dest));
					}
				}
			}
		}

		return moves;
	}

	std::vector<move> board::generate_white_jumps(void) const
	{
		std::vector<move> moves;
		bitboard white_jumpers = this->get_white_jumpers();
		bitboard orig;
		bitboard dest;
		bitboard temp;
		const bitboard not_occupied = this->get_not_occupied();

		while (white_jumpers)
		{
			orig = white_jumpers.get_lsb();
			white_jumpers &= ~orig;

			temp = (orig << 4) & this->_black_pieces;
			if (temp)
			{
				dest = (((temp & bitboard::MASK_L3) << 3) |
					((temp & bitboard::MASK_L5) << 5)) &
					not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}
			}

			temp = (((orig & bitboard::MASK_L3) << 3) |
				((orig & bitboard::MASK_L5) << 5)) &
				this->_black_pieces;
			if (temp)
			{
				dest = (temp << 4) & not_occupied;
				if (dest)
				{
					moves.push_back(move(orig, dest));
				}
			}

			if (orig & this->_kings)
			{
				temp = (orig >> 4) & this->_black_pieces;
				if (temp)
				{
					dest = (((temp & bitboard::MASK_R3)
							>> 3) |
						((temp & bitboard::MASK_R5)
							>> 5)) & not_occupied;
					if (dest)
					{
						moves.push_back(
							move(orig, dest));
					}
				}

				temp = (((orig & bitboard::MASK_R3) >> 3) |
					((orig & bitboard::MASK_R5) >> 5)) &
					this->_black_pieces;
				if (temp)
				{
					dest = (temp >> 4) & not_occupied;
					if (dest)
					{
						moves.push_back(
							move(orig, dest));
					}
				}
			}
		}

		return moves;
	}
}

// End of file
