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

    /* save this log level to the logging instance. */
    log->log_level = level;

    /* get the string to output. */
    const char* str = log_level_to_string(level);

    vcservice_log_append_string(log, str);
}

/**
 * \brief Convert the log level to a string.
 */
static const char* log_level_to_string(unsigned int level)
{
    switch (level)
    {
        case VCSERVICE_LOGLEVEL_CRITICAL:
            return "CRITICAL ";

        case VCSERVICE_LOGLEVEL_ERROR:
            return "ERROR ";

        case VCSERVICE_LOGLEVEL_NORMAL:
            return "NORMAL ";

        case VCSERVICE_LOGLEVEL_INFO:
            return "INFO ";

        case VCSERVICE_LOGLEVEL_VERBOSE:
            return "VERBOSE ";

        case VCSERVICE_LOGLEVEL_DEBUG:
            return "DEBUG ";

        default:
            return "UNKNOWN ";
    }
}
