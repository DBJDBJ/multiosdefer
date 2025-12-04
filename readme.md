# DEFER 

1. Macro for GCC 13, 14 uses Jens Gusted macro
   1. How far into the past versions this works I did not checked
   1. see `defer_gcc.h` for simpple usage
   2. 2025Q4, I am not 100% sure if that will be the final official `defer`
   3. `Makefile` is for Linux cli only
   4. `defer` is being standardized as `Technical Specification TS 25755` for potential inclusion in `C2y` (the next C standard, likely 2029).
2. for MSVC (aka CL) macro uses Win32 Structured Exception Handling (SEH)
   1. see `msvc_defer_we_preffer.h`
   2. You need to be on win32 cli to use this
   3. The rest of win32 requirements code buidling on Win machine, I am sure you know.

## Summary

Basicaly not ideal. But simple and working defer. Making your C23 code safer. Right now. Even MSVC code.

Enjoy!

> dbj@dbj.org , CC BY SA 4.0