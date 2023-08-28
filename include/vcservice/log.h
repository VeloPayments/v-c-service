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
#include <rcpr/uuid.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/

/**
 * \brief Forward decl for the logger interface.
 */
typedef struct vcservice_log vcservice_log;

/**
 * \brief Forward decl for the default log format.
 */
typedef struct vcservice_log_format_default vcservice_log_format_default;

/**
 * \brief The sentry value for the default log format.
 */
extern vcservice_log_format_default vcservice_log_format_default_sentry;

/**
 * \brief Forward decl for the hexadecimal log format.
 */
typedef struct vcservice_log_format_hex vcservice_log_format_hex;

/**
 * \brief The sentry value for the hexadecimal log format.
 */
extern vcservice_log_format_hex vcservice_log_format_hex_sentry;

/**
 * \brief Forward decl for the end-of-message type.
 */
typedef struct vcservice_log_end_of_message vcservice_log_end_of_message;

/**
 * \brief The end-of-message sentry value.
 */
extern vcservice_log_end_of_message vcservice_log_end_of_message_sentry;

/**
 * \brief Log levels.
 */
enum vcservice_loglevel
{
    VCSERVICE_LOGLEVEL_CRITICAL             =  0,
    VCSERVICE_LOGLEVEL_ERROR                =  1,
    VCSERVICE_LOGLEVEL_NORMAL               =  2,
    VCSERVICE_LOGLEVEL_INFO                 =  3,
    VCSERVICE_LOGLEVEL_VERBOSE              =  4,
    VCSERVICE_LOGLEVEL_DEBUG                =  5,
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
/* Start of public methods.                                                   */
/******************************************************************************/

/**
 * \brief Start a new logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 */
void
vcservice_log_message_start(vcservice_log* log);

/**
 * \brief Append the log level to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param level         The logging level to log.
 */
void
vcservice_log_append_log_level(vcservice_log* log, unsigned int level);

/**
 * \brief Append a string value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The string value to append.
 */
void
vcservice_log_append_string(vcservice_log* log, const char* val);

/**
 * \brief Append an 8-bit integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The integer value to append.
 */
void
vcservice_log_append_int8(vcservice_log* log, int8_t val);

/**
 * \brief Append an 8-bit unsigned integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The unsigned integer value to append.
 */
void
vcservice_log_append_uint8(vcservice_log* log, uint8_t val);

/**
 * \brief Append a 16-bit integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The integer value to append.
 */
void
vcservice_log_append_int16(vcservice_log* log, int16_t val);

/**
 * \brief Append a 16-bit unsigned integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The unsigned integer value to append.
 */
void
vcservice_log_append_uint16(vcservice_log* log, uint16_t val);

/**
 * \brief Append a 32-bit integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The integer value to append.
 */
void
vcservice_log_append_int32(vcservice_log* log, int32_t val);

/**
 * \brief Append a 32-bit unsigned integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The unsigned integer value to append.
 */
void
vcservice_log_append_uint32(vcservice_log* log, uint32_t val);

/**
 * \brief Append a 64-bit integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The integer value to append.
 */
void
vcservice_log_append_int64(vcservice_log* log, int64_t val);

/**
 * \brief Append a 64-bit unsigned integer value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The unsigned integer value to append.
 */
void
vcservice_log_append_uint64(vcservice_log* log, uint64_t val);

/**
 * \brief Append a UUID value to the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The uuid value to append.
 */
void
vcservice_log_append_uuid(vcservice_log* log, const RCPR_SYM(rcpr_uuid)* val);

/**
 * \brief Set the log formatting mode to the default.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The sentry value for this operation.
 */
void
vcservice_log_format_set_default(
    vcservice_log* log, const vcservice_log_format_default* val);

/**
 * \brief Set the log formatting mode to hex.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The sentry value for this operation.
 */
void
vcservice_log_format_set_hex(
    vcservice_log* log, const vcservice_log_format_hex* val);

/**
 * \brief Handle a no-operation, needed for type resolution in the debug macro.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 * \param val           The dummy value for this operation.
 */
void
vcservice_log_no_op(
    vcservice_log* log, const void* val);

/**
 * \brief Commit the logging message.
 *
 * \param log           The \ref vcservice_log instance for this operation.
 */
void
vcservice_log_message_commit(vcservice_log* log);

/******************************************************************************/
/* Start of accessors.                                                        */
/******************************************************************************/

/**
 * \brief Given a \ref vcservice_log instance, return its resource handle.
 *
 * \param log           The \ref vcservice_log instance from which the resource
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
 * \param log           The \ref vcservice_log instance from which the resource
 *                      handle is returned.
 *
 * \returns the logging threshold for this \ref vcservice_log instance.
 */
unsigned int
vcservice_log_threshold_level(const vcservice_log* log);

/******************************************************************************/
/* Start of utility macros.                                                   */
/******************************************************************************/

/* this section won't work in C++. */
#if !defined(__cplusplus)

/**
 * \brief Log a debug message to the given logger.
 *
 * \param log           The logger for this operation.
 * \param ...           A comma separated list of values to append to this log
 *                      message.
 */
#define DEBUG_LOG(log, ...) \
    LOG_WITH_LEVEL(log, VCSERVICE_LOGLEVEL_DEBUG, __VA_ARGS__)

/**
 * \brief Log a verbose message to the given logger.
 *
 * \param log           The logger for this operation.
 * \param ...           A comma separated list of values to append to this log
 *                      message.
 */
#define VERBOSE_LOG(log, ...) \
    LOG_WITH_LEVEL(log, VCSERVICE_LOGLEVEL_VERBOSE, __VA_ARGS__)

/**
 * \brief Log an info message to the given logger.
 *
 * \param log           The logger for this operation.
 * \param ...           A comma separated list of values to append to this log
 *                      message.
 */
#define INFO_LOG(log, ...) \
    LOG_WITH_LEVEL(log, VCSERVICE_LOGLEVEL_INFO, __VA_ARGS__)

/**
 * \brief Log a normal message to the given logger.
 *
 * \param log           The logger for this operation.
 * \param ...           A comma separated list of values to append to this log
 *                      message.
 */
#define NORMAL_LOG(log, ...) \
    LOG_WITH_LEVEL(log, VCSERVICE_LOGLEVEL_NORMAL, __VA_ARGS__)

/**
 * \brief Log an error message to the given logger.
 *
 * \param log           The logger for this operation.
 * \param ...           A comma separated list of values to append to this log
 *                      message.
 */
#define ERROR_LOG(log, ...) \
    LOG_WITH_LEVEL(log, VCSERVICE_LOGLEVEL_ERROR, __VA_ARGS__)

/**
 * \brief Log a critical message to the given logger.
 *
 * \param log           The logger for this operation.
 * \param ...           A comma separated list of values to append to this log
 *                      message.
 */
#define CRITICAL_LOG(log, ...) \
    LOG_WITH_LEVEL(log, VCSERVICE_LOGLEVEL_CRITICAL, __VA_ARGS__)

/**
 * \brief Format the given value as hexadecimal if that is supported for this
 * type.
 *
 * \param arg           The argument to format as hex.
 *
 * \note This macro must be used as an argument to a log macro.
 */
#define LOG_HEX(arg) \
    &vcservice_log_format_hex_sentry, (arg), \
    &vcservice_log_format_default_sentry

#define LOG_WITH_LEVEL(log, level, ...) \
    do { \
    if (vcservice_log_threshold_level(log) >= (level)) { \
        vcservice_log_message_start(log); \
        vcservice_log_append_log_level(log, (level)); \
        VCSERVICE_LOG01(log, __VA_ARGS__, \
            VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, \
            VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, \
            VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, \
            VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, \
            VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, \
            VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, VCLEOM, \
            VCLEOM, VCLEOM, VCLEOM) \
        vcservice_log_message_commit(log); \
    } } while (0)

#define VCSERVICE_LOG_END_OF_INPUT(arg) \
    _Generic((arg), \
        vcservice_log_end_of_message*: true, \
        const vcservice_log_end_of_message*: true, \
        default: false)

#define VCSERVICE_LOG_ITEM(log, arg) \
    _Generic((arg), \
        int8_t: vcservice_log_append_int8, \
        uint8_t: vcservice_log_append_uint8, \
        int16_t: vcservice_log_append_int16, \
        uint16_t: vcservice_log_append_uint16, \
        int32_t: vcservice_log_append_int32, \
        uint32_t: vcservice_log_append_uint32, \
        int64_t: vcservice_log_append_int64, \
        uint64_t: vcservice_log_append_uint64, \
        const char*: vcservice_log_append_string, \
        char*: vcservice_log_append_string, \
        const RCPR_SYM(rcpr_uuid)*: vcservice_log_append_uuid, \
        RCPR_SYM(rcpr_uuid)*: vcservice_log_append_uuid, \
        vcservice_log_format_default*: vcservice_log_format_set_default, \
        const vcservice_log_format_default*: \
            vcservice_log_format_set_default, \
        vcservice_log_format_hex*: vcservice_log_format_set_hex, \
        const vcservice_log_format_hex*: \
            vcservice_log_format_set_hex, \
        vcservice_log_end_of_message*: vcservice_log_no_op, \
        const vcservice_log_end_of_message*: vcservice_log_no_op \
        )((log), (arg))

#define VCSERVICE_LOG01(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG02(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG02(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG03(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG03(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG04(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG04(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG05(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG05(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG06(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG06(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG07(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG07(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG08(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG08(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG09(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG09(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG10(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG10(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG11(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG11(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG12(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG12(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG13(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG13(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG14(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG14(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG15(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG15(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG16(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG16(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG17(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG17(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG18(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG18(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG19(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG19(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG20(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG20(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG21(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG21(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG22(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG22(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG23(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG23(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG24(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG24(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG25(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG25(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG26(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG26(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG27(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG27(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG28(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG28(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG29(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG29(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG30(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG30(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG31(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG31(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG32(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG32(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG33(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG33(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG34(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG34(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG35(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG35(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG36(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG36(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG37(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG37(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG38(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG38(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG39(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG39(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG40(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG40(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG41(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG41(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG42(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG42(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG43(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG43(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG44(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG44(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG45(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG45(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG46(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG46(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG47(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG47(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG48(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG48(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG49(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG49(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG50(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG50(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        VCSERVICE_LOG_ITEM(log, arg); \
        VCSERVICE_LOG51(log, __VA_ARGS__); \
    }

#define VCSERVICE_LOG51(log, arg, ...) \
    if (!VCSERVICE_LOG_END_OF_INPUT(arg)) { \
        vcservice_log_format_set_default( \
            log, &vcservice_log_format_default_sentry); \
        VCSERVICE_LOG_ITEM(log, " (truncated)"); \
    }

#define VCLEOM &vcservice_log_end_of_message_sentry

#endif /* !defined(__cplusplus) */

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  /*__cplusplus*/
