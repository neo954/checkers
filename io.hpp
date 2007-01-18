/** @file io.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#ifndef __IO_HPP__
#define __IO_HPP__

#include <fcntl.h>
#include <string>
#include "loopbuffer.hpp"

namespace checkers
{
	class io
	{
	public:
                inline static io& init(void);

		template<typename T>
		io& operator <<(const T& rhs);
		inline io& operator <<(char rhs);
		inline io& operator <<(const std::string& rhs);
		inline io& operator <<(const char* rhs);
		inline io& operator <<(io& (*op)(io&));

		inline io& get_line(std::string& str);

		static io& wait(io& io);
		static io& nowait(io& io);
		static io& flush(io& io);
		static io& endl(io& io);

	private:
		io(void);
		~io(void);
		/// Define but not implement, to prevent object copy.
		io(const io& rhs);
		/// Define but not implement, to prevent object copy.
		io& operator=(const io& rhs) const;

		loopbuffer _read_buf;
		loopbuffer _write_buf;
		bool _wait;

		void setfl(int fd, int flags);
	};

	extern io& cio;
}

#include "io_i.hpp"
#endif // __IO_HPP__
// End of file
