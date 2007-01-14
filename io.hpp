/// @file io.hpp

#ifndef __IO_HPP__
#define __IO_HPP__

#include <fcntl.h>
#include <string>
#include "loopbuffer.hpp"

namespace checkers
{
	class io
	{
		enum wait
		{
			NOWAIT = 0,
			AWHILE = 10000
		};

	public:
                static io& init(void);

		inline std::string read_line(void);

		inline void write(char c);
		inline void write(const std::string& str);
		inline void write(const char* s);
		template<typename T>
        	void write(const T& v);

		void process(wait sleep = AWHILE);

	private:
		io(void);
		~io(void);
		io(const io& rhs);
		io& operator=(const io& rhs);

		loopbuffer _read_buf;
		loopbuffer _write_buf;

		void setfl(int fd, int flags);
	};
}

#include "io_i.hpp"
#endif // __IO_HPP__
// End of file
