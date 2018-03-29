
@echo off

REM This script intend to work only when devenv.exe defined in the system PATH, e.g. VS 2010 command prompt

where devenv.exe
if %ERRORLEVEL% EQU 0 GOTO CONTINUE
echo ERROR: Can't find devenv.exe, are you running from Visual Studio command prompt?
goto END

:CONTINUE

SET BUILD_DIR=.build_windows
SET EXTLIB_BUILD_DIR=extlib\%BUILD_DIR%

REM  Build all external libs
IF NOT exist %EXTLIB_BUILD_DIR% mkdir %EXTLIB_BUILD_DIR%

pushd %EXTLIB_BUILD_DIR%
cmake ..
@echo on
devenv extlib.sln /Build
@echo off

popd

REM Build the target
if NOT exist %BUILD_DIR% mkdir %BUILD_DIR%

pushd %BUILD_DIR%
cmake ..
@echo on
devenv spotcheck.sln /Build
@echo off

popd

:END
