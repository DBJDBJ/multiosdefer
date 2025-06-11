#ifndef DEFER_MSCV_WITH_DEFER_H
#pragma once
#define DEFER_MSCV_WITH_DEFER_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#define WITH_DEFER(cleanup_code, main_code) \
    __try                                   \
    {                                       \
        main_code                           \
    }                                       \
    __finally                               \
    {                                       \
        cleanup_code;                       \
    }

#define DBJ_DEFER(main_block, cleanup_block) \
    __try                                    \
    main_block __finally cleanup_block

// Usage:
void with_defer_test()
{
    DBJ_DEFER(
        { printf("DBJ_DEFER main block\n"); },
        { printf("DBJ_DEFER final block\n"); });

    printf("=== MSVC WITH_DEFER Test Begin ===\n");
    FILE *f = NULL;

    __try
    { // main code
        if (fopen_s(&f, "test_with_defer.txt", "w") != 0 || !f)
            return;
        if (1 > fwrite("Hello, MSVC WITH_DEFER!\n", 1, 27, f))
        {
            perror("Failed to write to file");
            return;
        }
        fclose(f);
        if (fopen_s(&f, "test_with_defer.txt", "r") != 0 || !f)
            return;
        char buf[256] = {0};
        fgets(buf, sizeof(buf), f);
        printf("Read: %s", buf);
    }
    __finally
    {
        if (f)
            fclose(f);
        printf("=== MSVC WITH_DEFER Test End ===\n");
    } // cleanup
}

#endif // DEFER_MSCV_WITH_DEFER_H