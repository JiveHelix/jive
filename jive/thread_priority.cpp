#ifndef _WIN32
// Implemented for POSIX

#include <pthread.h>
#include "jive/thread_priority.h"
#include <iostream>


namespace jive
{


/*
ESRCH  No thread with the ID thread could be found.

pthread_setschedparam() may additionally fail with the following
errors:

EINVAL policy is not a recognized policy, or param does not make
      sense for the policy.

EPERM  The caller does not have appropriate privileges to set the
      specified scheduling policy and parameters.

POSIX.1 also documents an ENOTSUP ("attempt was made to set the
policy or scheduling parameters to an unsupported value") error for
       pthread_setschedparam().
*/

void SetFifoPriority(std::thread &thread, int priorityOffset)
{
    if (priorityOffset > 0)
    {
        priorityOffset = 0;
        std::cerr << "[Warning] " << __FUNCTION__
            << ": priorityOffset higher than 0 is ignored."
            << std::endl;
    }

    int minPriority = sched_get_priority_min(SCHED_FIFO);
    int desiredPriority = sched_get_priority_max(SCHED_FIFO) - priorityOffset;
    if (desiredPriority < minPriority)
    {
        std::cerr << "[Warning] " << __FUNCTION__
            << ": priorityOffset exceeds the allowable range."
            << std::endl;
        desiredPriority = minPriority;
    }

    sched_param schedParam;

    schedParam.sched_priority = desiredPriority;

    int result = pthread_setschedparam(
        thread.native_handle(),
        SCHED_FIFO,
        &schedParam);

    if (!result)
    {
        // Success
        return;
    }

    switch (result)
    {
        case ESRCH:
            throw BadThreadId("No thread with the ID thread could be found");

        case EINVAL:
            throw BadPolicyOrParam(
                "policy is not a recognized policy, or param does not make "
                "sense for the policy");

        case EPERM:
            throw PermissionError(
                "The caller does not have appropriate privileges "
                "to set the specified scheduling policy and parameters");

        case ENOTSUP:
            throw BadPolicyOrParam(
                "attempt was made to set the policy or "
                "scheduling parameters to an unsupported value");

        default:
            throw ThreadPriorityError("Unknown error");
    }
}


int GetPriorityRange()
{
    return sched_get_priority_max(SCHED_FIFO)
        - sched_get_priority_min(SCHED_FIFO);
}


} // end namespace jive


#endif // _WIN32
