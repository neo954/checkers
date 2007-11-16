/* $Id: signal.cpp,v 1.8 2007-11-16 20:28:59 neo Exp $

   This file is a part of ponder, a English/American checkers game.

   Copyright (c) 2006, 2007 Mamiyami Information.
                     Gong Jie <neo@mamiyami.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Steet, Fifth Floor,
   Boston, MA 02110-1301, USA.
 */
/** @file signal.cpp
 *  @brief
 */

extern "C"
{
	#include <execinfo.h>
	#include <ucontext.h>
	#include <sys/time.h>
	#include <sys/resource.h>
}
#include <cerrno>
#include <cstdlib>
#include <stdexcept>
#include "signal.hpp"

namespace checkers
{
	sighandler_t signal(int signum, sighandler_t handler)
	{
		struct sigaction action;
		struct sigaction old_action;

		action.sa_handler = handler;
		// Block sigs of type being handled
		sigemptyset(&action.sa_mask);
		// Restart syscalls if possible
		action.sa_flags = SA_RESTART;

		if (::sigaction(signum, &action, &old_action) < 0) {
			/// @throw std::runtime_error when sigaction() failed.
			throw std::runtime_error(
				std::string("sigaction() failed: ") +
				std::strerror(errno));
		}

		/// @return the previous value of the signal handler.
		return (old_action.sa_handler);
	}

	sigaction_t signal(int signum, sigaction_t handler)
	{
		struct sigaction action;
		struct sigaction old_action;

		action.sa_sigaction = handler;
		// Block sigs of type being handled
		sigemptyset(&action.sa_mask);
		// Restart syscalls if possible
		action.sa_flags = SA_RESTART | SA_SIGINFO;

		if (::sigaction(signum, &action, &old_action) < 0) {
			/// @throw std::runtime_error when sigaction() failed.
			throw std::runtime_error(
				std::string("sigaction() failed: ") +
				std::strerror(errno));
		}

		/// @return the previous value of the signal handler.
		return (old_action.sa_sigaction);
	}

	void crash_handler(int signum, siginfo_t* siginfo, void* context)
	{
		printf("\n"
			"  =================================================="
				"====================\n"
			"  * Received Signal ");
		switch (signum)
		{
		case SIGABRT:
			printf("SIGABRT\n");
			goto common_si_code;
		case SIGBUS:
			printf("SIGBUS\n");
			switch (siginfo->si_code)
			{
			case BUS_ADRALN:
				printf("  * invalid address alignment\n");
				break;
			case BUS_ADRERR:
				printf("  * non-existent physical address\n");
				break;
			case BUS_OBJERR:
				printf("  * object specific hardware error\n");
				break;
			default:
				goto common_si_code;
			}
			break;
		case SIGCHLD:
			printf("SIGCHLD\n");
			switch (siginfo->si_code)
			{
			case CLD_EXITED   :
				printf("  * child has exited\n");
				break;
			case CLD_KILLED   :
				printf("  * child was killed\n");
				break;
			case CLD_DUMPED   :
				printf("  * child terminated abnormally\n");
				break;
			case CLD_TRAPPED  :
				printf("  * traced child has trapped\n");
				break;
			case CLD_STOPPED  :
				printf("  * child has stopped\n");
				break;
			case CLD_CONTINUED:
				printf("  * stopped child has continued"
					" (since Linux 2.6.9)\n");
				break;
			default:
				goto common_si_code;
			}
			break;
		case SIGFPE:
			printf("SIGFPE\n");
			switch (siginfo->si_code)
			{
			case FPE_INTDIV:
				printf("  * integer divide by zero\n");
				break;
			case FPE_INTOVF:
				printf("  * integer overflow\n");
				break;
			case FPE_FLTDIV:
				printf("  * floating point divide by zero\n");
				break;
			case FPE_FLTOVF:
				printf("  * floating point overflow\n");
				break;
			case FPE_FLTUND:
				printf("  * floating point underflow\n");
				break;
			case FPE_FLTRES:
				printf("  * floating point inexact result\n");
				break;
			case FPE_FLTINV:
				printf("  * floating point invalid"
					" operation\n");
				break;
			case FPE_FLTSUB:
				printf("  * subscript out of range\n");
				break;
			default:
				goto common_si_code;
			}
			break;
		case SIGILL:
			printf("SIGILL\n");
			switch (siginfo->si_code)
			{
			case ILL_ILLOPC:
				printf("  * illegal opcode\n");
				break;
			case ILL_ILLOPN:
				printf("  * illegal operand\n");
				break;
			case ILL_ILLADR:
				printf("  * illegal addressing mode\n");
				break;
			case ILL_ILLTRP:
				printf("  * illegal trap\n");
				break;
			case ILL_PRVOPC:
				printf("  * privileged opcode\n");
				break;
			case ILL_PRVREG:
				printf("  * privileged register\n");
				break;
			case ILL_COPROC:
				printf("  * coprocessor error\n");
				break;
			case ILL_BADSTK:
				printf("  * internal stack error\n");
				break;
			default:
				goto common_si_code;
			}
			break;
		case SIGPOLL:
			printf("SIGPOLL\n");
			switch (siginfo->si_code)
			{
			case POLL_IN :
				printf("  * data input available\n");
				break;
			case POLL_OUT:
				printf("  * output buffers available\n");
				break;
			case POLL_MSG:
				printf("  * input message available\n");
				break;
			case POLL_ERR:
				printf("  * i/o error\n");
				break;
			case POLL_PRI:
				printf("  * high priority input available\n");
				break;
			case POLL_HUP:
				printf("  * device disconnected\n");
				break;
			default:
				goto common_si_code;
			}
			break;
		case SIGSEGV:
			printf("SIGSEGV\n");
			switch (siginfo->si_code)
			{
			case SEGV_MAPERR:
				printf("  * address not mapped to object\n");
				break;
			case SEGV_ACCERR:
				printf("  * invalid permissions for mapped"
					" object\n");
				break;
			default:
				goto common_si_code;
			}
			break;
		case SIGTRAP:
			printf("SIGTRAP\n");
			switch (siginfo->si_code)
			{
			case TRAP_BRKPT:
				printf("  * process breakpoint\n");
				break;
			case TRAP_TRACE:
				printf("  * process trace trap\n");
				break;
			default:
				goto common_si_code;
			}
			break;
		default:
			printf("%d\n", signum);
		common_si_code:
			switch (siginfo->si_code)
			{
			case SI_USER   :
				printf("  * kill(), sigsend(), or raise()\n");
				break;
			case SI_KERNEL :
				printf("  * The kernel\n");
				break;
			case SI_QUEUE  :
				printf("  * sigqueue()\n");
				break;
			case SI_TIMER  :
				printf("  * POSIX timer expired\n");
				break;
			case SI_MESGQ  :
				printf("  * POSIX message queue state changed"
					" (since Linux 2.6.6)\n");
				break;
			case SI_ASYNCIO:
				printf("  * AIO completed\n");
				break;
			case SI_SIGIO  :
				printf("  * queued SIGIO\n");
				break;
			case SI_TKILL  :
				printf("  * tkill() or tgkill() (since Linux"
					" 2.4.19)\n");
				break;
			default:
				printf("  * Unknown or not-specified cause\n");
				break;
			}
			break;
		}

		printf("  * Creating caller backtrace...\n");
		void* callers[255];
		size_t stacks = backtrace(callers,
				sizeof(callers) / sizeof(callers[0]));
		char** call_names = backtrace_symbols(callers, stacks);
		for (size_t i = 0; i < stacks; ++i)
		{
			printf("  * %*p : %s\n", sizeof(void*),
				callers[i], call_names[i]);
		}

		printf("  ==================================================="
				"===================\n");

		/// cd to /tmp and perform a crash dump.
		(void)chdir("/tmp");

		const struct rlimit limit =
		{
			RLIM_INFINITY,
			RLIM_INFINITY
		};

		(void)setrlimit(RLIMIT_CORE, &limit);
		signal(SIGABRT, SIG_DFL);
		abort();
		// NOT REACHED
	}
}

// End of file
