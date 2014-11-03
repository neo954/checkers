/* $Id: engine.cpp,v 1.47 2009-08-09 13:32:13 neo Exp $

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007, 2008, 2009 Mamiyami Information.
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
/** @file engine.cpp
 *  @brief Game engine.
 */

extern "C"
{
	#include <unistd.h>
}
#include <cstdlib>
#include "absearch.hpp"
#include "engine.hpp"
#include "nonstdio.hpp"

namespace checkers
{
	engine::engine(void) :
		_board(), _rotate(false), _history(), _best_moves(),
		_force_mode(false), _depth_limit(UNLIMITED), _time_limit(10),
		_verbose(false)
	{
		this->_action.insert(std::make_pair("?",
			&engine::do_help));
		this->_action.insert(std::make_pair("analyze",
			&engine::do_analyze));
		this->_action.insert(std::make_pair("black",
			&engine::do_black));
		this->_action.insert(std::make_pair("force",
			&engine::do_force));
		this->_action.insert(std::make_pair("go",
			&engine::do_go));
		this->_action.insert(std::make_pair("help",
			&engine::do_help));
		this->_action.insert(std::make_pair("history",
			&engine::do_history));
		this->_action.insert(std::make_pair("new",
			&engine::do_new));
		this->_action.insert(std::make_pair("ping",
			&engine::do_ping));
		this->_action.insert(std::make_pair("ponder",
			&engine::not_implemented));
		this->_action.insert(std::make_pair("print",
			&engine::do_print));
		this->_action.insert(std::make_pair("quit",
			&engine::do_quit));
		this->_action.insert(std::make_pair("rotate",
			&engine::do_rotate));
		this->_action.insert(std::make_pair("sd",
			&engine::do_sd));
		this->_action.insert(std::make_pair("st",
			&engine::do_st));
		this->_action.insert(std::make_pair("setboard",
			&engine::do_setboard));
		this->_action.insert(std::make_pair("undo",
			&engine::do_undo));
		this->_action.insert(std::make_pair("verbose",
			&engine::do_verbose));
		this->_action.insert(std::make_pair("white",
			&engine::do_white));
	}

	engine& engine::init(void)
	{
		static engine engine;

		return engine;
	}

	void engine::run(void)
	{
		std::string command;
		std::vector<std::string> args;
		std::map<std::string, do_action>::const_iterator pos;

		this->print_board();

		for (;;)
		{
			this->prompt();
			this->ponder();

			if (nio.eof())
			{
				break;
			}

			nio >> command;

			args = engine::parse(command);
			if (!args.empty())
			{
				pos = this->_action.find(args[0]);
				if (pos != this->_action.end())
				{
					(this->*pos->second)(args);
				}
				else if (!this->human_makes_move(args[0]))
				{
					this->computer_makes_move();
				}
			}
		}
	}

	// ================================================================

	void engine::print_board(void)
	{
		nio << ";[FEN \"" << this->_board << "\"]\n";

		int i;
		int j;

		if (this->_rotate)
		{
			nio << "  +---+---+---+---+---+---+---+---+\n";
			for (i = 0; i < 32; i += 8)
			{
				nio << "  ";
				for (j = i; j < i + 4; ++j)
				{
					nio << "|   |";
					this->print_square(j);
				}
				nio << "|\n  +";
				for (j = i; j < i + 4; ++j)
				{
					nio << "---+" << (j + 1);
					if (j + 1 < 10)
					{
						nio << '-';
					}
					nio << "-+";
				}
				nio << "\n  |";
				for (j = i + 4; j < i + 8; ++j)
				{
					this->print_square(j);
					nio << "|   |";
				}
				nio << "\n  +";
				for (j = i + 4; j < i + 8; ++j)
				{
					nio << (j + 1);
					if (j + 1 < 10)
					{
						nio << '-';
					}
					nio << "-+---+";
				}
				nio << '\n';
			}
		}
		else
		{
			nio << "  +---+---+---+---+---+---+---+---+\n";
			for (i = 32; i > 0; i -= 8)
			{
				nio << "  ";
				for (j = i; j > i - 4; --j)
				{
					nio << "|   |";
					this->print_square(j - 1);
				}
				nio << "|\n  +";
				for (j = i; j > i - 4; --j)
				{
					nio << "---+" << j;
					if (j < 10)
					{
						nio << '-';
					}
					nio << "-+";
				}
				nio << "\n  |";
				for (j = i - 4; j > i - 8; --j)
				{
					this->print_square(j - 1);
					nio << "|   |";
				}
				nio << "\n  +";
				for (j = i - 4; j > i - 8; --j)
				{
					nio << j;
					if (j < 10)
					{
						nio << '-';
					}
					nio << "-+---+";
				}
				nio << '\n';
			}
		}
	}

	void engine::print_square(int n)
	{
		assert(0 <= n && n < 32);

		bitboard square(0x1U << n);

		if (this->_board.get_black_men() & square)
		{
			nio << "(b)";
		}		
		else if (this->_board.get_white_men() & square)
		{
			nio << "(w)";
		}		
		else if (this->_board.get_black_kings() & square)
		{
			nio << "(B)";
		}		
		else if (this->_board.get_white_kings() & square)
		{
			nio << "(W)";
		}
		else
		{
			nio << " \\ ";
		}
	}

	std::string engine::to_string(int v)
	{
		if (UNLIMITED == v)
		{
			return std::string("unlimited");
		}
		std::ostringstream stream;
		stream << v;
		return stream.str();
	}

	int engine::to_int(const std::string& str)
	{
		if ("unlimited" == str)
		{
			return UNLIMITED;
		}
		int v = std::strtol(str.c_str(), NULL, 10);
		if (v < 0 || v > UNLIMITED)
		{
			return UNLIMITED;
		}
		return v;
	}

	std::vector<std::string> engine::parse(const std::string& command)
	{
		std::vector<std::string> args;
		std::string::size_type idx_begin = 0;
		std::string::size_type idx_end   = 0;
		std::string piece;
		std::string::const_iterator begin = command.begin();

		while ((idx_end = command.find(' ', idx_begin))
			!= std::string::npos)
		{
			piece = std::string(begin + idx_begin,
				begin + idx_end);
			idx_begin = idx_end + 1;
			if (!piece.empty())
			{
				args.push_back(piece);
			}
		}

		if ((idx_end = command.find('\n', idx_begin))
			!= std::string::npos)
		{
			piece = std::string(begin + idx_begin,
				begin + idx_end);
		}
		else
		{
			piece = std::string(begin + idx_begin, command.end());
		}

		if (!piece.empty())
		{
			args.push_back(piece);
		}

		return args;
	}

	bool engine::make_move(const move& move)
	{
		this->_history.push_back(move);

		bool ret = this->_board.make_move(move);

		if (this->_best_moves.size())
		{
			if (move == this->_best_moves.front())
			{
				this->_best_moves.erase(
					this->_best_moves.begin());
			}
			else
			{
				this->_best_moves.clear();
			}
		}

		return ret;
	}

	void engine::computer_makes_move(void)
	{
		if (this->_force_mode)
		{
			return;
		}

		bool contin;

		nio << "  Thinking ...\n";

		std::vector<move> moves;
		do
		{
			absearch::think(this->_best_moves, this->_board,
				this->_depth_limit, this->_time_limit,
				this->_verbose);
			if (this->_best_moves.empty())
			{
				break;
			}
			do
			{
				moves.push_back(this->_best_moves.front());
				contin = this->make_move(
					this->_best_moves.front());
				this->print_board();
			} while (contin && !this->_best_moves.empty());
		} while (contin);

		for (std::vector<move>::const_iterator pos = moves.begin();
			pos != moves.end(); ++pos)
		{
			nio << *pos << '\n';
		}

		this->result();
	}

	/// @return whether human player move one more
	bool engine::human_makes_move(const std::string& str)
	{
		bool contin = true;

		try
		{
			move move = this->_board.parse_move(str);
			assert(this->_board.is_valid_move(move));
			contin = this->make_move(move);

			this->print_board();
			this->result();
		}
		catch (const std::logic_error& e)
		{
			nio << e.what() << '\n';
		}

		return contin;
	}

	void engine::idle(void)
	{
		for (;;)
		{
			nio << io::flush;
			if (nio.lines_to_read() || nio.eof())
			{
				break;
			}
			usleep(500);
		}
	}

	void engine::ponder(void)
	{
		if (this->_force_mode || !absearch::think(
			this->_best_moves, this->_board, engine::UNLIMITED,
			engine::UNLIMITED, this->_verbose))
		{
			this->idle();
		}
	}

	void engine::prompt(void)
	{
		nio << "  *** "
			<< (this->_board.is_black_to_move() ? "Black" : "White")
			<< " ***\n";
	}

	bool engine::result(void)
	{
		if (this->_board.is_losing())
		{
			nio <<
				(this->_board.is_black_to_move() ?
				"[Result \"1-0\"]\n{White win}\n" :
				"[Result \"0-1\"]\n{Black win}\n");
			return true;
		}
		if (this->_board.is_winning())
		{
			nio <<
				(this->_board.is_black_to_move() ?
				"[Result \"0-1\"]\n{Black win}\n" :
				"[Result \"1-0\"]\n{White win}\n");
			return true;
		}

		return false;
	}

	void engine::do_analyze(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		nio << "  Analyzing ...\n";
		absearch::think(this->_best_moves, this->_board,
			this->_depth_limit, this->_time_limit, true);
	}

	void engine::do_print(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->print_board();
	}

	void engine::do_rotate(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_rotate = !this->_rotate;
		this->print_board();
	}

	void engine::do_black(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_board.set_black_to_move();
		this->_best_moves.clear();
	}

	void engine::do_white(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_board.set_white_to_move();
		this->_best_moves.clear();
	}

	void engine::do_ping(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		nio << "pong";
		if (1 < args.size())
		{
			nio << ' ' << args[1];
		}
		nio << '\n';
	}

	void engine::do_go(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_force_mode = false;
		this->computer_makes_move();
	}

	void engine::do_help(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		nio <<
		// --+----1----+----2----+----3----+----4----+----5----+----6--|
		"    ?               Show this help information.\n"
		"    analyze         Engine thinks about what move it make next"
			" if it were on\n"
		"                    move.\n"
		"    black           Set Black on move, and the engine will"
			" play White.\n"
		"    force           Set the engine to play neither color"
			" (\"force mode\").\n"
		"    go              Leave force mode and set the engine to"
			" play the color that\n"
		"                    is on move.  Start thinking and eventually"
			" make a move.\n"
		"    help            Show this help information.\n"
		"    history         Show the record of moves.\n"
		"    new             Reset the board to the standard starting"
			" position.\n"
		"    ping N          N is a decimal number.  Reply by sending"
			" the string\n"
		"                    \"pong N\"\n"
		"    print           Show the current board.\n"
		"    quit            Quit this program.\n"
		"    rotate          Rotate the board 180 degrees.\n"
		"    setboard FEN    Set up the pieces position on the"
			" board.\n"
		"    sd DEPTH        The engine should limit its thinking to"
			" DEPTH ply.\n"
		"    st TIME         Set the time control to TIME seconds per"
			" move.\n"
		"    undo            Back up a move.\n"
		"    verbose         Toggle verbose mode.\n"
		"    white           Set White on move, and the engine will"
			" play Black.\n";
		// --+----1----+----2----+----3----+----4----+----5----+----6--|
	}

	void engine::do_history(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		for (std::vector<move>::const_iterator pos =
			this->_history.begin(); pos != this->_history.end();
			++pos)
		{
			nio << "  move " << *pos << '\n';
		}
	}

	void engine::do_new(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_board = board();
		this->_history.clear();
		this->_best_moves.clear();
		this->print_board();
	}

	void engine::do_quit(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		exit(0);
	}

	void engine::do_force(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_force_mode = true;
	}

	void engine::do_sd(const std::vector<std::string>& args)
	{
		if (args.size() <= 1)
		{
			nio << "Error (option missing): sd\n";
			return;
		}
		this->_depth_limit = this->to_int(args[1]);
	}

	void engine::do_st(const std::vector<std::string>& args)
	{
		if (args.size() <= 1)
		{
			nio << "Error (option missing): st\n";
			return;
		}
		this->_time_limit  = this->to_int(args[1]);
	}

	void engine::do_setboard(const std::vector<std::string>& args)
	{
		if (args.size() <= 1)
		{
			nio << "Error (option missing): setboard\n";
			return;
		}

		try
		{
			this->_board = board(args[1]);
		}
		catch (const std::logic_error& e)
		{
			nio << e.what() << '\n';
			return;
		}

		this->_history.clear();
		this->_best_moves.clear();
		this->print_board();
		nio << io::flush;
	}

	void engine::do_undo(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		if (this->_history.size() > 0)
		{
			this->_board.undo_move(this->_history.back());
			this->_history.pop_back();
			this->_best_moves.clear();
			this->print_board();
		}
		else
		{
			nio << "Error (no move to undo): undo\n";
		}
	}

	void engine::do_verbose(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		if (this->_verbose)
		{
			this->_verbose = false;
			nio << "  Verbose mode off.\n";
		}
		else
		{
			this->_verbose = true;
			nio << "  Verbose mode on.\n";
		}
	}

	void engine::not_implemented(const std::vector<std::string>& args)
	{
		nio << "Error (command not implemented): " << args[0]
			<< '\n';
	}
}

// End of file
