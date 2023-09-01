/**
 * \file log/vcservice_log_append_uuid.c
 *
 * \brief Append a uuid value.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <rcpr/uuid.h>
#include <string.h>

#include "log_internal.h"

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_uuid;

/**
 * \brief Append a UUID value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The uuid value to append.
 */
void
vcservice_log_append_uuid(vcservice_log* log, const RCPR_SYM(rcpr_uuid)* val)
{
    status retval;
    char* str;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));
    RCPR_MODEL_ASSERT(prop_uuid_valid(val));

    /* convert this uuid to a string. */
    retval = rcpr_uuid_to_string(&str, log->alloc, val);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

    /* log this string. */
    vcservice_log_append_string(log, str);

    /* clean up. */
    memset(str, 0, strlen(str));
    retval = rcpr_allocator_reclaim(log->alloc, str);
    if (STATUS_SUCCESS != retval)
    {
        goto done;
    }

done:
}
