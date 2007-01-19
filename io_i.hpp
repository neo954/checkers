/** @file io_i.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 *  @date $Date: 2007-01-19 14:40:56 $
 *  @version $Revision: 1.6 $
 */

#ifndef __IO_I_HPP__
#define __IO_I_HPP__

#include <sstream>

namespace checkers
{
	inline io& io::init(void)
	{
		static io io;

		return io;
	}

	template<typename T>
	io& io::operator <<(const T& rhs)
	{
		std::ostringstream stream;
		stream.setf(std::ios::fixed);
		stream << rhs;
		this->_write_buf.push_back(stream.str());
		return *this;
	}

	inline io& io::operator <<(char rhs)
	{
		this->_write_buf.push_back(rhs);
		return *this;
	}

	inline io& io::operator <<(const std::string& rhs)
	{
		this->_write_buf.push_back(rhs);
		return *this;
	}

	inline io& io::operator <<(const char* rhs)
	{
		this->_write_buf.push_back(rhs);
		return *this;
	}

	inline io& io::operator <<(io& (*op)(io&))
	{
		return (*op)(*this);
	}

	inline io& io::get_line(std::string& str)
	{
		str = this->_read_buf.get_line();
		return *this;
	}

	inline io& io::wait(io& io)
	{
		io._wait = true;
		return io;
	}

	inline io& io::nowait(io& io)
	{
		io._wait = false;
		return io;
	}

	inline io& io::endl(io& io)
	{
		io << '\n' << io::flush;
		return io;
	}
}

#endif // __IO_I_HPP
// End of file
