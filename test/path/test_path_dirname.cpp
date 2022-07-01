/**
 * \file path/test_path_dirname.cpp
 *
 * Test the path_dirname method.
 *
 * \copyright 2018-2022 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <cstring>
#include <string>
#include <vcservice/error_codes.h>
#include <vcservice/path.h>

using namespace std;

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_resource;

TEST_SUITE(path_append_default);

/**
 * \brief Verify that path_dirname checks its parameters.
 */
TEST(parameter_checks)
{
    rcpr_allocator* alloc = nullptr;
    const char* filename = "";
    char* dirname = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* passing a null pointer to path_append_default causes it to fail. */
    TEST_EXPECT(
        VCSERVICE_ERROR_PATH_INVALID_PARAMETER ==
            path_dirname(nullptr, alloc, filename));
    TEST_EXPECT(
        VCSERVICE_ERROR_PATH_INVALID_PARAMETER ==
            path_dirname(&dirname, nullptr, filename));
    TEST_EXPECT(
        VCSERVICE_ERROR_PATH_INVALID_PARAMETER ==
            path_dirname(&dirname, alloc, nullptr));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief When an empty string is encountered, return "." te represent the
 * current directory.
 */
TEST(empty_string)
{
    rcpr_allocator* alloc = nullptr;
    char* dirname = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: dirname is NULL. */
    TEST_ASSERT(nullptr == dirname);

    /* calling path_dirname should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == path_dirname(&dirname, alloc, ""));

    /* POSTCONDITION: dirname is NOT NULL. */
    TEST_ASSERT(nullptr != dirname);

    /* the dirname string matches the local directory symbolic name. */
    TEST_EXPECT(0 == strcmp(".", dirname));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, dirname));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief When a simple filename is encountered, the directory is the current
 * directory.
 */
TEST(simple_filename)
{
    rcpr_allocator* alloc = nullptr;
    char* dirname = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: dirname is NULL. */
    TEST_ASSERT(nullptr == dirname);

    /* calling path_dirname should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == path_dirname(&dirname, alloc, "foo.txt"));

    /* POSTCONDITION: dirname is NOT NULL. */
    TEST_ASSERT(nullptr != dirname);

    /* the dirname string matches the local directory symbolic name. */
    TEST_EXPECT(0 == strcmp(".", dirname));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, dirname));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief A filename with a single subdirectory is shortened to that
 * subdirectory.
 */
TEST(single_subdirectory)
{
    rcpr_allocator* alloc = nullptr;
    char* dirname = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: dirname is NULL. */
    TEST_ASSERT(nullptr == dirname);

    /* calling path_dirname should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS == path_dirname(&dirname, alloc, "build/foo.txt"));

    /* POSTCONDITION: dirname is NOT NULL. */
    TEST_ASSERT(nullptr != dirname);

    /* the dirname string matches the local directory symbolic name. */
    TEST_EXPECT(0 == strcmp("build", dirname));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, dirname));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief A filename with multiple subdirectories is properly handled.
 */
TEST(multiple_subdirectories)
{
    rcpr_allocator* alloc = nullptr;
    char* dirname = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: dirname is NULL. */
    TEST_ASSERT(nullptr == dirname);

    /* calling path_dirname should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == path_dirname(
                    &dirname, alloc, "build/host/checked/src/path/to/foo.txt"));

    /* POSTCONDITION: dirname is NOT NULL. */
    TEST_ASSERT(nullptr != dirname);

    /* the dirname string matches the local directory symbolic name. */
    TEST_EXPECT(0 == strcmp("build/host/checked/src/path/to", dirname));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, dirname));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief An absolute directory is properly extracted.
 */
TEST(multiple_subdirectories_absolute)
{
    rcpr_allocator* alloc = nullptr;
    char* dirname = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: dirname is NULL. */
    TEST_ASSERT(nullptr == dirname);

    /* calling path_dirname should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == path_dirname(
                    &dirname, alloc,
                    "/build/host/checked/src/path/to/foo.txt"));

    /* POSTCONDITION: dirname is NOT NULL. */
    TEST_ASSERT(nullptr != dirname);

    /* the dirname string matches the local directory symbolic name. */
    TEST_EXPECT(0 == strcmp("/build/host/checked/src/path/to", dirname));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, dirname));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief A file relative to the current directory is properly handled.
 */
TEST(explicit_curdir)
{
    rcpr_allocator* alloc = nullptr;
    char* dirname = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: dirname is NULL. */
    TEST_ASSERT(nullptr == dirname);

    /* calling path_dirname should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == path_dirname(
                    &dirname, alloc,
                    "./foo.txt"));

    /* POSTCONDITION: dirname is NOT NULL. */
    TEST_ASSERT(nullptr != dirname);

    /* the dirname string matches the local directory symbolic name. */
    TEST_EXPECT(0 == strcmp(".", dirname));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, dirname));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}
