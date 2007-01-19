/** @file timeval.hpp
 *  @brief
 *  @author GONG Jie <neo@mamiyami.com>
 *  @date $Date: 2007-01-19 14:40:56 $
 *  @version $Revision: 1.3 $
 */

#ifndef __TIMEVAL_HPP__
#define __TIMEVAL_HPP__

#include <sys/time.h>
#include <ctime>

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
