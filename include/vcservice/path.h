/**
 * \file vcservice/path.h
 *
 * \brief Utility methods for resolving paths.
 *
 * \copyright 2018-2022 Velo Payments, Inc.  All rights reserved.
 */

#pragma once

#include <rcpr/allocator.h>

/* make this header C++ friendly. */
#ifdef __cplusplus
extern "C" {
#endif  /*__cplusplus*/

/**
 * \brief Append the default path onto a given path.
 *
 * On success, outpath is updated to the appended path.  This value is owned by
 * the caller and must be reclaimed using the allocatorwhen no longer needed.
 *
 * \param outpath           Pointer to receive the combined path.
 * \param path              The path to append.
 *
 * \returns a status code indicating success or failure.
 *          - STATUS_SUCCESS on success.
 *          - VCSERVICE_ERROR_GENERAL_OUT_OF_MEMORY if the operation cannot
 *            be completed due to a memory allocation error.
 */
status path_append_default(
    char** outpath, RCPR_SYM(allocator)* alloc, const char* path);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  /*__cplusplus*/
