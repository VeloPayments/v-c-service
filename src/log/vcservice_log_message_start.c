/**
 * \file log/vcservice_log_message_start.c
 *
 * \brief Start a log message.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>
#include <time.h>

#include "log_internal.h"

/**
 * \brief Start a new logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 */
void
vcservice_log_message_start(vcservice_log* log)
{
    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));

    /* clear the log message. */
    memset(log->log_message, 0, sizeof(log->log_message));

    /* reset the index. */
    log->log_idx = 0;

    /* get the current time. */
    time_t curr = time(NULL);

    /* convert to local time. */
    struct tm local;
    localtime_r(&curr, &local);

    /* calculate the current size of the log message. */
    size_t message_size = sizeof(log->log_message) - log->log_idx;

    /* print the local time to the buffer. */
    size_t adjsize =
        strftime(
            log->log_message + log->log_idx, message_size, "%Y-%m-%d %H:%M:%S ",
            &local);

    /* adjust the size. */
    log->log_idx += adjsize;
}
