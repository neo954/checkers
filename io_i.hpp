/// @file io_i.hpp

#ifndef __IO_I_HPP__
#define __IO_I_HPP__

#include <sstream>

namespace checkers
{
	inline std::string io::read_line(void)
	{
		return this->_read_buf.get_line();
	}

	inline void io::write(char c)
	{
		this->_write_buf.push_back(c);
	}

	inline void io::write(const std::string& str)
	{
		this->_write_buf.push_back(str);
	}

	inline void io::write(const char* s)
	{
		this->_write_buf.push_back(s);
	}

	template<typename T>
	inline void io::write(const T& v)
	{
		std::ostringstream stream;
		stream << std::ios::fixed << v;
		this->_write_buf.push_back(stream.str());
	}
}

#endif // __IO_I_HPP
// End of file
