/// @file ponder.cpp

#include <iostream>
#include "engine.hpp"

int main(void)
{
	try
	{
		checkers::engine& engine = checkers::engine::init();

		engine.run();
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "Runtime error: " << e.what() << std::endl;
		exit(1);
	}
	catch (std::exception& e)
	{
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(1);
	}

	return 0;
}

// End of file
