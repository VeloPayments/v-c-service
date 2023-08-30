/**
 * \file log/vcservice_log_threshold_level.c
 *
 * \brief Get the threshold level for the given logger.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include "log_internal.h"

/**
 * \brief Given a \ref vcservice_log instance, return its logging threshold
 * level.
 *
 * \param log           The \ref vcservice_log instance from which the resource
 *                      handle is returned.
 *
 * \returns the logging threshold for this \ref vcservice_log instance.
 */
unsigned int
vcservice_log_threshold_level(const vcservice_log* log)
{
    return log->threshold_level;
}
