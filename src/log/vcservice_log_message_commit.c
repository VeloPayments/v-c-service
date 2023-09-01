/**
 * \file log/vcservice_log_message_commit.c
 *
 * \brief Commit the logging message to the logging medium.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include "log_internal.h"

/**
 * \brief Commit the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 */
void
vcservice_log_message_commit(vcservice_log* log)
{
    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));

    /* append a newline. */
    vcservice_log_append_string(log, "\n");

    /* call the log write handler. */
    log->log_write_cb(log, log->log_level, log->user_context);
}
