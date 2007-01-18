/** @file ponder.cpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#include <iostream>
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
		std::cerr << "Fatal error: " << e.what() << std::endl;
		exit(1);
	}

	return 0;
}

// End of file
