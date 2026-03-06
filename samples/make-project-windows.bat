@echo off

set SCRIPT_DIR=%~dp0
set PREMAKE=%SCRIPT_DIR%..\tools\premake\bin\windows\premake5.exe
set SCRIPT=%SCRIPT_DIR%make-project-standalone.lua

%PREMAKE% --file=%SCRIPT% vs2026
