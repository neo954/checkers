/** @file loopbuffer.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 *  @date $Date: 2007-01-19 14:40:56 $
 *  @version $Revision: 1.5 $
 */

#ifndef __LOOPBUFFER_HPP__
#define __LOOPBUFFER_HPP__

namespace checkers
{
	class loopbuffer
	{
	public:
		explicit inline loopbuffer(int max_size = 512);
		inline ~loopbuffer(void);

		inline int max_size(void) const;
		inline int size(void) const;
		inline bool is_full(void) const;
		inline bool is_empty(void) const;
		char front(void) const;
		void pop_front(void);
		bool read(int fd);
		bool write(int fd);

		std::string get_line(void);
		void push_back(char c);
		void push_back(const std::string& str);
		void push_back(const char* s);

	private:
		loopbuffer(const loopbuffer& rhs);
		loopbuffer& operator=(const loopbuffer& rhs);

		void enlarge(void);

		char* _buffer;
		int _max_size;
		int _front;
		int _rear;
		int _lines;
	};
}

#include "loopbuffer_i.hpp"
#endif // __LOOPBUFFER_HPP__
// End of file
