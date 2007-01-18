/** @file loopbuffer.cpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#include <errno.h>
#include <unistd.h>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include "loopbuffer.hpp"

namespace checkers
{
	char loopbuffer::front(void) const
	{
		assert(!this->is_empty());		

		return this->_buffer[this->_front];
	}

	void loopbuffer::pop_front(void)
	{
		assert(!this->is_empty());		

		if ('\n' == this->front())
		{
			--this->_lines;
		}
		this->_front = (this->_front + 1) % this->_max_size;
	}

	bool loopbuffer::read(int fd)
	{
		char c;
		ssize_t n;

		for (;;)
		{
			n = ::read(fd, &c, 1);
			if (n < 0)
			{
				if (EWOULDBLOCK == errno)
				{
					break;
				}
				std::ostringstream error;
				error << "Read error while read from fd - " << fd;
				throw std::runtime_error(error.str());
			}
			else if (0 == n)
			{
				// eof
				return false;
			}
			else
			{
				this->push_back(c);
			}
		}
		return true;
	}

	bool loopbuffer::write(int fd)
	{
		char c;
		ssize_t n;
		while (!this->is_empty())
		{
			c = this->front();
			n = ::write(fd, &c, 1);
			if (n < 0)
			{
				if (EWOULDBLOCK == errno)
				{
					break;
				}
				std::ostringstream error;
				error << "Write error while write to fd - " << fd;
				throw std::runtime_error(error.str());
			}
			else if (0 == n)
			{
				// eof
				return false;
			}
			else
			{
				this->pop_front();
			}
		}
		return true;
	}

	std::string loopbuffer::get_line(void)
	{
		std::string line;
		char c;

		if (this->_lines)
		{
			for (;;)
			{
				c = this->front();
				line += c;
				this->pop_front();
				if ('\n' == c)
				{
					break;
				}
			}
		}
		return line;
	}

	void loopbuffer::push_back(char c)
	{
		if (this->is_full())
		{
			this->enlarge();
		}

		if ('\n' == c)
		{
			++this->_lines;
		}
		this->_buffer[this->_rear] = c;
		this->_rear = (this->_rear + 1) % this->_max_size;
	}

	void loopbuffer::push_back(const std::string& str)
	{
		for (std::string::const_iterator pos = str.begin();
			pos != str.end(); ++pos)
		{
			this->push_back(*pos);
		}	
	}

	void loopbuffer::push_back(const char* s)
	{
		for (; *s != '\0'; ++s)
		{
			this->push_back(*s);
		}
	}

	// ================================================================

	void loopbuffer::enlarge(void)
	{
		assert(this->is_full());

		const int multiple = 2;
		char* buffer = new char[this->_max_size * multiple];
		if (this->_front < this->_rear)
		{
			memcpy(buffer, this->_buffer + this->_front,
				this->_rear - this->_front);
		}
		else
		{
			memcpy(buffer, this->_buffer + this->_front,
				this->_max_size - this->_front);
			memcpy(buffer + this->_max_size - this->_front,
				this->_buffer, this->_rear);
		}
		delete[] this->_buffer;
		this->_buffer = buffer;
		this->_front = 0;
		this->_rear = this->_max_size - 1;
		this->_max_size *= multiple;
	}
}

// End of file
