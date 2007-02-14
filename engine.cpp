/* This file is a part of textual checkers, a English/American checkers
   game.

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
/** @file engine.cpp
 *  @brief
 *  @author Gong Jie <neo@mamiyami.com>
 *  $Date: 2007-02-14 14:52:34 $
 *  $Revision: 1.20 $
 */

#include "engine.hpp"
#include "intelligence.hpp"

namespace checkers
{
	engine::engine(void) :
		_board(), _rotate(false), _history(), _force_mode(false),
		_depth_limit(UNLIMITED), _time_limit(10),
		_io(STDIN_FILENO, STDOUT_FILENO)
	{
		this->_action.push_back(std::make_pair("analyze",
			&engine::do_analyze));
		this->_action.push_back(std::make_pair("black",
			&engine::do_black));
		this->_action.push_back(std::make_pair("force",
			&engine::do_force));
		this->_action.push_back(std::make_pair("go",
			&engine::do_go));
		this->_action.push_back(std::make_pair("help",
			&engine::do_help));
		this->_action.push_back(std::make_pair("new",
			&engine::do_new));
		this->_action.push_back(std::make_pair("ping",
			&engine::do_ping));
		this->_action.push_back(std::make_pair("ponder",
			&engine::not_implemented));
		this->_action.push_back(std::make_pair("print",
			&engine::do_print));
		this->_action.push_back(std::make_pair("quit",
			&engine::do_quit));
		this->_action.push_back(std::make_pair("rotate",
			&engine::do_rotate));
		this->_action.push_back(std::make_pair("sd",
			&engine::do_sd));
		this->_action.push_back(std::make_pair("st",
			&engine::do_st));
		this->_action.push_back(std::make_pair("setboard",
			&engine::do_setboard));
		this->_action.push_back(std::make_pair("undo",
			&engine::do_undo));
		this->_action.push_back(std::make_pair("white",
			&engine::do_white));
	}

	engine& engine::init(void)
	{
		static engine engine;

		return engine;
	}

	void engine::run(void)
	{
		std::string line;
		std::vector<std::string> args;
		std::string::size_type idx_begin;
		std::string::size_type idx_end;
		std::string piece;
		std::string::const_iterator begin;
		std::vector<std::pair<std::string, do_action> >::const_iterator
			pos;

		this->_board.opening();
		this->print();

		for (;;)
		{
			this->prompt();

			do
			{
				this->_io << io::wait << io::flush;
				if (!this->_io.state())
				{
					exit(0);
				}
				this->_io.getline(line);
			} while (line.empty());

			idx_begin = 0;
			idx_end = 0;
			begin = line.begin();
			args.clear();
			while ((idx_end = line.find(' ', idx_begin))
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
			piece = std::string(begin + idx_begin,
				static_cast<std::string::const_iterator>(
				line.end()));
			if (!piece.empty())
			{
				args.push_back(piece);
			}

			if (args.empty())
			{
				continue;
			}
			// Args is produced

			for (pos = this->_action.begin();
				pos != this->_action.end(); ++pos)
			{
				if (pos->first == args[0])
				{
					(this->*pos->second)(args);
					goto done;
				}
			}

			// Process user move
			if (1 == args.size() && move::is_valid(args[0]))
			{
				move move(args[0]);
				if (this->_board.is_valid_move(move))
				{
					if (this->make_move(move))
					{
						this->print();
						goto done;
					}

					this->print();
					if (!this->result())
					{
						this->_io << io::nowait
							<< io::flush;
						this->go();
					}
					goto done;
				}

				this->_io << "Illegal move: " << args[0] << '\n';
				goto done;
			}

			this->_io << "Error (unknown command): " << args[0]
				<< '\n';
done:
			// Null statement
			;
		}
	}

	// ================================================================

	void engine::print(void)
	{
		int i;
		int j;

		if (this->_rotate)
		{
			this->_io << "       H   G   F   E   D   C   B   A\n";
			this->_io << "     +---+---+---+---+---+---+---+---+\n";
			for (i = 1; i <= 8; ++i)
			{
				this->_io << "  " << i;
				if (i % 2)
				{
					this->_io << "  | ";
				}
				for (j = i * 4 - 1; j >= i * 4 - 4; --j)
				{
					this->_io << "  |"
						<< this->to_string(
							bitboard(0x1 << j))
						<< "| ";
				}
				if (!(i % 2))
				{
					this->_io << "  | ";
				}
				this->_io << ' ' << i << '\n';
				this->_io << "     +---+---+---+---+---+---+---+---+\n";
			}
			this->_io << "       H   G   F   E   D   C   B   A\n";
		}
		else
		{
			this->_io << "       A   B   C   D   E   F   G   H\n";
			this->_io << "     +---+---+---+---+---+---+---+---+\n";
			for (i = 8; i >= 1; --i)
			{
				this->_io << "  " << i;
				if (!(i % 2))
				{
					this->_io << "  | ";
				}
				for (j = i * 4 - 4; j <= i * 4 - 1; ++j)
				{
					this->_io << "  |"
						<< this->to_string(
							bitboard(0x1 << j))
						<< "| ";
				}
				if (i % 2)
				{
					this->_io << "  | ";
				}
				this->_io << ' ' << i << '\n';
				this->_io << "     +---+---+---+---+---+---+---+---+\n";
			}
			this->_io << "       A   B   C   D   E   F   G   H\n";
		}
	}

	std::string engine::to_string(const bitboard& square)
	{
		assert(1 == square.bit_count());

		if (this->_board.get_black_men() & square)
		{
			return "(b)";
		}		
		if (this->_board.get_white_men() & square)
		{
			return "(w)";
		}		
		if (this->_board.get_black_kings() & square)
		{
			return "(B)";
		}		
		if (this->_board.get_white_kings() & square)
		{
			return "(W)";
		}
		return " \\ ";
	}

	std::string engine::to_string(int v)
	{
		if (UNLIMITED == v)
		{
			return "unlimited";
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
		int v = ::strtol(str.c_str(), NULL, 10);
		if (v < 0 || v > UNLIMITED)
		{
			return UNLIMITED;
		}
		return v;
	}

	bool engine::make_move(const move& move)
	{
		this->_history.push_back(this->_board);
		return this->_board.make_move(move);
	}

	void engine::go(void)
	{
		if (this->_force_mode)
		{
			return;
		}

		bool contin;
		std::vector<move> best_moves;
		std::vector<move>::size_type i;

		this->_io << "  Thinking ...\n";

		do
		{
			best_moves = intelligence::think(this->_board,
				this->_depth_limit, this->_time_limit,
				&this->_io);
			if (best_moves.empty())
			{
				break;
			}
			i = 0;
			do
			{
				this->_io << "move " << best_moves[i] << '\n';
				contin = this->make_move(best_moves[i]);
				this->print();
				++i;
			} while (contin && i < best_moves.size());
		} while (contin);
		this->result();
	}

	void engine::prompt(void)
	{
		this->_io << "  *** "
			<< (this->_board.is_black_move() ? "Black" : "White")
			<< " move ***\n";
	}

	bool engine::result(void)
	{
		if (this->_board.is_winning())
		{
			this->_io << "RESULT "
				<< (this->_board.is_black_move() ?
				"1-0 {Black win}\n" :
				"0-1 {White win}\n");
			return true;
		}
		else if (this->_board.is_losing())
		{
			this->_io << "RESULT "
				<< (this->_board.is_black_move() ?
				"0-1 {White win}\n" :
				"1-0 {Black win}\n");
			return true;
		}

		return false;
	}

	void engine::do_analyze(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_io << "  Analyzing ...\n";
		(void)intelligence::think(this->_board, this->_depth_limit,
			this->_time_limit, &this->_io);
	}

	void engine::do_print(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->print();
	}

	void engine::do_rotate(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_rotate = !this->_rotate;
		this->print();
	}

	void engine::do_black(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_board.set_black();
	}

	void engine::do_white(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_board.set_white();
	}

	void engine::do_ping(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_io << "pong";
		if (1 < args.size())
		{
			this->_io << ' ' << args[1];
		}
		this->_io << '\n';
	}

	void engine::do_go(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_force_mode = false;
		this->go();
	}

	void engine::do_help(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_io << "    analyze         Engine thinks about what move it make next if it were on\n";
		this->_io << "                    move.\n";
		this->_io << "    black           Set Black on move.  Set the engine to play White.\n";
		this->_io << "    force           Set the engine to play neither color (\"force mode\").\n";
		this->_io << "    go              Leave force mode and set the engine to play the color that\n";
		this->_io << "                    is on move.  Start thinking and eventually make a move.\n";
		this->_io << "    help            Show this help information.\n";
		this->_io << "    new             Reset the board to the standard starting position.\n";
		this->_io << "    ping N          N is a decimal number.  Reply by sending the string\n";
		this->_io << "                    \"pong N\"\n";
		this->_io << "    print           Show the current board.\n";
		this->_io << "    quit            Quit this program.\n";
		this->_io << "    rotate          Rotate the board 180 degrees.\n";
		this->_io << "    setboard FEN    Set up the pieces position on the board.\n";
		this->_io << "    sd DEPTH        The engine should limit its thinking to DEPTH ply.\n";
		this->_io << "    st TIME         Set the time control to TIME seconds per move.\n";
		this->_io << "    white           Set White on move.  Set the engine to play Black.\n";
		this->_io << "    undo            Back up a move.\n";
	}

	void engine::do_new(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_board.opening();
		this->_history.clear();
		this->print();
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
			this->_io << "Error (option missing): sd\n";
			return;
		}
		this->_depth_limit = this->to_int(args[1]);
	}

	void engine::do_st(const std::vector<std::string>& args)
	{
		if (args.size() <= 1)
		{
			this->_io << "Error (option missing): st\n";
			return;
		}
		this->_time_limit  = this->to_int(args[1]);
	}

	void engine::do_setboard(const std::vector<std::string>& args)
	{
		if (args.size() <= 1)
		{
			this->_io << "Error (option missing): setboard\n";
			return;
		}
		this->_board = board(args.size() > 2 ?
			(args[1] + ' ' + args[2]) : args[1]);
		this->_history.clear();
		this->print();
	}

	void engine::do_undo(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		if (this->_history.size() > 0)
		{
			this->_board = this->_history.back();
			this->_history.pop_back();
			this->print();
		}
		else
		{
			this->_io << "Error: No moves to undo!\n";
		}
	}

	void engine::not_implemented(const std::vector<std::string>& args)
	{
		this->_io << "Error (command not implemented): " << args[0] << '\n';
	}
}

// End of file
