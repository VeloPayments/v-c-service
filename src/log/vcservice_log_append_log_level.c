/**
 * \file log/vcservice_log_append_log_level.c
 *
 * \brief Append the log level.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>

#include "log_internal.h"

static const char* log_level_to_string(unsigned int level);

/**
 * \brief Append the log level to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param level         The logging level to log.
 */
void
vcservice_log_append_log_level(vcservice_log* log, unsigned int level)
{
    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));
    RCPR_MODEL_ASSERT(prop_vcservice_log_threshold_level_valid(level));

    /* get the string to output. */
    const char* str = log_level_to_string(level);

    /* calculate the current size of the log message. */
    size_t message_size = sizeof(log->log_message) - log->log_idx;

    /* get the length of the string. */
    size_t adjsize = strlen(str);
    if (adjsize > message_size)
    {
        adjsize = message_size;
    }

    /* copy the string. */
    memcpy(log->log_message + log->log_idx, str, adjsize);

    /* adjust the size. */
    log->log_idx += adjsize;
}

/**
 * \brief Convert the log level to a string.
 */
static const char* log_level_to_string(unsigned int level)
{
    switch (level)
    {
        case VCSERVICE_LOGLEVEL_CRITICAL:
            return "CRITICAL";

        case VCSERVICE_LOGLEVEL_ERROR:
            return "ERROR";

        case VCSERVICE_LOGLEVEL_NORMAL:
            return "NORMAL";

        case VCSERVICE_LOGLEVEL_INFO:
            return "INFO";

        case VCSERVICE_LOGLEVEL_VERBOSE:
            return "VERBOSE";

        case VCSERVICE_LOGLEVEL_DEBUG:
            return "DEBUG";

        default:
            return "UNKNOWN";
    }
}
