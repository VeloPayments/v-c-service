/**
 * \file log/vcservice_log_write_psock.c
 *
 * \brief Write a log message to the given psock.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include "log_internal.h"

RCPR_IMPORT_psock;

/**
 * \brief Write the log message to the given psock (type erased as
 * user_context).
 *
 * \param log           The \ref vcservice_log instance.
 * \param log_level     The log level for the message to write.
 * \param user_context  The type erased psock.
 */
void
vcservice_log_write_psock(
    vcservice_log* log, unsigned int log_level,
    RCPR_SYM(resource)* user_context)
{
    status retval;

    /* the context is a psock instance. */
    /* TODO - use this instead of a raw cast. */
    /* psock* sock = psock_resource_handle_to_psock(user_context); */
    psock* sock = (psock*)user_context;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));
    RCPR_MODEL_ASSERT(prop_vcservice_log_threshold_level_valid(log_level));
    RCPR_MODEL_ASSERT(prop_psock_valid(sock));

    /* this interface ignores the log level. */
    (void)log_level;

    /* write the message to the socket. */
    retval = psock_write_raw_data(sock, log->log_message, log->log_idx);
    if (STATUS_SUCCESS != retval)
    {
        /* eat the failure for logging. */
        goto done;
    }

done:
}
