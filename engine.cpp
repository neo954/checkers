/// @file engine.cpp

#include "engine.hpp"
#include "intelligence.hpp"
#include "io.hpp"

namespace checkers
{
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
		io& io = io::init();

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
			io.process();
			line = io.read_line();
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
			// args is produced

			for (pos = this->_action.begin();
				pos != this->_action.end(); ++pos)
			{
				if (pos->first == args[0])
				{
					(this->*pos->second)(args);
					goto done;
				}
			}

			// process move
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
					io.process();
					this->go();
					goto done;
				}
			}

			io.write("Error: Illegal ");
			io.write((this->_board.is_black_move()) ?
				"black" : "white");
			io.write(" move - ");
			io.write(args[0]);
			io.write('\n');
done:
			args.clear();
			this->prompt();
		}
	}

	// ================================================================

	void engine::print(void)
	{
		io& io = io::init();

		int i;
		int j;

		if (this->_rotate)
		{
			io.write("      H   G   F   E   D   C   B   A\n");
			io.write("    +---+---+---+---+---+---+---+---+\n");
			for (i = 1; i <= 8; ++i)
			{
				io.write(' ');
				io.write(i);
				if (i % 2)
				{
					io.write("  | ");
				}
				for (j = i * 4 - 1; j >= i * 4 - 4; --j)
				{
					io.write("  |");
					this->print(0x1 << j);
					io.write("| ");
				}
				if (!(i % 2))
				{
					io.write("  | ");
				}
				io.write(' ');
				io.write(i);
				io.write('\n');
				io.write("    +---+---+---+---+---+---+---+---+\n");
			}
			io.write("      H   G   F   E   D   C   B   A\n");
		}
		else
		{
			io.write("      A   B   C   D   E   F   G   H\n");
			io.write("    +---+---+---+---+---+---+---+---+\n");
			for (i = 8; i >= 1; --i)
			{
				io.write(' ');
				io.write(i);
				if (!(i % 2))
				{
					io.write("  | ");
				}
				for (j = i * 4 - 4; j <= i * 4 - 1; ++j)
				{
					io.write("  |");
					this->print(0x1 << j);
					io.write("| ");
				}
				if (i % 2)
				{
					io.write("  | ");
				}
				io.write(' ');
				io.write(i);
				io.write('\n');
				io.write("    +---+---+---+---+---+---+---+---+\n");
			}
			io.write("      A   B   C   D   E   F   G   H\n");
		}
	}

	void engine::print(bitboard square)
	{
		assert(1 == square.bit_count());

		io& io = io::init();

		if (this->_board.get_black_men() & square)
		{
			io.write("(b)");
		}		
		else if (this->_board.get_white_men() & square)
		{
			io.write("(w)");
		}		
		else if (this->_board.get_black_kings() & square)
		{
			io.write("(B)");
		}		
		else if (this->_board.get_white_kings() & square)
		{
			io.write("(W)");
		}
		else
		{
			io.write(" \\ ");
		}
	}

	void engine::rotate(void)
	{
		this->_rotate = !this->_rotate;
		this->print();
	}

	void engine::go(void)
	{
		io& io = io::init();
		bool cont;
		int val;
		std::vector<move> best_moves;
		std::vector<move>::const_iterator pos;

		do
		{
			best_moves.clear();

			io.write("  Thinking ...\n");
			io.process();

			intelligence intelligence(this->_board);
			val = intelligence.alpha_beta_search(best_moves, 12);

			if (best_moves.empty())
			{
				this->declare_winning();
				return;
			}

			io.write("  Best moves - ");
			io.write('(');
			io.write(val);
			io.write(')');
			for (pos = best_moves.begin();
				pos != best_moves.end(); ++pos)
			{
				io.write(' ');
				io.write(pos->to_string());
			}
			io.write('\n');

			io.write("Info: My move is: ");
			io.write(best_moves[0].to_string());
			io.write('\n');
			cont = this->_board.make_move(best_moves[0]);
			this->print();
		} while (cont);
	}

	void engine::prompt(void)
	{
		io& io = io::init();

		if (this->_board.is_black_move())
		{
			io.write("Info: Black move\n");
		}
		else
		{
			io.write("Info: White move\n");
		}
	}

	void engine::declare_winning(void)
	{
		io& io = io.init();

		if (this->_board.is_winning())
		{
			io.write("Info: *** ");
			io.write(this->_board.is_black_move() ?
				"Black" : "White");
			io.write(" win ***\n");
		}
		else if (this->_board.is_losing())
		{
			io.write("Info: *** ");
			io.write(this->_board.is_black_move() ?
				"White" : "Black");
			io.write(" win ***\n");
		}
	}

	void engine::do_print(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		this->print();
	}

	void engine::do_rotate(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		this->rotate();
	}

	void engine::do_black(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		//this->_player = BLACK;
	}

	void engine::do_white(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		//this->_player = WHITE;
	}

	void engine::do_ping(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		io& io = io::init();

		io.write("pong");
		if (1 < args.size())
		{
			io.write(' ');
			io.write(args[1]);
		}
		io.write('\n');
	}

	void engine::do_go(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		this->go();
	}

	void engine::do_help(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		io& io = io::init();

		io.write("Info: Help message\n");
		io.write("  black         Set Black on move. Set the engine to play White.\n");
		io.write("  go            Set the engine to play the color that is on move.\n");
		io.write("  help          Show this help information.\n");
		io.write("  new           Reset the board to the standard starting position.\n");
		io.write("  ping N        N is a decimal number. Reply by sending the string \"pong N\"\n");
		io.write("  print         Show the current board.\n");
		io.write("  quit          Quit this program.\n");
		io.write("  rotate        Rotate the board 180 degrees.\n");
		io.write("  white         Set White on move. Set the engine to play Black.\n");
	}

	void engine::do_new(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		this->_board.opening();
		this->print();
	}

	void engine::do_quit(const std::vector<std::string>& args)
	{
		// void the warning: unused parameter ‘args’
		(void)args;

		exit(0);
	}
}

// End of file
