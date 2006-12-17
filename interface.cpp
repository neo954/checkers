/// @file interface.cpp

#include <iostream>
#include "interface.hpp"

namespace checkers
{
	interface::interface(void)
	{
		this->_rotate = false;
	}

	interface::~interface(void)
	{
	}

	void interface::rotate(void)
	{
		this->_rotate = !this->_rotate;
	}
};
// End of file
