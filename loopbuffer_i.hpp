/// @file loopbuffer_i.hpp

#ifndef __LOOPBUFFER_I_HPP__
#define __LOOPBUFFER_I_HPP__

namespace checkers
{
	inline loopbuffer::loopbuffer(int max_size) :
		_max_size(max_size), _front(0), _rear(0), _lines(0)
	{
		this->_buffer = new char[this->_max_size];
	}

	inline loopbuffer::~loopbuffer(void)
	{
		delete[] this->_buffer;
	}

	inline int loopbuffer::max_size(void) const
	{
		return this->_max_size;
	}

	inline int loopbuffer::size(void) const
	{
		return (this->_rear - this->_front + this->_max_size)
			% this->_max_size;
	}

	inline bool loopbuffer::is_full(void) const
	{
		return (this->_rear + 1) % this->_max_size == this->_front;
	}

	inline bool loopbuffer::is_empty(void) const
	{
		return this->_front == this->_rear;
	}
}

#endif // __LOOPBUFFER_I_HPP__
// End of file
