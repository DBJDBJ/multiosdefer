#ifndef DEFER_MSCV_WITH_DEFER_H
#pragma once
#define DEFER_MSCV_WITH_DEFER_H

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

// Usage:
void with_defer_test()
{
    printf("=== MSVC WITH_DEFER Test Begin ===\n");
    FILE *f = fopen("test_with_defer.txt", "w+");

    WITH_DEFER(
        {
            if (f) fclose(f);
            printf("=== MSVC WITH_DEFER Test End ===\n");
        } // cleanup
        ,
        { // main code
            if (!f)
                return;
                if ( 1 > fwrite("Hello, MSVC WITH_DEFER!\n", 1, 27, f))
                {
                    perror("Failed to write to file");
                    return;
                }
            char buf[256] = {0};
            fgets(buf, sizeof(buf), f);
            printf("Read: %s", buf);
        });
}

#endif // DEFER_MSCV_WITH_DEFER_H