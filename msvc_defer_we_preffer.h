#ifndef DEFER_MSCV_WITH_DEFER_H
#pragma once
#define DEFER_MSCV_WITH_DEFER_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

// abandoned
// #define WITH_DEFER(cleanup_code, main_code) \
//     __try                                   \
//     {                                       \
//         main_code                           \
//     }                                       \
//     __finally                               \
//     {                                       \
//         cleanup_code;                       \
//     }

// if you must use this
// I preffer "naked" __try / __finally
#define MSVC_DEFER(main_block, cleanup_block) \
    __try                                     \
    main_block __finally cleanup_block

// Usage:
void msvc_defer_we_preffer()
{
    printf("=== MSVC WITH_DEFER Test Begin ===\n");

    // very simple macro
    MSVC_DEFER(
        { printf("MSVC_DEFER macro main block\n"); },
        { printf("MSVC_DEFER macrofinal block\n"); });

    char *global_pointer = NULL;

    // I preffer no macros if not necessary
    printf("=== MSVC naked __try / __finally Test ===\n");
    __try
    { // main code
        global_pointer = calloc(sizeof(char), 0xFFFF);
        printf("Allocated 0xFFFF\n");
    }
    __finally
    {
        if (global_pointer)
            free(global_pointer);
        global_pointer = NULL;
        printf("Freed 0xFFFF\n");
    } // cleanup
    printf("=== MSVC WITH_DEFER Test End ===\n");
}

#endif // DEFER_MSCV_WITH_DEFER_H