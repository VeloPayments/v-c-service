/**
 * \file log/log_internal.h
 *
 * \brief Internal header for log library.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/resource/protected.h>
#include <vcservice/log.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/

#define MAX_LOG_MESSAGE_SIZE 4096

#define LOG_BITS_FORMAT_MASK            0x0000FFFF
#define LOG_BITS_FORMAT_HEX             0x00000001
#define LOG_BITS_FORMAT_DEFAULT         0x00000000

/**
 * \brief The log instance.
 */
struct vcservice_log
{
    RCPR_SYM(resource) hdr;
    RCPR_SYM(allocator)* alloc;
    unsigned int threshold_level;
    RCPR_SYM(resource)* user_context;
    unsigned int log_level;
    char log_message[MAX_LOG_MESSAGE_SIZE];
    size_t log_idx;
    uint32_t log_bits;

    void (*log_write_cb)(
        vcservice_log* log, unsigned int log_level,
        RCPR_SYM(resource)* user_contex);
};

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
    RCPR_SYM(resource)* r);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  /*__cplusplus*/
