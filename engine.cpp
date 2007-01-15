/// @file engine.cpp

#include <sys/time.h>
#include <time.h>
#include <iomanip>
#include "engine.hpp"
#include "intelligence.hpp"

namespace checkers
{
	io& engine::_io = io::init();

	engine::engine(void) :
		_board(), _rotate(false)
	{
		this->_action.push_back(std::make_pair("go",
			&engine::do_go));
		this->_action.push_back(std::make_pair("print",
			&engine::do_print));
		this->_action.push_back(std::make_pair("rotate",
			&engine::do_rotate));
		this->_action.push_back(std::make_pair("black",
			&engine::do_black));
		this->_action.push_back(std::make_pair("white",
			&engine::do_white));
		this->_action.push_back(std::make_pair("ping",
			&engine::do_ping));
		this->_action.push_back(std::make_pair("help",
			&engine::do_help));
		this->_action.push_back(std::make_pair("new",
			&engine::do_new));
		this->_action.push_back(std::make_pair("quit",
			&engine::do_quit));
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
		bool cont;
		int val;
		std::vector<move> best_moves;
		int depth;
		std::vector<move>::size_type i;
		struct timeval start_time;
		struct timeval terminal_time;

		do
		{
			best_moves.clear();

			this->_io.write("  Thinking ...\n");
			this->_io.process();

			for (depth = 1; depth <= 13; ++depth)
			{
				::gettimeofday(&start_time, NULL);
				intelligence intelligence(this->_board);
				intelligence::reset_nodes();
				intelligence.init_best_moves(best_moves);
				val = intelligence.alpha_beta_search(
					best_moves, depth);
				::gettimeofday(&terminal_time, NULL);

				if (best_moves.empty())
				{
					this->declare_winning();
					return;
				}

				this->print(depth, val,
					(terminal_time.tv_sec
						- start_time.tv_sec) * 1000000 +
					(terminal_time.tv_usec
						- start_time.tv_usec),
					intelligence::get_nodes(), best_moves);
				this->_io.process();
			}

			i = 0;
			do
			{
				this->_io.write("move ");
				this->_io.write(best_moves[i].to_string());
				this->_io.write('\n');
				cont = this->_board.make_move(best_moves[i]);
				this->print();
				++i;
			} while (cont && i < best_moves.size());
		} while (cont);
		this->declare_winning();
	}

	/// @param time microseconds
	void engine::print(int depth, int val, int time, int nodes,
		const std::vector<move>& best_moves)
	{
		std::ostringstream stream;
		std::vector<move>::size_type i;
		std::vector<move>::size_type max_size;

		if (1 == depth % 10)
		{
			stream << "  depth   value      time       nodes\n";
			stream << "  ----------------------------------------------------------------------------\n";
		}
		stream << "  " << std::setw(5) << depth;
		stream << "  " << std::setw(6) << val;
		stream << "  " << std::setw(4) << (time / 1000000) << '.' <<
			std::setw(3) << std::setfill('0') <<
			((time / 1000) % 1000) << std::setfill(' ');
		stream << "  " << std::setw(10) << nodes;
		stream << " ";

		max_size = best_moves.size();
		for (i = 0; i < max_size; ++i)
		{
			if (i > 0 && 0 == i % 8)
			{
				stream << "\n                                      ";
			}
			stream << " " << best_moves[i].to_string();
		}
		stream << '\n';
		this->_io.write(stream.str());
	}

	void engine::prompt(void)
	{
		if (this->_board.is_black_move())
		{
			this->_io.write("  Black move\n");
		}
		else
		{
			this->_io.write("  White move\n");
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

		this->go();
	}

	void engine::do_help(const std::vector<std::string>& args)
	{
		// Void the warning: unused parameter ‘args’
		(void)args;

		this->_io.write("  Help message\n");
		this->_io.write("    black       Set Black on move. Set the engine to play White.\n");
		this->_io.write("    go          Set the engine to play the color that is on move.\n");
		this->_io.write("    help        Show this help information.\n");
		this->_io.write("    new         Reset the board to the standard starting position.\n");
		this->_io.write("    ping N      N is a decimal number. Reply by sending the string \"pong N\"\n");
		this->_io.write("    print       Show the current board.\n");
		this->_io.write("    quit        Quit this program.\n");
		this->_io.write("    rotate      Rotate the board 180 degrees.\n");
		this->_io.write("    white       Set White on move. Set the engine to play Black.\n");
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
}

// End of file
