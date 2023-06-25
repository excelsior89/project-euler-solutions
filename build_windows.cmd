rem Copyright 2020-2023 Paul Robertson
rem
rem Build script for Windows

rem Default to release build
set build=Release
if "%~1"=="Debug" set build=Debug

rem Create build directory
mkdir build_windows
cd build_windows
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

rem Run CMake targeting VS2017 and 64bit architecture
cmake -G "Visual Studio 16 2019" -A x64 %* ..
cmake --build . --target install --config %build% -- /M

rem Now install the build target
rem cmake --build . --target install
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cd ..