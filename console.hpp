/// @file console.hpp

#ifndef __CONSOLE_HPP__
#define __CONSOLE_HPP__

#include "interface.hpp"

namespace checkers
{
	class console : public interface
	{
	public:
		~console(void);

		void draw_board(const board& board);
		move fetch_move(void);

	private:
		void draw_square(const board& board, bitboard square);
	};
};

#endif // __CONSOLE_HPP__
// End of file
