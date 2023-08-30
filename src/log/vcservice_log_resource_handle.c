/**
 * \file log/vcservice_log_write_psock.c
 *
 * \brief Write a log message to the given psock.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include "log_internal.h"

/**
 * \brief Given a \ref vcservice_log instance, return its resource handle.
 *
 * \param log           The \ref vcservice_log instance from which the resource
 *                      handle is returned.
 *
 * \returns the resource handle for this \ref vcservice_log instance.
 */
RCPR_SYM(resource*)
vcservice_log_resource_handle(vcservice_log* log)
{
    return &log->hdr;
}
