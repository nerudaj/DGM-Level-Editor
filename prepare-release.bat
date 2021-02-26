rem Source cmake path
call /tools/doomsh.cmd

rem Set these variables to your liking
set BUILDDIR=vsbuild
set SOLUTION=template.sln
set RELDIR=RELEASE
set PROJECT_NAME=Template

@echo off

echo Phase 1 - Purging
rd /s /q %BUILDDIR%

echo Phase 2 - Configuring
mkdir %BUILDDIR%
cd %BUILDDIR%
cmake.exe ..
cd ..

echo Phase 3 - Building
call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\VsDevCmd.bat"
cd %BUILDDIR%
devenv %SOLUTION% /Build Release
cd ..

echo Phase 4 - Packaging
cd %BUILDDIR%
cpack.exe
cd ..

echo Phase 5 - Finalizing
mkdir RELEASE
copy %BUILDDIR%\*.zip RELEASE

echo Done!

pause

@echo on