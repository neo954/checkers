/// @file console.cpp

#include <iostream>
#include <string>
#include "engine.hpp"

namespace checkers
{
	engine::engine(void) :
		_board(), _rotate(false), _read_buf(), _write_buf()
	{
	}

	engine& engine::init(void)
	{
		static engine engine;
		return engine;
	}

	void engine::run(void)
	{
		for (;;)
		{
			if (this->_board.get_black_jumpers())
			{
				do
				{
					std::cout << "*** black jump ***" << std::endl;
					checkers::move move("");
	 				if (this->_board.is_valid_black_jump(move))
					{
						this->_board.black_jump(move);
						this->draw();
					}
					else
					{
						std::cout << "Illegal black jump!" << std::endl;
					}
				} while (this->_board.get_black_jumpers());
			} 
			else if (this->_board.get_black_movers())
			{
				for (;;)
				{
					std::cout << "*** black move ***" << std::endl;
					checkers::move move("");
					if (this->_board.is_valid_black_move(move))
					{
						this->_board.black_move(move);
						this->draw();
						break;
					}
					else
					{
						std::cout << "Illegal black move!" << std::endl;
					}
				}
			}
			else
			{
				std::cout << "White win!" << std::endl;
				break;
			}

			if (this->_board.get_white_jumpers())
			{
				do
				{
					std::cout << "*** white jump ***" << std::endl;
					checkers::move move("");
		 			if (this->_board.is_valid_white_jump(move))
					{
						this->_board.white_jump(move);
						this->draw();
					}
					else
					{
						std::cout << "Illegal white jump!" << std::endl;
					}
				} while (this->_board.get_white_jumpers());
			} 
			else if (this->_board.get_white_movers())
			{
				for (;;)
				{
					std::cout << "*** white move ***" << std::endl;
					checkers::move move("");
					if (this->_board.is_valid_white_move(move))
					{	
						this->_board.white_move(move);
						this->draw();
						break;
					}
					else
					{
						std::cout << "Illegal white move!" << std::endl;
					}
				}
			}
			else
			{
				std::cout << "Black win!" << std::endl;
				break;
			}
		}
	}

	void engine::draw(void) const
	{
		int i;
		int j;

		if (this->_rotate)
		{
			std::cout << "     H   G   F   E   D   C   B   A" << std::endl;
			std::cout << "   +---+---+---+---+---+---+---+---+" << std::endl;
			for (i = 1; i <= 8; ++i)
			{
				std::cout << i;
				if (i % 2)
				{
					std::cout << "  | ";
				}
				for (j = i * 4 - 1; j >= i * 4 - 4; --j)
				{
					std::cout << "  |";
					this->draw(0x1 << j);
					std::cout << "| ";
				}
				if (!(i % 2))
				{
					std::cout << "  | ";
				}
				std::cout << " " << i << std::endl;
				std::cout << "   +---+---+---+---+---+---+---+---+" << std::endl;
			}
			std::cout << "     H   G   F   E   D   C   B   A" << std::endl;
		}
		else
		{
			std::cout << "     A   B   C   D   E   F   G   H" << std::endl;
			std::cout << "   +---+---+---+---+---+---+---+---+" << std::endl;
			for (i = 8; i >= 1; --i)
			{
				std::cout << i;
				if (!(i % 2))
				{
					std::cout << "  | ";
				}
				for (j = i * 4 - 4; j <= i * 4 - 1; ++j)
				{
					std::cout << "  |";
					this->draw(0x1 << j);
					std::cout << "| ";
				}
				if (i % 2)
				{
					std::cout << "  | ";
				}
				std::cout << " " << i << std::endl;
				std::cout << "   +---+---+---+---+---+---+---+---+" << std::endl;
			}
			std::cout << "     A   B   C   D   E   F   G   H" << std::endl;
		}
	}

	void engine::draw(bitboard square) const
	{
		assert(1 == square.bit_count());

		if (this->_board.get_black_men() & square)
		{
			std::cout << "(b)";
		}		
		else if (this->_board.get_white_men() & square)
		{
			std::cout << "(w)";
		}		
		else if (this->_board.get_black_kings() & square)
		{
			std::cout << "(B)";
		}		
		else if (this->_board.get_white_kings() & square)
		{
			std::cout << "(W)";
		}
		else
		{
			std::cout << " \\ ";
		}
	}

	void engine::rotate(void)
	{
		this->_rotate = !this->_rotate;
	}
};
// End of file
