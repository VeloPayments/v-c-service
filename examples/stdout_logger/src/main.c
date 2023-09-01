/**
 * \file examples/stdout_logger/main.c
 *
 * \brief Main entry point for the stdout_logger example.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <unistd.h>
#include <vcservice/log.h>

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

/**
 * \brief Main entry point for the stdout_logger example.
 *
 * \param argc          The argument count.
 *\ param argv          The argument vector.
 */
int main(int argc, char* argv[])
{
    status retval, release_retval;
    bool error = false;
    rcpr_allocator* alloc;
    vcservice_log* log;
    unsigned int threshold_level = VCSERVICE_LOGLEVEL_DEBUG;

    (void)argc;
    (void)argv;

    /* create a malloc allocator. */
    retval = rcpr_malloc_allocator_create(&alloc);
    if (STATUS_SUCCESS != retval)
    {
        error = true;
        goto done;
    }

    /* create the logger. */
    retval =
        vcservice_log_create_using_standard_output(
            &log, alloc, threshold_level);
    if (STATUS_SUCCESS != retval)
    {
        error = true;
        goto cleanup_alloc;
    }

    INFO_LOG(
        log, "The threshold log level has been set to ", threshold_level, ".");
    VERBOSE_LOG(log, "This is an example verbose log.");
    ERROR_LOG(log, "This is an example error log.");
    CRITICAL_LOG(log, "This is an example critical log.");
    DEBUG_LOG(log, "This is an example debug log.");
    NORMAL_LOG(log, "This is an example normal log.");

    goto cleanup_log;

cleanup_log:
    release_retval = resource_release(vcservice_log_resource_handle(log));
    if (STATUS_SUCCESS != release_retval)
    {
        error = 1;
        retval = release_retval;
    }

cleanup_alloc:
    release_retval = resource_release(rcpr_allocator_resource_handle(alloc));
    if (STATUS_SUCCESS != release_retval)
    {
        error = true;
        retval = release_retval;
    }

done:
    if (error)
        return 1;
    else
        return 0;
}
