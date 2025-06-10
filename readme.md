> **IMPORTANT!**
> `.vscode` stuff is included, as it is not usualy the case in github repos, because it depends on the authors machine setup. 
> 
> My setup is:
> 
> MINGW MSYS is instaled in `D:\msys64` , and it is important to use the "new tool chain"  from: `D:\msys64\ucrt64\bin`
> 
> VStudio root is `D:/Program Files` 
>
> I am sure fully you can amend stuff in `.vscode`  to match your setup
> 
> &nbsp;
> 

# Why?

You want to build C console program on Windows10 (2025Q3), and lo-and-behold you want to use that `defer` thingy implemented as a macro (of course).

You start copy pasting and you use Claude, ChatGPT or whatever else you fancy.

You naturally reach for VStudio Clang combination.  

> After several hours or days more likely, you realise, to implement that simple but uncomprehensible gcc `defer` macro in clang is very far from simple. (`defer` as presented on Jens Gustedt blog)
> And along that path of pain, your trusty AI assistant pushed you down several rabbit holes.

After day of search, finally you hit the local blocks implementation header for clang, needed if you want `-fblocks` to work. Now you have it compile and run inside VStudio. (yes there were a lot of warnings but you silenced them all)

But. VStudio debugger does not want to break inside defer blocks? `printf` from the same does not print anything. 

In desperation you install mingw/gcc for win. Of course that "simple" `defer` macro works. 

But. You are building non-trivial windows exe, using windows.h , jpeg lib, etc. With VStidio MSVC is easy to make it work. With clang too. And using defer? You are just not sure anymore.

Then you remember win32 "Structured Exception Handling" (SEH). Using SEH and MSVC you can have standard win32 C tool chain, and you can simulate the defer. Not exactly `ISO C2y` (next standard after C23) but nevertheles the same concept. And its simplicity makes you feel stupid, for not remembering it all these days ago.

This is why I have isolated them both apporaches in this simple repo. I hope you will find it on time.

# Defer Macros for C for MSVC and for GCC

Cross-compiler defer implementation for C using GCC nested functions and MSVC structured exception handling.

## Files Structure

```
defer-project/
├── defer_gcc.h         # GCC implementation using auto functions
├── defer_msvc.h        # MSVC implementation using __try/__finally  
├── main.c              # Test program with compiler detection
├── .vscode/
│   ├── tasks.json      # Build tasks for both compilers
│   ├── launch.json     # Debug configurations
│   └── c_cpp_properties.json # IntelliSense settings
└── README.md
```

## Usage

### GCC Version (defer_gcc.h)
```c
#include "defer_gcc.h"

void example() {
    void *ptr = malloc(100);
    defer { free(ptr); };
    
    FILE *f = fopen("test.txt", "w");
    defer { if(f) fclose(f); };
    
    // Early return? Cleanup still happens!
    if (error) return;
}
```

### MSVC Version (defer_msvc.h)
```c
#include "defer_msvc.h"

void example() {
    void *ptr = malloc(100);
    FILE *f = fopen("test.txt", "w");
    
    defer {
        // Main code here
        if (!ptr || !f) break;
        
        // Do work...
        
    } end_defer(
        // Cleanup code - always runs
        if (ptr) free(ptr);
        if (f) fclose(f);
    );
}
```

## Building in VS Code

### Prerequisites
- **For GCC**: MinGW-w64 or similar GCC distribution
- **For MSVC**: Visual Studio Build Tools or Visual Studio

### Quick Start
1. Open folder in VS Code
2. Press `Ctrl+Shift+P` → "Tasks: Run Task"
3. Choose either:
   - "Build with GCC" or "Run GCC Build"
   - "Build with MSVC" or "Run MSVC Build"

### Manual Build Commands

**GCC:**
```bash
gcc -o defer_test_gcc.exe main.c -std=c11 -Wall -Wextra -lpthread
./defer_test_gcc.exe
```

**MSVC:**
```cmd
cl /Fe:defer_test_msvc.exe main.c /std:c11 /W4
defer_test_msvc.exe
```

## How It Works

### GCC Implementation
- Uses GCC's `auto` nested functions
- Leverages `[[gnu::cleanup(...)]]` attribute
- Cleanup functions execute when variables go out of scope
- Works with early returns, breaks, continues, exceptions

### MSVC Implementation  
- Uses Microsoft's `__try` / `__finally` structured exception handling
- `__finally` block always executes regardless of how `__try` block exits
- Works with early returns, exceptions, and structured exits

## Features

- ✅ **Cross-compiler**: Automatically detects GCC vs MSVC
- ✅ **Always executes**: Cleanup happens on any exit path
- ✅ **Multiple defers**: Can have many per function (GCC) or nested blocks (MSVC)
- ✅ **Zero runtime overhead**: All resolved at compile time
- ✅ **Exception safe**: Works with structured exceptions and setjmp/longjmp

## Debugging

Both configurations include debug builds:
- Press `F5` to debug
- Choose "Debug with GCC" or "Debug with MSVC"
- Breakpoints work normally in both cleanup and main code

## Notes

- **GCC**: Requires GCC extensions (nested functions, cleanup attribute)
- **MSVC**: Requires Windows and structured exception handling
- **Portability**: Each header is compiler-specific but usage is similar
- **Thread Safety**: Both implementations are thread-safe for the defer mechanism itself

# Summary

Now you have a complete cross-compiler defer setup that actually works:

-  **GCC (UCRT64)**: Clean Go-style `defer { cleanup; };` syntax
-  **MSVC**: Block-style `defer { code } end_defer(cleanup);` syntax  
-  **VS Code integration**: Build/run/debug tasks for both compilers
-  **Automatic detection**: Single `main.c` picks the right implementation
-  **Proper paths**: Using your actual D: drive MSYS2/VS installations

You can now:
- Switch between compilers with different build tasks
- Debug with `F5` 
- Get proper IntelliSense for both
- Use defer for guaranteed cleanup in any C project

The defer macros will handle resource cleanup automatically on any exit path - early returns, exceptions, breaks, continues, etc. Perfect for RAII-style programming in C!

Enjoy your bulletproof resource management! 🚀