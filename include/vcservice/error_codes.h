/**
 * \file vcservice/error_codes.h
 *
 * \brief Error codes for vcservice.
 *
 * \copyright 2022 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/**
 * \brief An out-of-memory condition was encountered during an operation.
 */
#define VCSERVICE_ERROR_GENERAL_OUT_OF_MEMORY 0x6100

/**
 * \brief An invalid parameter was passed to a function.
 */
#define VCSERVICE_ERROR_PATH_INVALID_PARAMETER 0x6101

/**
 * \brief The logging interface could not dup standard output.
 */
#define VCSERVICE_ERROR_LOG_STDOUT_DUP 0x6102

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif /*__cplusplus*/
