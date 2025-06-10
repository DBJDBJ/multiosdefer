// defer_gcc.h - GCC defer implementation using nested functions
#pragma once
#ifndef DEFER_GCC_H_
#define DEFER_GCC_H_

#include <stdlib.h>
#include <stdio.h>

// GCC defer using auto nested functions + cleanup attribute
#define DEFER(F, V)              \
    auto void F(int *);          \
    [[gnu::cleanup(F)]] int V;   \
    auto void F(int *)

// Helper macros for unique names
#define __DEFER_(N) DEFER(__DEFER_FUNCTION_##N, __DEFER_VARIABLE_##N)
#define _DEFER(N) __DEFER_(N)

// The main defer macro - uses __COUNTER__ for unique names
#define defer _DEFER(__COUNTER__)

// Example usage
#ifdef DEFER_TESTING

#include <pthread.h>

pthread_mutex_t test_mutex = PTHREAD_MUTEX_INITIALIZER;

void test_defer_gcc() {
    printf("=== GCC Defer Test ===\n");
    
    // Memory allocation with defer cleanup
    void *ptr = malloc(1024);
    defer { 
        printf("GCC: Freeing memory\n");
        free(ptr); 
    };
    
    // File handling with defer cleanup
    FILE *f = fopen("test_gcc.txt", "w");
    defer { 
        if (f) {
            printf("GCC: Closing file\n");
            fclose(f); 
        }
    };
    
    // Mutex with defer cleanup
    pthread_mutex_lock(&test_mutex);
    defer { 
        printf("GCC: Unlocking mutex\n");
        pthread_mutex_unlock(&test_mutex); 
    };
    
    if (!ptr || !f) {
        printf("GCC: Resource allocation failed - but cleanup will happen\n");
        return;
    }
    
    fprintf(f, "Hello from GCC defer!");
    printf("GCC: Work completed\n");
    
    // All defers execute in reverse order automatically
}

#endif // DEFER_TESTING

#endif // DEFER_GCC_H_