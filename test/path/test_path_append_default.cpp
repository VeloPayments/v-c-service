/**
 * \file path/test_path_append_default.cpp
 *
 * Test the path_append_default method.
 *
 * \copyright 2018-2022 Velo-Payments, Inc.  All rights reserved.
 */

#include <minunit/minunit.h>
#include <paths.h>
#include <cstring>
#include <string>
#include <vcservice/error_codes.h>
#include <vcservice/path.h>

using namespace std;

RCPR_IMPORT_allocator_as(rcpr);
RCPR_IMPORT_resource;

TEST_SUITE(path_append_default);

/**
 * \brief Verify that path_append_default checks its parameters.
 */
TEST(parameter_checks)
{
    rcpr_allocator* alloc = nullptr;
    const char* path = "";
    char* outpath = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* passing a null pointer to path_append_default causes it to fail. */
    TEST_EXPECT(
        VCSERVICE_ERROR_PATH_INVALID_PARAMETER ==
            path_append_default(nullptr, alloc, path));
    TEST_EXPECT(
        VCSERVICE_ERROR_PATH_INVALID_PARAMETER ==
            path_append_default(&outpath, nullptr, path));
    TEST_EXPECT(
        VCSERVICE_ERROR_PATH_INVALID_PARAMETER ==
            path_append_default(&outpath, alloc, nullptr));

    /* clean up. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief When an empty path is passed to this function, it sets output as the
 * default path.
 */
TEST(empty_string)
{
    rcpr_allocator* alloc = nullptr;
    char* outpath = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: outpath is not set. */
    TEST_ASSERT(nullptr == outpath);

    /* The function should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == path_append_default(&outpath, alloc, ""));

    /* POSTCONDITION: outpath is set. */
    TEST_ASSERT(nullptr != outpath);

    /* the path should be equal to the default path. */
    TEST_EXPECT(0 == strcmp(_PATH_DEFPATH, outpath));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, outpath));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief Append the default path onto an arbitrary path.
 */
TEST(arbitrary_path_1)
{
    rcpr_allocator* alloc = nullptr;
    string arbitrary_path = string("baz") + string(":") + _PATH_DEFPATH;
    char* outpath = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: outpath is not set. */
    TEST_ASSERT(nullptr == outpath);

    /* The function should succeed. */
    TEST_ASSERT(STATUS_SUCCESS == path_append_default(&outpath, alloc, "baz"));

    /* POSTCONDITION: outpath is set. */
    TEST_ASSERT(nullptr != outpath);

    /* the path should be equal to the default path. */
    TEST_EXPECT(0 == strcmp(arbitrary_path.c_str(), outpath));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, outpath));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}

/**
 * \brief Append the default path onto an arbitrary path.
 */
TEST(arbitrary_path_2)
{
    rcpr_allocator* alloc = nullptr;
    string begin_path("/bin:/usr/bin:/home/foo/bin");
    string arbitrary_path =
        string("/bin:/usr/bin:/home/foo/bin") + string(":") + _PATH_DEFPATH;
    char* outpath = nullptr;

    /* we should be able to create a malloc allocator. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_malloc_allocator_create(&alloc));

    /* PRECONDITION: outpath is not set. */
    TEST_ASSERT(nullptr == outpath);

    /* The function should succeed. */
    TEST_ASSERT(
        STATUS_SUCCESS
            == path_append_default(&outpath, alloc, begin_path.c_str()));

    /* POSTCONDITION: outpath is set. */
    TEST_ASSERT(nullptr != outpath);

    /* the path should be equal to the default path. */
    TEST_EXPECT(0 == strcmp(arbitrary_path.c_str(), outpath));

    /* clean up. */
    TEST_ASSERT(STATUS_SUCCESS == rcpr_allocator_reclaim(alloc, outpath));
    TEST_ASSERT(
        STATUS_SUCCESS
            == resource_release(rcpr_allocator_resource_handle(alloc)));
}
