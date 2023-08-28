/**
 * \file log/log_internal.h
 *
 * \brief Internal header for log library.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/resource/protected.h>

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
