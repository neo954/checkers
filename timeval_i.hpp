/** @file timeval_i.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#ifndef __TIMEVAL_I_HPP__
#define __TIMEVAL_I_HPP__

namespace checkers
{
	inline struct timeval operator -(const struct timeval& rhs)
	{
		struct timeval tv;
		tv.tv_sec  = -1 - rhs.tv_sec;
		tv.tv_usec = 1000000 - rhs.tv_usec;
		return tv;
	}

	inline struct timeval& operator +=(struct timeval& lhs,
		const struct timeval& rhs)
	{
		lhs.tv_sec  += rhs.tv_sec;
		lhs.tv_usec += rhs.tv_usec;
		if (lhs.tv_usec >= 1000000)
		{
			lhs.tv_usec -= 1000000;
			++lhs.tv_sec;
		}

		return lhs;
	}

	inline struct timeval& operator +=(struct timeval& lhs, time_t rhs)
	{
		lhs.tv_sec += rhs;
		return lhs;
	}

	inline struct timeval& operator -=(struct timeval& lhs,
		const struct timeval& rhs)
	{
		lhs += -rhs;
		return lhs;
	}

	inline struct timeval& operator -=(struct timeval& lhs, time_t rhs)
	{
		lhs += -rhs;
		return lhs;
	}

	inline struct timeval operator +(const struct timeval& lhs,
		const struct timeval& rhs)
	{
		struct timeval tv = lhs;
		tv += rhs;
		return tv;
	}

	inline struct timeval operator +(const struct timeval& lhs, time_t rhs)
	{
		struct timeval tv = lhs;
		tv += rhs;
		return tv;
	}

	inline struct timeval operator -(const struct timeval& lhs,
		const struct timeval& rhs)
	{
		return lhs + -rhs;
	}

	inline struct timeval operator -(const struct timeval& lhs, time_t rhs)
	{
		return lhs + -rhs;
	}

	inline bool operator >(const struct timeval& lhs,
		const struct timeval& rhs)
	{
		return lhs.tv_sec > rhs.tv_sec ||
			lhs.tv_sec == rhs.tv_sec && lhs.tv_usec > rhs.tv_usec;
	}

	inline bool operator <(const struct timeval& lhs,
		const struct timeval& rhs)
	{
		return rhs > lhs;
	}
}

#endif // __TIMEVAL_I_HPP__
// End of file
