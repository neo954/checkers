/** @file engine.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 *  @date $Date: 2007-01-19 14:40:56 $
 *  @version $Revision: 1.12 $
 */

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
		/// Define but not implement, to prevent object copy.
		engine(const engine& rhs);
		/// Define but not implement, to prevent object copy.
		engine& operator=(const engine& rhs) const;

		void print(void);
		std::string to_string(const bitboard& square);
		std::string to_string(int v);
		int to_int(const std::string& str);

		void rotate(void);

		void go(void);

		void prompt(void);
		bool result(void);

		void do_analyze(const std::vector<std::string>& args);
		void do_black(const std::vector<std::string>& args);
		void do_force(const std::vector<std::string>& args);
		void do_go(const std::vector<std::string>& args);
		void do_help(const std::vector<std::string>& args);
		void do_new(const std::vector<std::string>& args);
		void do_ping(const std::vector<std::string>& args);
		void do_print(const std::vector<std::string>& args);
		void do_quit(const std::vector<std::string>& args);
		void do_rotate(const std::vector<std::string>& args);
		void do_set(const std::vector<std::string>& args);
		void do_white(const std::vector<std::string>& args);

		board _board;
		bool _rotate;
		bool _force_mode;
		int _depth_limit;
		int _time_limit;

		static const int UNLIMITED = 99999;

		typedef void (engine::*do_action)(const std::vector<std::string>&);
		std::vector<std::pair<std::string, do_action> > _action;
	};
}

#endif // __ENGINE_HPP__
// End of file
