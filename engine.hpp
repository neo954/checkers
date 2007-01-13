/// @file engine.hpp

#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "board.hpp"

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

		void print(void);
		void print(bitboard square);
		void rotate(void);

		void go(void);

		void prompt(void);
		void declare_winning(void);

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

		typedef void (engine::*do_action)(const std::vector<std::string>&);
		std::vector<std::pair<std::string, do_action> > _action;
	};
}

#endif // __ENGINE_HPP__
// End of file
