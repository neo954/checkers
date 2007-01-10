/// @file console.cpp

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "engine.hpp"
#include "io.hpp"

namespace checkers
{
	engine::engine(void) :
		_board(), _rotate(false), _player(BLACK)
	{
		this->_action.push_back(std::make_pair("print",  &engine::do_print));
		this->_action.push_back(std::make_pair("rotate", &engine::do_rotate));
		this->_action.push_back(std::make_pair("black",  &engine::do_black));
		this->_action.push_back(std::make_pair("white",  &engine::do_white));
		this->_action.push_back(std::make_pair("ping",   &engine::do_ping));
		this->_action.push_back(std::make_pair("help",   &engine::do_help));
		this->_action.push_back(std::make_pair("new",    &engine::do_new));
		this->_action.push_back(std::make_pair("quit",   &engine::do_quit));
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
					goto again;
				}
			}

			// process move
			if (1 == args.size() && move::is_valid(args[0]))
			{
				if (this->make_move(move(args[0])))
				{
					this->print();
				}
				goto again;
			}

			io.write("Error: Illegal ");
			io.write((BLACK == this->_player) ? "black" : "white");
			io.write(" move - ");
			io.write(args[0]);
			io.write('\n');
again:
			args.clear();
			this->prompt();
		}
	}

	// ================================================================

	bool engine::make_move(const move& move)
	{
		io& io = io::init();

		if (BLACK == this->_player)
		{
			if (this->_board.get_black_jumpers())
			{
	 			if (this->_board.is_valid_black_jump(move))
				{
					this->_board.black_jump(move);
					if (this->_board.get_black_jumpers())
					{
						return true;
					}
				}
				else
				{
					io.write("Error: Illegal black move - ");
					io.write(move.to_string());
					io.write(", jump is forced\n");
					return false;
				}
			} 
			else if (this->_board.get_black_movers())
			{
				if (this->_board.is_valid_black_move(move))
				{
					this->_board.black_move(move);
				}
				else
				{
					io.write("Error: Illegal black move - ");
					io.write(move.to_string());
					io.write('\n');
					return false;
				}
			}
			else
			{
				assert("White win");
				throw std::runtime_error("White win");
			}
		}
		else
		{
			if (this->_board.get_white_jumpers())
			{
		 		if (this->_board.is_valid_white_jump(move))
				{
					this->_board.white_jump(move);
					if (this->_board.get_white_jumpers())
					{
						return true;
					}
				}
				else
				{
					io.write("Error: Illegal white move - ");
					io.write(move.to_string());
					io.write(", jump is forced\n");
					return false;
				}
			} 
			else if (this->_board.get_white_movers())
			{
				if (this->_board.is_valid_white_move(move))
				{	
					this->_board.white_move(move);
				}
				else
				{
					io.write("Error: Illegal white move - ");
					io.write(move.to_string());
					io.write('\n');
					return false;
				}
			}
			else
			{
				assert("Black win");
				throw std::runtime_error("Black win");
			}
		}

		this->switch_player();
		return true;
	}

	void engine::switch_player(void)
	{
		this->_player = (BLACK == this->_player) ? WHITE : BLACK;
	}

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
	}

	void engine::prompt(void)
	{
		io& io = io::init();

		if (BLACK == this->_player)
		{
			io.write("Info: Black move\n");
		}
		else
		{
			io.write("Info: White move\n");
		}
	}

	void engine::do_print(const std::vector<std::string>& args)
	{
		this->print();
	}

	void engine::do_rotate(const std::vector<std::string>& args)
	{
		this->rotate();
	}

	void engine::do_black(const std::vector<std::string>& args)
	{
		this->_player = BLACK;
	}

	void engine::do_white(const std::vector<std::string>& args)
	{
		this->_player = WHITE;
	}

	void engine::do_ping(const std::vector<std::string>& args)
	{
		io& io = io::init();

		io.write("pong");
		if (1 < args.size())
		{
			io.write(' ');
			io.write(args[1]);
		}
		io.write('\n');
	}

	void engine::do_help(const std::vector<std::string>& args)
	{
		io& io = io::init();

		io.write("Info: Help message\n");
		io.write(" ping N         N is a decimal number. Reply by sending the string \"pong N\"\n");
		io.write(" print          Show the current board.\n");
		io.write(" rotate         Rotate the board 180 degrees.\n");
		io.write(" black          Set Black on move. Set the engine to play White. Stop clocks.\n");
		io.write(" white          Set White on move. Set the engine to play Black. Stop clocks.\n");
		io.write(" help           Show this help information.\n");
		io.write(" new            Reset the board to the standard starting position.\n");
		io.write(" quit           Quit this program.\n");
	}

	void engine::do_new(const std::vector<std::string>& args)
	{
		this->_board.opening();
		this->_player = BLACK;
		this->print();
	}

	void engine::do_quit(const std::vector<std::string>& args)
	{
		exit(0);
	}
};
// End of file
