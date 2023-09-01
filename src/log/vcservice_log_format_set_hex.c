/**
 * \file log/vcservice_log_format_set_hex.c
 *
 * \brief Set the hex formatting bit.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include "log_internal.h"

/**
 * \brief Set the log formatting mode to hex.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The sentry value for this operation.
 */
void
vcservice_log_format_set_hex(
    vcservice_log* log, const vcservice_log_format_hex* val)
{
    (void)val;

    /* parameter sanity checks. */
    RCPR_MODEL_ASSERT(prop_vcservice_log_valid(log));

    /* clear the current formatting settings. */
    log->log_bits &= ~LOG_BITS_FORMAT_MASK;

    /* set the hex formatting bits. */
    log->log_bits |= LOG_BITS_FORMAT_HEX;
}
