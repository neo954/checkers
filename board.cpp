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
			case 'k':
				this->_black_pieces |= square;
				this->_kings |= square;
				break;
			case 'm':
				this->_black_pieces |= square;
				break;
			case 'K':
				this->_white_pieces |= square;
				this->_kings |= square;
				break;
			case 'M':
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
					output += 'k';
				}
				else
				{
					output += 'm';
				}
			}
			else if (this->_white_pieces & square)
			{
				if (this->_kings & square)
				{
					output += 'K';
				}
				else
				{
					output += 'M';
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

	void board::black_move(const move& move)
	{
		assert(this->is_valid_black_move(move));

		if (this->_kings & move.get_orig())
		{
			this->_kings &= ~move.get_orig();
			this->_kings |= move.get_dest();
		}
		this->_black_pieces &= ~move.get_orig();
		this->_black_pieces |= move.get_dest();
		this->black_man_crown();
	}

	void board::white_move(const move& move)
	{
		assert(this->is_valid_white_move(move));

		if (this->_kings & move.get_orig())
		{
			this->_kings &= ~move.get_orig();
			this->_kings |= move.get_dest();
		}
		this->_white_pieces &= ~move.get_orig();
		this->_white_pieces |= move.get_dest();
		this->white_man_crown();
	}

	void board::black_jump(const move& move)
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
		this->black_man_crown();
	}

	void board::white_jump(const move& move)
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
		this->white_man_crown();
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
};
// End of file
