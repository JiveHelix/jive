#pragma once


#ifndef _WIN32
// Iplemented for POSIX


#include <thread>
#include "jive/create_exception.h"


namespace jive
{


CREATE_EXCEPTION(ThreadPriorityError, std::runtime_error);
CREATE_EXCEPTION(BadThreadId, ThreadPriorityError);
CREATE_EXCEPTION(BadPolicyOrParam, ThreadPriorityError);
CREATE_EXCEPTION(PermissionError, ThreadPriorityError);


/*
 * Only use this method for high-priority real-time threads.
 * After a successful call, thread scheduling policy will be SCHED_FIFO
 *
 * @param thread target priority
 * @param priorityOffset Higheset priority is 0. Negative values set the
 * priority lower, offset from sched_get_priority_max(SCHED_FIFO)
 *
 */
void SetFifoPriority(std::thread &thread, int priorityOffset);


/*
 * Get the number of distinct SCHED_FIFO priorities available.
 */
int GetPriorityRange();


} // end namespace jive


#endif
