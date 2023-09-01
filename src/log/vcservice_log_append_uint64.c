/**
 * \file log/vcservice_log_append_uint64.c
 *
 * \brief Append a uint64 value.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <stdio.h>

#include "log_internal.h"

static const char* log_format_string(const vcservice_log* log);

/**
 * \brief Append a 64-bit unsigned integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The unsigned integer value to append.
 */
void
vcservice_log_append_uint64(vcservice_log* log, uint64_t val)
{
    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));

    /* calculate the current size of the log message. */
    size_t message_size = sizeof(log->log_message) - log->log_idx;

    /* get the format string. */
    const char* format_string = log_format_string(log);

    /* print the string and get the adjusted size. */
    size_t adjsize =
        snprintf(
            log->log_message + log->log_idx, message_size, format_string, val);
    if (adjsize > message_size)
    {
        adjsize = message_size;
    }

    /* adjust the size. */
    log->log_idx += adjsize;
}

/**
 * \brief Get the log format string for this logging operation.
 */
static const char* log_format_string(const vcservice_log* log)
{
    /* should we log a hex value? */
    if (log->log_bits & LOG_BITS_FORMAT_HEX)
    {
        return "0x%016lx";
    }
    else
    {
        return "%lu";
    }
}
