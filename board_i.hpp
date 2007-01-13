/// @file board_i.hpp

#ifndef __BOARD_I_HPP__
#define __BOARD_I_HPP__

namespace checkers
{
	inline board::board(void) :
		_black_pieces(bitboard::EMPTY), _white_pieces(bitboard::EMPTY),
		_kings(bitboard::EMPTY), _player(BLACK)
	{
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

	inline bool board::is_black_move(void) const
	{
		return BLACK == this->_player;
	}

	inline bool board::is_white_move(void) const
	{
		return WHITE == this->_player;
	}

	// ================================================================

	/// @return crowned black men
	inline bitboard board::black_man_crown(void)
	{
		bitboard crown = this->get_black_men() &
			bitboard::BLACK_KINGS_ROW;
		this->_kings |= crown;

		return crown;
	}

	/// @return crowned white men
	inline bitboard board::white_man_crown(void)
	{
		bitboard crown = this->get_white_men() &
			bitboard::WHITE_KINGS_ROW;
		this->_kings |= crown;

		return crown;
	}
}

#endif // __BOARD_I_HPP__
// End of file
