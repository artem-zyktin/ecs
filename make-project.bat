@echo off

set PREMAKE=tools\premake\bin\windows\premake5.exe
set SCRIPT=make-project.lua

%PREMAKE% --file=%SCRIPT% vs2026