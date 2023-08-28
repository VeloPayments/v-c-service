/**
 * \file vcservice/log.h
 *
 * \brief Logger interface.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/allocator.h>
#include <rcpr/psock.h>
#include <rcpr/resource.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/

/**
 * \brief Forward decl for the logger interface.
 */
typedef struct vcservice_log vcservice_log;

/**
 * \brief Log levels.
 */
enum vcservice_loglevel
{
    VCSERVICE_LOGLEVEL_CRITICAL             =  0,
    VCSERVICE_LOGLEVEL_ERROR                = 10,
    VCSERVICE_LOGLEVEL_NORMAL               = 20,
    VCSERVICE_LOGLEVEL_INFO                 = 30,
    VCSERVICE_LOGLEVEL_VERBOSE              = 40,
    VCSERVICE_LOGLEVEL_DEBUG                = 50,
};

/******************************************************************************/
/* Start of constructors.                                                     */
/******************************************************************************/

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
vcservice_log_create_from_descriptor(
    vcservice_log** log, RCPR_SYM(allocator)* a, RCPR_SYM(psock)* sock,
    unsigned int threshold_level);

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
    vcservice_log** log, RCPR_SYM(allocator)* a, unsigned int threshold_level);

/******************************************************************************/
/* Start of accessors.                                                        */
/******************************************************************************/

/**
 * \brief Given a \ref vcservice_log instance, return its resource handle.
 *
 * \param log           The \ref vcservice_log insatnce from which the resource
 *                      handle is returned.
 *
 * \returns the resource handle for this \ref vcservice_log instance.
 */
RCPR_SYM(resource*)
vcservice_log_resource_handle(vcservice_log* log);

/**
 * \brief Given a \ref vcservice_log instance, return its logging threshold
 * level.
 *
 * \param log           The \ref vcservice_log insatnce from which the resource
 *                      handle is returned.
 *
 * \returns the logging threshold for this \ref vcservice_log instance.
 */
unsigned int
vcservice_log_threshold_level(const vcservice_log* log);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  /*__cplusplus*/
