/// @file engine.cpp

#include "engine.hpp"
#include "intelligence.hpp"

namespace checkers
{
	engine::engine(void) :
		_board(), _rotate(false), _force_mode(false),
		_depth_limit(INT_MAX), _time_limit(10),
		_io(io::init())
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
		this->_action.push_back(std::make_pair("print",
			&engine::do_print));
		this->_action.push_back(std::make_pair("quit",
			&engine::do_quit));
		this->_action.push_back(std::make_pair("rotate",
			&engine::do_rotate));
		this->_action.push_back(std::make_pair("set",
			&engine::do_set));
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
		this->prompt();

		for (;;)
		{
			this->_io.process();
			line = this->_io.read_line();
			if (line.empty())
			{
				continue;
			}

			assert('\n' == line.at(line.size() - 1));

			idx_begin = 0;
			idx_end = 0;
			begin = line.begin();
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
				line.end() - 1));
			if (!piece.empty())
			{
				args.push_back(piece);
			}

			if (args.empty())
			{
				this->prompt();
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
					if (this->_board.make_move(move))
					{
						this->print();
						goto done;
					}

					this->print();
					this->declare_winning();
					this->_io.process();
					this->go();
					goto done;
				}

				this->_io.write("Illegal move: ");
				this->_io.write(args[0]);
				this->_io.write('\n');
				goto done;
			}

			this->_io.write("Error (unknown command): ");
			this->_io.write(args[0]);
			this->_io.write('\n');
done:
			args.clear();
			this->prompt();
		}
	}

	// ================================================================

	void engine::print(void)
	{
		int i;
		int j;

		if (this->_rotate)
		{
			this->_io.write("       H   G   F   E   D   C   B   A\n");
			this->_io.write("     +---+---+---+---+---+---+---+---+\n");
			for (i = 1; i <= 8; ++i)
			{
				this->_io.write("  ");
				this->_io.write(i);
				if (i % 2)
				{
					this->_io.write("  | ");
				}
				for (j = i * 4 - 1; j >= i * 4 - 4; --j)
				{
					this->_io.write("  |");
					this->print(0x1 << j);
					this->_io.write("| ");
				}
				if (!(i % 2))
				{
					this->_io.write("  | ");
				}
				this->_io.write(' ');
				this->_io.write(i);
				this->_io.write('\n');
				this->_io.write("     +---+---+---+---+---+---+---+---+\n");
			}
			this->_io.write("       H   G   F   E   D   C   B   A\n");
		}
		else
		{
			this->_io.write("       A   B   C   D   E   F   G   H\n");
			this->_io.write("     +---+---+---+---+---+---+---+---+\n");
			for (i = 8; i >= 1; --i)
			{
				this->_io.write("  ");
				this->_io.write(i);
				if (!(i % 2))
				{
					this->_io.write("  | ");
				}
				for (j = i * 4 - 4; j <= i * 4 - 1; ++j)
				{
					this->_io.write("  |");
					this->print(0x1 << j);
					this->_io.write("| ");
				}
				if (i % 2)
				{
					this->_io.write("  | ");
				}
				this->_io.write(' ');
				this->_io.write(i);
				this->_io.write('\n');
				this->_io.write("     +---+---+---+---+---+---+---+---+\n");
			}
			this->_io.write("       A   B   C   D   E   F   G   H\n");
		}
	}

	void engine::print(bitboard square)
	{
		assert(1 == square.bit_count());

		if (this->_board.get_black_men() & square)
		{
			this->_io.write("(b)");
		}		
		else if (this->_board.get_white_men() & square)
		{
			this->_io.write("(w)");
		}		
		else if (this->_board.get_black_kings() & square)
		{
			this->_io.write("(B)");
		}		
		else if (this->_board.get_white_kings() & square)
		{
			this->_io.write("(W)");
		}
		else
		{
			this->_io.write(" \\ ");
		}
	}

	void engine::rotate(void)
	{
		this->_rotate = !this->_rotate;
		this->print();
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

		this->_io.write("  Think ...\n");

		do
		{
			best_moves = intelligence::think(this->_board,
				this->_depth_limit, this->_time_limit);
			if (best_moves.empty())
			{
				break;
			}
			i = 0;
			do
			{
				this->_io.write("move ");
				this->_io.write(best_moves[i].to_string());
				this->_io.write('\n');
				contin = this->_board.make_move(best_moves[i]);
				this->print();
				++i;
			} while (contin && i < best_moves.size());
		} while (contin);
		this->declare_winning();
	}

	void engine::prompt(void)
	{
		if (this->_board.is_black_move())
		{
			this->_io.write("  *** Black move\n");
		}
		else
		{
			this->_io.write("  *** White move\n");
		}
	}

	void engine::declare_winning(void)
	{
		if (this->_board.is_winning())
		{
			this->_io.write("RESULT ");
			this->_io.write(this->_board.is_black_move() ?
				"1-0 {Black win}\n" :
				"0-1 {White win}\n");
		}
		else if (this->_board.is_losing())
		{
			this->_io.write("RESULT ");
			this->_io.write(this->_board.is_black_move() ?
				"0-1 {White win}\n" :
				"1-0 {Black win}\n");
		}
	}

	void engine::do_analyze(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_io.write("  Analyzing ...\n");
		(void)intelligence::think(this->_board, this->_depth_limit,
			this->_time_limit);
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

		this->rotate();
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

		this->_io.write("pong");
		if (1 < args.size())
		{
			this->_io.write(' ');
			this->_io.write(args[1]);
		}
		this->_io.write('\n');
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

		this->_io.write(
			"  Help message\n"
			"    analyze     Engine thinks about what move it make next if it were on move.\n"
			"    black       Set Black on move.  Set the engine to play White.\n"
			"    force       Set the engine to play neither color (\"force mode\").\n"
			"    go          Leave force mode and set the engine to play the color that is\n"
			"                on move.  Start thinking and eventually make a move.\n"
			"    help        Show this help information.\n"
			"    new         Reset the board to the standard starting position.\n"
			"    ping N      N is a decimal number. Reply by sending the string \"pong N\"\n"
			"    print       Show the current board.\n"
			"    quit        Quit this program.\n"
			"    rotate      Rotate the board 180 degrees.\n"
			"    white       Set White on move.  Set the engine to play Black.\n"
		);
	}

	void engine::do_new(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_board.opening();
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

	void engine::do_set(const std::vector<std::string>& args)
	{
		std::vector<move>::size_type size = args.size();

		if (size < 1)
		{
			this->_io.write("Error (option missing): set\n");
		}

		if ("all" == args[1])
		{
			if (this->_depth_limit < INT_MAX)
			{
				this->_io.write("  Depth limit: ");
				this->_io.write(this->_depth_limit);
				this->_io.write(" ply(s)\n");
			}
			if (this->_time_limit < INT_MAX)
			{
				this->_io.write("  Time limit:  ");
				this->_io.write(this->_time_limit);
				this->_io.write(" second(s)\n");
			}
		}
		else if ("depth" == args[1])
		{
			if (size < 2)
			{
				this->_io.write("Error (option missing): set depth\n");
			}
			this->_depth_limit = strtol(args[2].c_str(),
				static_cast<char**>(NULL), 10);
			this->_time_limit = INT_MAX;
		}
		else if ("time" == args[1])
		{
			if (size < 2)
			{
				this->_io.write("Error (option missing): set time\n");
			}
			this->_depth_limit = INT_MAX;
			this->_time_limit = strtol(args[2].c_str(),
				static_cast<char**>(NULL), 10);
		}
		else
		{
			this->_io.write("Error (unknown option): ");
			this->_io.write(args[1]);
			this->_io.write('\n');
		}
	}
}

// End of file
