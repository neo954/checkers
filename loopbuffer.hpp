/// @file loopbuffer.hpp

#ifndef __LOOPBUFFER_HPP__
#define __LOOPBUFFER_HPP__

namespace checkers
{
	class loopbuffer
	{
	public:
		explicit loopbuffer(int max_size = 512);
		~loopbuffer(void);

		int max_size(void) const;
		int size(void) const;
		bool is_full(void) const;
		bool is_empty(void) const;
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
};

#endif // __LOOPBUFFER_HPP__
// End of file
