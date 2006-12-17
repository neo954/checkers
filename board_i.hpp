/// @file board_i.cpp

#ifndef __BOARD_I_HPP__
#define __BOARD_I_HPP__

namespace checkers
{
	inline void board::black_man_crown(void)
	{
		this->_kings |= this->_black_pieces & bitboard::BLACK_KINGS_ROW;
	}

	inline void board::white_man_crown(void)
	{
		this->_kings |= this->_white_pieces & bitboard::WHITE_KINGS_ROW;
	}

	inline bitboard board::get_black_pieces(void) const
	{
		return this->_black_pieces;
	}

	inline bitboard board::get_white_pieces(void) const
	{
		return this->_white_pieces;
	}

	inline bitboard board::get_occupied(void) const
	{
		return this->_black_pieces | this->_white_pieces;
	}

	inline bitboard board::get_not_occupied(void) const
	{
		return ~this->get_occupied();
	}

	inline bitboard board::get_black_men(void) const
	{
		return this->_black_pieces & ~this->_kings;
	}

	inline bitboard board::get_white_men(void) const
	{
		return this->_white_pieces & ~this->_kings;
	}

	inline bitboard board::get_black_kings(void) const
	{
		return this->_black_pieces & this->_kings;
	}

	inline bitboard board::get_white_kings(void) const
	{
		return this->_white_pieces & this->_kings;
	}
};
#endif // __BOARD_I_HPP__
// End of file
