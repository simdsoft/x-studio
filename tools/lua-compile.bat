@rem Copyright (c) 2014-2022 Simdsoft Limited, All rights reserved.
@rem Usage: "%XS_INSTDIR%\tools\lua-compile.bat" IN_DIR OUT_DIR [STRIP] [KEEP_EXT] [BC_TYPE] [NO_GC64]
@rem reference links:
@rem   a. https://lua.org
@rem   b. https://luajit.org/running.html#opt_b
@echo off
set TOOLS_DIR=%~dp0
set IN_DIR=%1
set OUT_DIR=%2
set STRIP=%3
set KEEP_EXT=%4
set BC_TYPE=%5
set NO_GC64=%6

if not defined IN_DIR set /p IN_DIR=Please input source directory:
if not defined OUT_DIR set /p OUT_DIR=Please input destination directory:

if not defined STRIP set STRIP=true
if not defined KEEP_EXT set KEEP_EXT=false
if not defined BC_TYPE set BC_TYPE=plain

set INVALARG=0
if not exist "%IN_DIR%" echo the source directory %IN_DIR% not exist! && set INVALARG=1 && goto :L_exit
if not exist "%OUT_DIR%" echo The destination directory %OUT_DIR% not exist! && set INVALARG=1 && goto :L_exit

rem Convert to absolute path
pushd %IN_DIR%
set IN_DIR=%CD%
popd

pushd %OUT_DIR%
set OUT_DIR=%CD%
popd

rem determine luac executable file
set LUAC_TOOL=
set LUAC_WORKDIR=

if "%BC_TYPE%"=="plain" (
  set LUAC_TOOL="%XS_INSTDIR%\luac.exe" 
  set LUAC_WORKDIR="%XS_INSTDIR%" 
) else if "%BC_TYPE%"=="jit" (
  if "%NO_GC64%"=="true" (
    set LUAC_TOOL="%TOOLS_DIR%luajit\luajit.exe"
  ) else (
    set LUAC_TOOL="%TOOLS_DIR%luajit\gc64\luajit.exe"
  )
  set LUAC_WORKDIR="%TOOLS_DIR%scripts"
) else (
  echo "Invalid BC_TYPE, must be 'plain' or 'jit'"
  goto :L_exit
)

rem print parameters
echo IN_DIR=%IN_DIR%
echo OUT_DIR=%OUT_DIR%
echo LUAC_TOOL=%LUAC_TOOL%
echo LUAC_WORKDIR=%LUAC_WORKDIR%

rem determine strip opt
set STRIP_OPT=
if "%STRIP%"=="true" set STRIP_OPT=-s

pushd %LUAC_WORKDIR%
if "%BC_TYPE%"=="plain" (
  for /f "usebackq" %%i in (`"%TOOLS_DIR%scripts\find_lua" %IN_DIR%`) do (echo %%i&&call %LUAC_TOOL% %STRIP_OPT% -o "%%~dpi%%~ni%%~xic" "%%i")
) else (
  for /f "usebackq" %%i in (`"%TOOLS_DIR%scripts\find_lua" %IN_DIR%`) do (echo %%i&&call %LUAC_TOOL% -b %STRIP_OPT% "%%i" "%%~dpi%%~ni%%~xic")
)
popd

if defined OUT_DIR xcopy /y /s /e /f /h "%IN_DIR%\*.luac" "%OUT_DIR%\" && del /f /q /s "%IN_DIR%"\*.luac

if "%KEEP_EXT%"=="true" (
  for /r "%OUT_DIR%" %%i in (*.luac) do (echo Renaming %%i&&call move /y "%%i" "%%~dpi%%~ni.lua")
)

:L_exit
if %INVALARG% NEQ 0 (
  echo Usage: "%%XS_INSTDIR%%\tools\lua-compile.bat" IN_DIR OUT_DIR [STRIP] [KEEP_EXT] [BC_TYPE] [NO_GC64]
)
ping /n 2 127.0.1>nul && goto :eof
