/**
 * \file path/path_append_default.c
 *
 * \brief Append the default path onto the given path.
 *
 * \copyright 2018-2022 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <paths.h>
#include <rcpr/string.h>
#include <vcservice/error_codes.h>
#include <vcservice/path.h>

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_string_as(rcpr);

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
    char** outpath, RCPR_SYM(allocator)* alloc, const char* path)
{
    /* parameter sanity checks. */
    MODEL_ASSERT(NULL != outpath);
    MODEL_ASSERT(rcpr_prop_allocator_valid(alloc));
    MODEL_ASSERT(NULL != path);

    /* runtime parameter checks. */
    if (NULL == outpath || NULL == alloc || NULL == path)
    {
        return VCSERVICE_ERROR_PATH_INVALID_PARAMETER;
    }

    /* if the path is empty, return a copy of the default path. */
    if (0 == path[0])
    {
        return
            rcpr_strdup(outpath, alloc, _PATH_DEFPATH);
    }
    else
    {
        return
            rcpr_strcatv(outpath, alloc, path, ":", _PATH_DEFPATH, NULL);
    }
}
