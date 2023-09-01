/**
 * \file examples/stdout_logger/main.c
 *
 * \brief Main entry point for the stdout_logger example.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <stdlib.h>
#include <unistd.h>
#include <vcservice/log.h>

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;
RCPR_IMPORT_uuid;

static unsigned int logger_threshold_level();

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
    const unsigned int threshold_level = logger_threshold_level();

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

    /* basic log level display. */
    INFO_LOG(
        log, "The threshold log level has been set to ", threshold_level, ".");
    INFO_LOG(
        log, "To override the threshold level,",
        " set VCSERVICE_LOG_THRESHOLD_LEVEL");
    VERBOSE_LOG(log, "This is an example verbose log.");
    ERROR_LOG(log, "This is an example error log.");
    CRITICAL_LOG(log, "This is an example critical log.");
    DEBUG_LOG(log, "This is an example debug log.");
    NORMAL_LOG(log, "This is an example normal log.");

    /* we can log hex values. */
    int myval = 0xDEADBEEF;
    uint8_t x = '\n';
    int y = 271;

    NORMAL_LOG(log, "myval = ", LOG_HEX(myval), ".");
    NORMAL_LOG(log, "x = ", LOG_HEX(x), ", y = ", y, ".");

    /* we can log UUID values. */
    rcpr_uuid id;
    retval =
        rcpr_uuid_parse_string(&id, "5b4bde6e-c7e5-4761-822f-59c489107c54");
    if (STATUS_SUCCESS != retval)
    {
        ERROR_LOG(log, "UUID parse failed.");
    }

    NORMAL_LOG(log, "id = ", &id, ".");

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

/**
 * \brief Get the logger threshold level from the environment.
 *
 * \returns the threshold level or a default.
 */
static unsigned int logger_threshold_level()
{
    const unsigned int default_threshold_level = VCSERVICE_LOGLEVEL_DEBUG;
    const char* threshold_level_string =
        getenv("VCSERVICE_LOG_THRESHOLD_LEVEL");

    if (NULL == threshold_level_string)
    {
        return default_threshold_level;
    }

    unsigned int threshold_level = (unsigned int)atoi(threshold_level_string);
    if (threshold_level > VCSERVICE_LOGLEVEL_DEBUG)
    {
        return default_threshold_level;
    }

    return threshold_level;
}
