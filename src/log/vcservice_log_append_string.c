/**
 * \file log/vcservice_log_append_string.c
 *
 * \brief Append a string to the log message.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>

#include "log_internal.h"

/**
 * \brief Append a string value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The string value to append.
 */
void
vcservice_log_append_string(vcservice_log* log, const char* val)
{
    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));
    RCPR_MODEL_ASSERT(prop_vcservice_log_threshold_level_valid(level));

    /* calculate the current size of the log message. */
    size_t message_size = sizeof(log->log_message) - log->log_idx;

    /* get the length of the string. */
    size_t adjsize = strlen(val);
    if (adjsize > message_size)
    {
        adjsize = message_size;
    }

    /* copy the string. */
    memcpy(log->log_message + log->log_idx, val, adjsize);

    /* adjust the size. */
    log->log_idx += adjsize;
}
