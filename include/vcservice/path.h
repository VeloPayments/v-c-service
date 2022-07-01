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
 * the caller and must be reclaimed using the allocator when no longer needed.
 *
 * \param outpath           Pointer to receive the combined path.
 * \param alloc             The allocator to use for this operation.
 * \param path              The path to append.
 *
 * \returns a status code indicating success or failure.
 *          - STATUS_SUCCESS on success.
 *          - VCSERVICE_ERROR_GENERAL_OUT_OF_MEMORY if the operation cannot
 *            be completed due to a memory allocation error.
 *          - a non-zero error code on failure.
 */
status path_append_default(
    char** outpath, RCPR_SYM(allocator)* alloc, const char* path);

/**
 * \brief Given a pathname, return the directory portion of this pathname.
 *
 * On success, dirname is updated to the directory portion of the filename.
 * This value is owned by the caller and must be reclaimed using the allocator
 * when no longer needed.
 *
 * \param dirname           Pointer to receive the directory name on success.
 * \param alloc             The allocator to use for this operation.
 * \param filename          The filename from which the directory name is
 *                          computed.
 *
 * \returns a status code indicating success or failure.
 *      - STATUS_SUCCESS on success.
 *      - a non-zero error code on failure.
 */
status path_dirname(
    char** dirname, RCPR_SYM(allocator)* alloc, const char* filename);

/* make this header C++ friendly. */
#ifdef __cplusplus
}
#endif  /*__cplusplus*/
