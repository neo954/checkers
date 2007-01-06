#include <iostream>
#include <string>
#include "loopbuffer.hpp"

void test(bool value)
{
	if (value)
	{
		std::cout << "PASS" << std::endl;
		return;
	}
	std::cout << "FAIL" << std::endl;
	exit(1);
}

int main(void)
{
	checkers::loopbuffer buffer(4);

	for (char c = 32; c < 64; ++c)
	{
		std::cout << "Test push and pop one character " << c << " ......... ";
		buffer.push_back(c);
		char d = buffer.front();
		buffer.pop_front();
		test(c == d);
	}

	std::cout << "Test buffer size .......................... ";
	test(0 == buffer.size());
	std::cout << "Test buffer max size ...................... ";
	test(4 == buffer.max_size());

	for (char c = 32; c < 64; c += 2)
	{
		std::cout << "Test push and pop two character " << c << " " << static_cast<char>(c + 1) << " ....... ";
		buffer.push_back(c);
		buffer.push_back(c + 1);
		char d = buffer.front();
		buffer.pop_front();
		char e = buffer.front();
		buffer.pop_front();
		test(c == d && c + 1 == e);
	}

	std::cout << "Test buffer size .......................... ";
	test(0 == buffer.size());
	std::cout << "Test buffer max size ...................... ";
	test(4 == buffer.max_size());

	std::string str = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	for (std::string::iterator pos = str.begin(); pos != str.end(); ++pos)
	{
		buffer.push_back(*pos);
	}
	std::cout << buffer.size() << std::endl;
	std::cout << "Test buffer size .......................... ";
	test(static_cast<int>(str.size()) == buffer.size());
	std::cout << "Test buffer max size ...................... ";
	test(64 == buffer.max_size());

	std::string str2;
	while (!buffer.is_empty())
	{
		str2 += buffer.front();
		buffer.pop_front();
	}
	std::cout << str2 << std::endl;
	std::cout << "Compare two strings ....................... ";
	test(str2 == str);
	std::cout << "Test buffer size .......................... ";
	test(0 == buffer.size());
	std::cout << "Test buffer max size ...................... ";
	test(64 == buffer.max_size());
}
