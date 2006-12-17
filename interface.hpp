/// @file interface.hpp

#ifndef __INTERFACE_HPP__
#define __INTERFACE_HPP__

#include "board.hpp"
#include "move.hpp"

namespace checkers
{
	class board;

	class interface
	{
	protected:
		interface(void);
		virtual ~interface(void);

	public:
		void rotate(void);
		virtual void draw_board(const board& board) = 0;
		virtual move fetch_move(void) = 0;

	protected:
		bool _rotate;
	};
};

#endif // __INTERFACE_HPP__
// End of file
