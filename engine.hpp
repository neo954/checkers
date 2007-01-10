/// @file engine.hpp

#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <string>
#include <vector>
#include "board.hpp"
#include "loopbuffer.hpp"
#include "player.hpp"

namespace checkers
{
	class engine
	{
	public:
		static engine& init(void);
		void run(void);

	private:
		engine(void);
		engine(const engine& rhs);
		engine& operator=(const engine& rhs);

		bool make_move(const move& move);
		void switch_player(void);
		void print(void);
		void print(bitboard square);
		void rotate(void);

		void parse_input(void);

		void prompt(void);

		void do_black(const std::vector<std::string>& args);
		void do_go(const std::vector<std::string>& args);
		void do_help(const std::vector<std::string>& args);
		void do_new(const std::vector<std::string>& args);
		void do_ping(const std::vector<std::string>& args);
		void do_print(const std::vector<std::string>& args);
		void do_quit(const std::vector<std::string>& args);
		void do_rotate(const std::vector<std::string>& args);
		void do_white(const std::vector<std::string>& args);

		board _board;
		bool _rotate;
		player _player;

		typedef void (engine::*do_action)(const std::vector<std::string>&);
		std::vector<std::pair<std::string, do_action> > _action;
	};
};
#endif // __ENGINE_HPP__
// End of file
