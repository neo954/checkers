/// @file main.cpp

#include <iostream>
#include "board.hpp"
#include "console.hpp"

int main(void)
{
	checkers::board board;
	checkers::console interface;

	board.opening();
	board.draw(interface);

	for (;;)
	{
		if (board.get_black_jumpers())
		{
			do
			{
				std::cout << "black jump - " << std::flush;
				checkers::move move = interface.fetch_move();
	 			if (board.is_valid_black_jump(move))
				{
					board.black_jump(move);
					board.draw(interface);
				}
				else
				{
					std::cout << "Invalid black jump!" << std::endl;
				}
			} while (board.get_black_jumpers());
		} 
		else if (board.get_black_movers())
		{
			for (;;)
			{
				std::cout << "black move - " << std::flush;
				checkers::move move = interface.fetch_move();
				if (board.is_valid_black_move(move))
				{
					board.black_move(move);
					board.draw(interface);
					break;
				}
				else
				{
					std::cout << "Invalid black move!" << std::endl;
				}
			}
		}
		else
		{
			std::cout << "White win!" << std::endl;
			break;
		}

		if (board.get_white_jumpers())
		{
			do
			{
				std::cout << "white jump - " << std::flush;
				checkers::move move = interface.fetch_move();
	 			if (board.is_valid_white_jump(move))
				{
					board.white_jump(move);
					board.draw(interface);
				}
				else
				{
					std::cout << "Invalid white jump!" << std::endl;
				}
			} while (board.get_white_jumpers());
		} 
		else if (board.get_white_movers())
		{
			for (;;)
			{
				std::cout << "white move - " << std::flush;
				checkers::move move = interface.fetch_move();
				if (board.is_valid_white_move(move))
				{	
					board.white_move(move);
					board.draw(interface);
					break;
				}
				else
				{
					std::cout << "Invalid white move!" << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Black win!" << std::endl;
			break;
		}
	}
	return 0;
}
// End of file
