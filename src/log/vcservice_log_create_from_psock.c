/**
 * \file log/vcservice_log_create_from_psock.c
 *
 * \brief Create a log instance from a psock.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>

#include "log_internal.h"

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

/**
 * \brief Create a \ref vcservice_log from a \ref psock and a threshold log
 * level.
 *
 * \param log                   Pointer to the \ref vcservice_log pointer to
 *                              receive this resource on success.
 * \param alloc                 Pointer to the allocator to use for creating
 *                              this \ref vcservice_log instance.
 * \param sock                  Pointer to the \ref psock to use for this
 *                              logger. This \ref psock instance is owned by
 *                              this logger instance and will be released when
 *                              it is released.
 * \param threshold_level       The threshold level for logging messages.
 *
 * Log messages are written to the \ref psock if they are more critical than
 * (less than or equal to) the threshold log level.
 *
 * \note This \ref vcservice_log instance is a \ref resource that must be
 * released by calling \ref resource_release on its resource handle when it is
 * no longer needed by the caller.  The resource handle can be accessed by
 * calling \ref vcservice_log_resource_handle on this \ref vcservice_log
 * instance.  The \ref psock is owned by this log interface on success and will
 * be released when it is released.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 *
 * \pre
 *      - \p log must not reference a valid logger instance and must not be
 *        NULL.
 *      - \p alloc must reference a valid \ref allocator and must not be NULL.
 *      - \p sock must reference a valid \ref psock and must not be NULL.
 *      - \p threshold_level must be a valid log level belonging to
 *        \ref vcservice_loglevel.
 *
 * \post
 *      - On success, \p log is set to a pointer to a valid \ref vcservice_log
 *        instance.
 *      - On failure, \p log is set to NULL and an error status is returned.
 */
status FN_DECL_MUST_CHECK
vcservice_log_create_from_psock(
    vcservice_log** log, RCPR_SYM(allocator)* alloc, RCPR_SYM(psock)* sock,
    unsigned int threshold_level)
{
    status retval;
    vcservice_log* tmp;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(NULL != log);
    RCPR_MODEL_ASSERT(rcpr_prop_allocator_valid(a));
    RCPR_MODEL_ASSERT(prop_psock_valid(sock));
    RCPR_MODEL_ASSERT(
        prop_vcservice_log_threshold_level_valid(threshold_level));

    /* allocate memory for this instance. */
    retval = rcpr_allocator_allocate(alloc, (void**)&tmp, sizeof(*tmp));
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* clear memory. */
    memset(tmp, 0, sizeof(*tmp));

    /* initialize resource. */
    resource_init(&tmp->hdr, &vcservice_log_resource_release);
    tmp->alloc = alloc;
    tmp->threshold_level = threshold_level;
    tmp->user_context = psock_resource_handle(sock);
    tmp->log_write_cb = &vcservice_log_write_psock;

    /* success. */
    *log = tmp;
    retval = STATUS_SUCCESS;
    goto done;

done:
    return retval;
}
