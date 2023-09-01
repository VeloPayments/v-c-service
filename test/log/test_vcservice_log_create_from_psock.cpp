/**
 * \file log/test_vcservice_log_create_from_psock.cpp
 *
 * Test the vcservice_log_create_from_psock method.
 *
 * \copyright 2023 Velo Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <vcservice/log.h>

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_psock;
RCPR_IMPORT_resource;

TEST_SUITE(test_vcservice_log_create_from_psock);

/**
 * \brief Verify that we can create a logger instance and check its logging
 * threshold level.
 */
TEST(basics)
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

    /* verify that the logging threshold level is correct. */
    TEST_EXPECT(VCSERVICE_LOGLEVEL_INFO == vcservice_log_threshold_level(log));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS == resource_release(vcservice_log_resource_handle(log)));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}
