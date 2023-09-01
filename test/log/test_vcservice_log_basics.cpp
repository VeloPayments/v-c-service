/**
 * \file log/test_vcservice_log_basics.cpp
 *
 * Basic unit tests for a \ref vcservice_log instance.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <string.h>

#include "../../src/log/log_internal.h"

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

TEST_SUITE(test_vcservice_log_create_from_psock);

/**
 * \brief We can start a log message, and it will clear the buffer and reset the
 * buffer index.
 */
TEST(log_message_start)
{
    rcpr_allocator* alloc;
    psock* sock;
    vcservice_log* log;

    /* create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* create a buffer backed psock. */
    TEST_ASSERT(
        STATUS_SUCCESS == psock_create_from_buffer(&sock, alloc, NULL, 0));

    /* create a logger instance. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == vcservice_log_create_from_psock(
                    &log, alloc, sock, VCSERVICE_LOGLEVEL_INFO));

    /* write junk to the message buffer. */
    memset(log->log_message, 0xFF, sizeof(log->log_message));

    /* set the index to a dummy value. */
    log->log_idx = 97;

    /* start a message. */
    vcservice_log_message_start(log);

    printf("%lu\n", log->log_idx);

    /* The index is updated past the date. */
    TEST_EXPECT(20 == log->log_idx);

    /* past the index and to the end of the buffer, the buffer is nulled. */
    for (size_t i = log->log_idx; i < MAX_LOG_MESSAGE_SIZE; ++i)
    {
        TEST_EXPECT(0 == log->log_message[i]);
    }

    /* verify that the logging threshold level is correct. */
    TEST_EXPECT(VCSERVICE_LOGLEVEL_INFO == vcservice_log_threshold_level(log));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcservice_log_resource_handle(log)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}
