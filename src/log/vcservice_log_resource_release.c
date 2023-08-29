/**
 * \file log/vcservice_log_resource_release.c
 *
 * \brief Release a \ref vcservice_log resource.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <string.h>

#include "log_internal.h"

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_resource;

/**
 * \brief Release the \ref vcservice_log resource.
 *
 * \param r             The resource to release.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status
vcservice_log_resource_release(
    RCPR_SYM(resource)* r)
{
    vcservice_log* log = (vcservice_log*)r;
    status user_context_release_retval = STATUS_SUCCESS;
    status reclaim_retval = STATUS_SUCCESS;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));

    /* cache allocator. */
    rcpr_allocator* alloc = log->alloc;

    /* release the user context if set. */
    if (NULL != log->user_context)
    {
        user_context_release_retval = resource_release(log->user_context);
    }

    /* clear memory. */
    memset(log, 0, sizeof(*log));

    /* reclaim memory. */
    reclaim_retval = rcpr_allocator_reclaim(alloc, log);

    /* decode return code. */
    if (STATUS_SUCCESS != user_context_release_retval)
    {
        return user_context_release_retval;
    }
    else
    {
        return reclaim_retval;
    }
}
