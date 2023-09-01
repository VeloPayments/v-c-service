/**
 * \file log/vcservice_log_create_using_standard_output.c
 *
 * \brief Create a logger that logs to standard output.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <unistd.h>
#include <vcservice/error_codes.h>

#include "log_internal.h"

RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

/**
 * \brief Create a \ref vcservice_log instance that logs to standard output,
 * using the given threshold log level.
 *
 * \param log                   Pointer to the \ref vcservice_log pointer to
 *                              receive this resource on success.
 * \param alloc                 Pointer to the allocator to use for creating
 *                              this \ref vcservice_log instance.
 * \param threshold_level       The threshold level for logging messages.
 *
 * Log messages are written to standard output if they are more critical than
 * (less than or equal to) the threshold log level.
 *
 * \note This \ref vcservice_log instance is a \ref resource that must be
 * released by calling \ref resource_release on its resource handle when it is
 * no longer needed by the caller.  The resource handle can be accessed by
 * calling \ref vcservice_log_resource_handle on this \ref vcservice_log
 * instance.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 *
 * \pre
 *      - \p log must not reference a valid logger instance and must not be
 *        NULL.
 *      - \p alloc must reference a valid \ref allocator and must not be NULL.
 *      - \p threshold_level must be a valid log level belonging to
 *        \ref vcservice_loglevel.
 *
 * \post
 *      - On success, \p log is set to a pointer to a valid \ref vcservice_log
 *        instance.
 *      - On failure, \p log is set to NULL and an error status is returned.
 */
status FN_DECL_MUST_CHECK
vcservice_log_create_using_standard_output(
    vcservice_log** log, RCPR_SYM(allocator)* alloc,
    unsigned int threshold_level)
{
    status retval, release_retval;
    int stdout_dup_desc = -1;
    psock* sock = NULL;

    /* duplicate the standard output descriptor. */
    stdout_dup_desc = dup(1);
    if (stdout_dup_desc < 0)
    {
        retval = VCSERVICE_ERROR_LOG_STDOUT_DUP;
        goto done;
    }

    /* create a psock instance for this duplicate descriptor. */
    retval = psock_create_from_descriptor(&sock, alloc, stdout_dup_desc);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_stdout_dup_desc;
    }

    /* the descriptor is now owned by the psock interface. */
    stdout_dup_desc = -1;

    /* create the logger from this psock instance. */
    retval = vcservice_log_create_from_psock(log, alloc, sock, threshold_level);
    if (STATUS_SUCCESS != retval)
    {
        goto cleanup_sock;
    }

    /* the socket is now owned by the logger. */
    sock = NULL;

    /* success. */
    retval = STATUS_SUCCESS;
    goto done;

cleanup_sock:
    if (NULL != sock)
    {
        release_retval = resource_release(psock_resource_handle(sock));
        if (STATUS_SUCCESS != release_retval)
        {
            retval = release_retval;
        }
    }

cleanup_stdout_dup_desc:
    if (stdout_dup_desc >= 0)
    {
        close(stdout_dup_desc);
    }

done:
    return retval;
}
