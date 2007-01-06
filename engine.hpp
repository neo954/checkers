/// @file engine.hpp

#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <string>
#include "board.hpp"
#include "loopbuffer.hpp"

namespace checkers
{
	class engine
	{
	public:
		static engine& init(void);
		void run(void);
		move fetch_move(const std::string& input);

	private:
		engine(void);
		engine(const engine& rhs);
		engine& operator=(const engine& rhs);
		void draw(void) const;
		void draw(bitboard square) const;
		void rotate(void);

		board _board;
		bool _rotate;

		loopbuffer _read_buf;
		loopbuffer _write_buf;
	};
};

#endif // __ENGINE_HPP__
// End of file
