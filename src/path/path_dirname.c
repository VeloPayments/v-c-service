/**
 * \file path/path_dirname.c
 *
 * \brief Given a pathname, return the directory portion of this pathname.
 *
 * \copyright 2018-2022 Velo Payments, Inc.  All rights reserved.
 */

#include <cbmc/model_assert.h>
#include <rcpr/string.h>
#include <string.h>
#include <vcservice/error_codes.h>
#include <vcservice/path.h>

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_string_as(rcpr);

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
    char** dirname, RCPR_SYM(allocator)* alloc, const char* filename)
{
    status retval, release_retval;
    char* workpath = NULL;
    char* dirpath = NULL;
    char* pathlast = NULL;
    const char* dirsep = NULL;

    /* parameter sanity checks. */
    MODEL_ASSERT(NULL != dirname);
    MODEL_ASSERT(rcpr_prop_allocator_valid(alloc));
    MODEL_ASSERT(NULL != filename);

    /* runtime parameter checks. */
    if (NULL == dirname || NULL == alloc || NULL == filename)
    {
        retval = VCSERVICE_ERROR_PATH_INVALID_PARAMETER;
        goto done;
    }

    /* if the filename is an empty string, then return the current directory. */
    if (0 == filename[0])
    {
        /* attempt to duplicate the local directory symbol. */
        retval = rcpr_strdup(dirname, alloc, ".");
        goto done;
    }
    else
    {
        /* copy the filename for the working path. */
        retval = rcpr_strdup(&workpath, alloc, filename);
        if (STATUS_SUCCESS != retval)
        {
            goto done;
        }

        /* the maximum size of the path is the size of the filename. */
        size_t pathmax = strlen(filename);
        size_t pathrem = pathmax;
        retval = rcpr_allocator_allocate(alloc, (void**)dirname, pathmax + 1);
        if (STATUS_SUCCESS != retval)
        {
            goto cleanup_workpath;
        }

        /* clear dirname. */
        (*dirname)[0] = 0;

        /* iterate through directories, copying these to the string. */
        for (
            /* initialize pathent with the first path entry. */
            char* pathent = strtok_r(workpath, "/", &pathlast);
            /* continue while there are more path entries. */
            NULL != pathent;
            /* get the next entry. */
            pathent = strtok_r(NULL, "/", &pathlast))
        {
            /* if dirpath exists, append it. */
            if (NULL != dirpath)
            {
                /* if dirsep exists, append it. */
                if (NULL != dirsep)
                {
                    strncat(*dirname, dirsep, pathrem);
                    pathrem -= strlen(dirsep);
                }
                else
                {
                    /* this is the first pass. If the filename is absolute, copy
                     * a slash to the beginning of dirname.
                     */
                    if ('/' == filename[0])
                    {
                        strncat(*dirname, "/", pathrem);
                        pathrem -= 1;
                    }

                    /* set the directory separator. */
                    dirsep = "/";
                }

                /* append dirpath. */
                strncat(*dirname, dirpath, pathrem);
                pathrem -= strlen(dirpath);
            }

            /* update dirpath. */
            dirpath = pathent;
        }

        /* if we only encountered one entry or fewer, set *dirname to "." */
        if (NULL == dirsep)
        {
            strncpy(*dirname, ".", pathrem);
        }

        /* firce dirname to be zero-terminated. */
        (*dirname)[pathmax] = 0;

        /* success. */
        retval = STATUS_SUCCESS;
    }

cleanup_workpath:
    if (NULL != workpath)
    {
        release_retval = rcpr_allocator_reclaim(alloc, workpath);
        if (STATUS_SUCCESS != release_retval)
        {
            retval = release_retval;
        }
    }

done:
    return retval;
}
