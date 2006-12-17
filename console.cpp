/// @file console.cpp

#include <iostream>
#include <string>
#include "console.hpp"

namespace checkers
{
	console::~console(void)
	{
	}

	void console::draw_board(const board& board)
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
					this->draw_square(board, 0x1 << j);
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
					this->draw_square(board, 0x1 << j);
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

	move console::fetch_move(void)
	{
		std::string input;
		int orig_file;
		int orig_rank;
		int dest_file;
		int dest_rank;

		for (;;)
		{
			std::cout << "input your move >>> " << std::flush;
			std::cin >> input;
			if (input.length() != 5) continue;
			if (input[0] >='a' && input[0] <='h')
			{
				input[0] -= 'a' - 'A';
			}
			if (input[0] < 'A' || input[0] > 'H')
			{
				continue;
			}
			if (input[1] < '1' || input[1] > '8')
			{
				continue;
			}
			if (input[2] != '-') continue;
			if (input[3] >='a' && input[3] <='h')
			{
				input[3] -= 'a' - 'A';
			}
			if (input[3] < 'A' || input[3] > 'H')
			{
				continue;
			}
			if (input[4] < '1' || input[4] > '8')
			{
				continue;
			}
			orig_file = input[0] - 'A';
			orig_rank = input[1] - '1';
			dest_file = input[3] - 'A';
			dest_rank = input[4] - '1';
			if ((orig_file % 2 != orig_rank % 2) ||
				(dest_file % 2 != dest_rank % 2))
			{
				continue;
			}
			break;
		}
		
		return move(bitboard(input[0] - 'A', input[1] - '1'), bitboard(input[3] - 'A', input[4] - '1'));
	}

	void console::draw_square(const board& board, bitboard square)
	{
		if (board.get_black_men() & square)
		{
			std::cout << "(b)";
		}		
		else if (board.get_white_men() & square)
		{
			std::cout << "(w)";
		}		
		else if (board.get_black_kings() & square)
		{
			std::cout << "(B)";
		}		
		else if (board.get_white_kings() & square)
		{
			std::cout << "(W)";
		}
		else
		{
			std::cout << " \\ ";
		}
	}
};
// End of file
