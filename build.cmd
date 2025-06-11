@echo off
setlocal enabledelayedexpansion

:: Configuration - Adjust these paths for your system
set "MSYS64_ROOT=D:\msys64"
set "VS_ROOT=D:\Program Files\Microsoft Visual Studio\2022\Community"
set "GCC_PATH=%MSYS64_ROOT%\ucrt64\bin"
set "VCVARS_PATH=%VS_ROOT%\VC\Auxiliary\Build\vcvars64.bat"

:: Project settings
set "SOURCE=main.c"
set "GCC_OUTPUT=defer_test_gcc.exe"
set "MSVC_OUTPUT=defer_test_msvc.exe"

echo ============================================
echo    Defer C Project Build Script (%0)
echo ============================================
echo.

:: Parse command line arguments
if "%1"=="" goto :show_help
if "%1"=="help" goto :show_help
if "%1"=="-h" goto :show_help
if "%1"=="--help" goto :show_help

if "%1"=="gcc" goto :build_gcc
if "%1"=="msvc" goto :build_msvc
if "%1"=="both" goto :build_both
if "%1"=="clean" goto :clean_all
if "%1"=="run-gcc" goto :run_gcc
if "%1"=="run-msvc" goto :run_msvc
if "%1"=="debug-gcc" goto :debug_gcc
if "%1"=="debug-msvc" goto :debug_msvc

echo ERROR: Unknown command "%1"
echo.
goto :show_help

:show_help
echo Usage: %0 [command]
echo.
echo Commands:
echo   gcc           Build with GCC (debug mode)
echo   msvc          Build with MSVC (debug mode)
echo   both          Build with both compilers
echo   clean         Clean all build outputs
echo   run-gcc       Build and run GCC version
echo   run-msvc      Build and run MSVC version
echo   debug-gcc     Build GCC version optimized for debugging
echo   debug-msvc    Build MSVC version optimized for debugging
echo   help          Show this help
echo.
echo Examples:
echo   %0 gcc
echo   %0 run-msvc
echo   %0 clean
echo   %0 both
echo.
goto :end

:build_gcc
echo [GCC] Building %SOURCE% with GCC...
if not exist "%GCC_PATH%\gcc.exe" (
    echo ERROR: GCC not found at %GCC_PATH%\gcc.exe
    echo Please check MSYS64_ROOT path in this script
    goto :error
)

"%GCC_PATH%\gcc.exe" -g -O0 -o %GCC_OUTPUT% %SOURCE% -std=c11 -Wall -Wextra -lpthread
if errorlevel 1 (
    echo ERROR: GCC build failed
    goto :error
)
echo [GCC] Build successful: %GCC_OUTPUT%
goto :end

:build_msvc
echo [MSVC] Building %SOURCE% with MSVC...
if not exist "%VCVARS_PATH%" (
    echo ERROR: Visual Studio not found at "%VCVARS_PATH%"
    echo Please check VS_ROOT path in this script
    goto :error
)

call "%VCVARS_PATH%" >nul 2>&1
if errorlevel 1 (
    echo ERROR: Failed to initialize MSVC environment
    goto :error
)

cl /Zi /Od /Fe:%MSVC_OUTPUT% %SOURCE% /W4 /nologo
if errorlevel 1 (
    echo ERROR: MSVC build failed
    goto :error
)
echo [MSVC] Build successful: %MSVC_OUTPUT%
goto :end

:build_both
echo [BOTH] Building with both compilers...
echo.
call :build_gcc
if errorlevel 1 goto :error
echo.
call :build_msvc
if errorlevel 1 goto :error
echo.
echo [BOTH] All builds completed successfully!
goto :end

:clean_all
echo [CLEAN] Cleaning build outputs...
if exist %GCC_OUTPUT% (
    del %GCC_OUTPUT%
    echo Deleted: %GCC_OUTPUT%
)
if exist %MSVC_OUTPUT% (
    del %MSVC_OUTPUT%
    echo Deleted: %MSVC_OUTPUT%
)
if exist *.pdb (
    del *.pdb
    echo Deleted: *.pdb files
)
if exist *.obj (
    del *.obj
    echo Deleted: *.obj files
)
if exist *.ilk (
    del *.ilk
    echo Deleted: *.ilk files
)
echo [CLEAN] Cleanup completed
goto :end

:run_gcc
call :build_gcc
if errorlevel 1 goto :error
echo.
echo [RUN-GCC] Running %GCC_OUTPUT%...
echo ----------------------------------------
%GCC_OUTPUT%
echo ----------------------------------------
echo [RUN-GCC] Execution completed (exit code: %errorlevel%)
goto :end

:run_msvc
call :build_msvc
if errorlevel 1 goto :error
echo.
echo [RUN-MSVC] Running %MSVC_OUTPUT%...
echo ----------------------------------------
%MSVC_OUTPUT%
echo ----------------------------------------
echo [RUN-MSVC] Execution completed (exit code: %errorlevel%)
goto :end

:debug_gcc
echo [DEBUG-GCC] Building %SOURCE% with GCC (debug optimized)...
if not exist "%GCC_PATH%\gcc.exe" (
    echo ERROR: GCC not found at %GCC_PATH%\gcc.exe
    goto :error
)

"%GCC_PATH%\gcc.exe" -g3 -O0 -DDEBUG -o %GCC_OUTPUT% %SOURCE% -std=c11 -Wall -Wextra -lpthread
if errorlevel 1 (
    echo ERROR: GCC debug build failed
    goto :error
)
echo [DEBUG-GCC] Debug build successful: %GCC_OUTPUT%
echo Use: gdb %GCC_OUTPUT%
goto :end

:debug_msvc
echo [DEBUG-MSVC] Building %SOURCE% with MSVC (debug optimized)...
if not exist "%VCVARS_PATH%" (
    echo ERROR: Visual Studio not found
    goto :error
)

call "%VCVARS_PATH%" >nul 2>&1
cl /ZI /Od /DDEBUG /Fe:%MSVC_OUTPUT% %SOURCE% /W4 /nologo
if errorlevel 1 (
    echo ERROR: MSVC debug build failed
    goto :error
)
echo [DEBUG-MSVC] Debug build successful: %MSVC_OUTPUT%
echo Use: devenv %MSVC_OUTPUT% or attach debugger
goto :end

:error
echo.
echo Build failed with errors!
exit /b 1

:end
echo.