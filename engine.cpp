/** @file engine.cpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 *  @date $Date: 2007-01-19 14:40:56 $
 *  @version $Revision: 1.17 $
 */

#include "engine.hpp"
#include "intelligence.hpp"

namespace checkers
{
	engine::engine(void) :
		_board(), _rotate(false), _force_mode(false),
		_depth_limit(UNLIMITED), _time_limit(10)
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
			cio << io::wait << io::flush;
			cio.get_line(line);
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
					if (!this->result())
					{
						cio << io::nowait << io::flush;
						this->go();
					}
					goto done;
				}

				cio << "Illegal move: " << args[0] << '\n';
				goto done;
			}

			cio << "Error (unknown command): " << args[0] << '\n';
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
			cio << "       H   G   F   E   D   C   B   A\n";
			cio << "     +---+---+---+---+---+---+---+---+\n";
			for (i = 1; i <= 8; ++i)
			{
				cio << "  " << i;
				if (i % 2)
				{
					cio << "  | ";
				}
				for (j = i * 4 - 1; j >= i * 4 - 4; --j)
				{
					cio << "  |"
						<< this->to_string(
							bitboard(0x1 << j))
						<< "| ";
				}
				if (!(i % 2))
				{
					cio << "  | ";
				}
				cio << ' ' << i << '\n';
				cio << "     +---+---+---+---+---+---+---+---+\n";
			}
			cio << "       H   G   F   E   D   C   B   A\n";
		}
		else
		{
			cio << "       A   B   C   D   E   F   G   H\n";
			cio << "     +---+---+---+---+---+---+---+---+\n";
			for (i = 8; i >= 1; --i)
			{
				cio << "  " << i;
				if (!(i % 2))
				{
					cio << "  | ";
				}
				for (j = i * 4 - 4; j <= i * 4 - 1; ++j)
				{
					cio << "  |"
						<< this->to_string(
							bitboard(0x1 << j))
						<< "| ";
				}
				if (i % 2)
				{
					cio << "  | ";
				}
				cio << ' ' << i << '\n';
				cio << "     +---+---+---+---+---+---+---+---+\n";
			}
			cio << "       A   B   C   D   E   F   G   H\n";
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

		cio << "  Think ...\n";

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
				cio << "move " << best_moves[i] << '\n';
				contin = this->_board.make_move(best_moves[i]);
				this->print();
				++i;
			} while (contin && i < best_moves.size());
		} while (contin);
		this->result();
	}

	void engine::prompt(void)
	{
		if (this->_board.is_black_move())
		{
			cio << "  *** Black move\n";
		}
		else
		{
			cio << "  *** White move\n";
		}
	}

	bool engine::result(void)
	{
		if (this->_board.is_winning())
		{
			cio << "RESULT ";
			cio << (this->_board.is_black_move() ?
				"1-0 {Black win}\n" :
				"0-1 {White win}\n");
			return true;
		}
		else if (this->_board.is_losing())
		{
			cio << "RESULT ";
			cio << (this->_board.is_black_move() ?
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

		cio << "  Analyzing ...\n";
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

		cio << "pong";
		if (1 < args.size())
		{
			cio << ' ' << args[1];
		}
		cio << '\n';
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

		cio << "  Help message\n";
		cio << "    analyze     Engine thinks about what move it make next if it were on move.\n";
		cio << "    black       Set Black on move.  Set the engine to play White.\n";
		cio << "    force       Set the engine to play neither color (\"force mode\").\n";
		cio << "    go          Leave force mode and set the engine to play the color that is\n";
		cio << "                on move.  Start thinking and eventually make a move.\n";
		cio << "    help        Show this help information.\n";
		cio << "    new         Reset the board to the standard starting position.\n";
		cio << "    ping N      N is a decimal number. Reply by sending the string \"pong N\"\n";
		cio << "    print       Show the current board.\n";
		cio << "    quit        Quit this program.\n";
		cio << "    rotate      Rotate the board 180 degrees.\n";
		cio << "    white       Set White on move.  Set the engine to play Black.\n";
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

		if (size <= 1)
		{
			cio << "Error (option missing): set\n";
			return;
		}

		if ("all" == args[1])
		{
			cio << "  depth      (plys) "
				<< this->to_string(this->_depth_limit) << '\n';
			cio << "  time    (seconds) "
				<< this->to_string(this->_time_limit) << '\n';
		}
		else if ("depth" == args[1])
		{
			if (size <= 2)
			{
				cio << "Error (option missing): set depth\n";
				return;
			}
			this->_depth_limit = this->to_int(args[2]);
		}
		else if ("time" == args[1])
		{
			if (size <= 2)
			{
				cio << "Error (option missing): set time\n";
				return;
			}
			this->_time_limit  = this->to_int(args[2]);
		}
		else
		{
			cio << "Error (unknown option): " << args[1] << '\n';
		}
	}
}

// End of file
