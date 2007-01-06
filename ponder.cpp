/// @file ponder.cpp

#include <iostream>
#include <stdexcept>
#include "engine.hpp"

int main(void)
{
	try
	{
		checkers::engine& engine = checkers::engine::init();

		engine.run();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}

	return 0;
}
// End of file
