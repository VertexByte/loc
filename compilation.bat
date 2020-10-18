@echo off

cl -nologo -Od -Zi loc.cpp /Fe"loc.exe" /link user32.lib shlwapi.lib kernel32.lib

