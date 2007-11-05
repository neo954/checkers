/* This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007 Mamiyami Information.
                     Gong Jie <neo@mamiyami.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
 */
/** @file engine.hpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  @date $Date: 2007-11-05 17:29:55 $
 *  @version $Revision: 1.17 $
 */

#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include "board.hpp"
#include "io.hpp"

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

		bool make_move(const move& move);

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
		void do_sd(const std::vector<std::string>& args);
		void do_st(const std::vector<std::string>& args);
		void do_setboard(const std::vector<std::string>& args);
		void do_white(const std::vector<std::string>& args);
		void do_undo(const std::vector<std::string>& args);
		void not_implemented(const std::vector<std::string>& args);

		board _board;
		bool _rotate;
		std::vector<move> _history;
		std::vector<move> _best_moves;
		bool _force_mode;
		int _depth_limit;
		int _time_limit;
		io _io;

		static const int UNLIMITED = 99999;

		typedef void (engine::*do_action)(const std::vector<std::string>&);
		std::vector<std::pair<std::string, do_action> > _action;
	};
}

#endif // __ENGINE_HPP__
// End of file
