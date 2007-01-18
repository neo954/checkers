/** @file timeval.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 */

#ifndef __TIMEVAL_HPP__
#define __TIMEVAL_HPP__

#include <sys/time.h>
#include <time.h>

namespace checkers
{
	inline struct timeval operator -(const struct timeval& rhs);

	inline struct timeval& operator +=(struct timeval& lhs,
		const struct timeval& rhs);
	inline struct timeval& operator +=(struct timeval& lhs, time_t rhs);

	inline struct timeval& operator -=(struct timeval& lhs,
		const struct timeval& rhs);
	inline struct timeval& operator -=(struct timeval& lhs, time_t rhs);

	inline struct timeval operator +(const struct timeval& lhs,
		const struct timeval& rhs);
	inline struct timeval operator +(const struct timeval& lhs, time_t rhs);

	inline struct timeval operator -(const struct timeval& lhs,
		const struct timeval& rhs);
	inline struct timeval operator -(const struct timeval& lhs, time_t rhs);

	inline bool operator >(const struct timeval& lhs,
		const struct timeval& rhs);
	inline bool operator <(const struct timeval& lhs,
		const struct timeval& rhs);
}

#include "timeval_i.hpp"
#endif // __TIMEVAL_HPP__
// End of file
