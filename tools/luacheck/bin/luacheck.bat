@echo off
"%XS_INSTDIR%\lua.exe" -e "package.path=[[%~dp0..\src\?.lua;%~dp0..\src\?\init.lua;]]..package.path" "%~dp0luacheck.lua" %*
