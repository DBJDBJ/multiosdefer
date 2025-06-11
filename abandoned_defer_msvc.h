// defer_msvc.h - MSVC defer implementation using __try/__finally
#pragma once
#ifndef DEFER_MSVC_H_
#define DEFER_MSVC_H_

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

//-------------------------------------------------------------------
// Abandoned
// Macros in here seem like unncessary complicatons
// It is much cleaner to use "naked" __try / __finally
//-------------------------------------------------------------------

// MSVC defer using __try/__finally blocks
#define defer \
    __try { \
        do

#define end_defer(cleanup_code) \
        while(0); \
    } \
    __finally { \
        cleanup_code; \
    }

// Alternative block-style defer
#define DEFER_BEGIN \
    __try {

#define DEFER_END(cleanup_code) \
    } \
    __finally { \
        cleanup_code; \
    }

// Scoped defer - creates a new scope with cleanup
#define SCOPED_DEFER(cleanup_code) \
    __try { \
        do {} while(0)

#define SCOPED_DEFER_END(cleanup_code) \
    } \
    __finally { \
        cleanup_code; \
    }

// Example usage
#ifdef DEFER_TESTING

CRITICAL_SECTION test_cs;
static int cs_initialized = 0;

void init_critical_section() {
    if (!cs_initialized) {
        InitializeCriticalSection(&test_cs);
        cs_initialized = 1;
    }
}

void test_defer_msvc() {
    printf("=== MSVC Defer Test ===\n");
    
    init_critical_section();
    
    // Method 1: Using defer/end_defer
    void *ptr = malloc(1024);
    FILE *f = fopen("test_msvc.txt", "w");
    
    defer {
        // Main code block
        if (!ptr || !f) {
            printf("MSVC: Resource allocation failed - but cleanup will happen\n");
            break;  // Exit the do-while, triggers finally
        }
        
        EnterCriticalSection(&test_cs);
        fprintf(f, "Hello from MSVC defer!");
        printf("MSVC: Work completed\n");
        
    } end_defer(
        // Cleanup code - always executes
        if (ptr) {
            printf("MSVC: Freeing memory\n");
            free(ptr);
        }
        if (f) {
            printf("MSVC: Closing file\n");
            fclose(f);
        }
        if (cs_initialized) {
            printf("MSVC: Leaving critical section\n");
            LeaveCriticalSection(&test_cs);
        }
    );
}

void test_defer_msvc_nested() {
    printf("=== MSVC Nested Defer Test ===\n");
    
    void *outer_ptr = malloc(512);
    
    DEFER_BEGIN
        printf("MSVC: Outer scope work\n");
        
        void *inner_ptr = malloc(256);
        DEFER_BEGIN
            printf("MSVC: Inner scope work\n");
            if (inner_ptr) {
                memset(inner_ptr, 0x42, 256);
            }
        DEFER_END(
            if (inner_ptr) {
                printf("MSVC: Freeing inner memory\n");
                free(inner_ptr);
            }
        )
        
    DEFER_END(
        if (outer_ptr) {
            printf("MSVC: Freeing outer memory\n");
            free(outer_ptr);
        }
    )
}

#endif // DEFER_TESTING

#endif // DEFER_MSVC_H_