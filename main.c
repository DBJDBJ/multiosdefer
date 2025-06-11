// main.c - Test both defer implementations
#define DEFER_TESTING

// Compiler detection
#if defined(_MSC_VER)
    #include "msvc_defer_we_preffer.h"
    #define COMPILER_NAME "MSVC"
#elif defined(__GNUC__)
    #include "defer_gcc.h"  
    #define COMPILER_NAME "GCC"
#else
    #error "Unsupported compiler - need MSVC or GCC"
#endif

int main(void) {
    printf("Testing defer with %s\n", COMPILER_NAME);
    printf("====================\n\n");
    
#if defined(_MSC_VER)
    msvc_defer_we_preffer();
    // printf("\n");
    // test_defer_msvc();
    // printf("\n");
    // test_defer_msvc_nested();
#elif defined(__GNUC__)
    test_defer_gcc();
#endif
    
    printf("\n%s defer test completed!\n", COMPILER_NAME);
    printf("====================\n\n");
    return 0;
}