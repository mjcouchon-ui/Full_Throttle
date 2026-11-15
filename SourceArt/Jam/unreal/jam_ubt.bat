@echo off
rem Runs UnrealBuildTool WITHOUT the engine's Build.bat.
rem Build.bat holds a file lock of its own for as long as it runs (%tmp%\...Build.bat.lock), and every editor or game
rem start on this engine calls Build.bat -Mode=ValidatePlatforms and waits for it. So one compile through Build.bat
rem made EVERY game start on the PC wait until the compile had ended (15 Nov: three games stood at 0.9 GB for five
rem minutes behind one compile; this is what stalled the PC at noon). This file does what Build.bat does after its
rem lock, nothing else: the engine's own dotnet, the working directory UnrealBuildTool wants, the same arguments.
rem   jam_ubt.bat FullThrottleEditor Win64 Development "-project=<project>\FullThrottle.uproject" -NoMutex -NoUBA ...
rem Always pass -NoMutex (or two builds wait for each other inside the tool) and a -log= of your own (the tool's
rem default log file is one file for the whole PC). Never two builds of the SAME checkout at once: the callers'
rem checkout lock sees to that.
setlocal
if "%JAM_ENGINE%"=="" set JAM_ENGINE=K:\UE_5.9
pushd "%JAM_ENGINE%\Engine\Source"
if errorlevel 1 exit /B 999
call "%JAM_ENGINE%\Engine\Build\BatchFiles\GetDotnetPath.bat"
if errorlevel 1 ( popd & exit /B 999 )
dotnet "..\..\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll" %*
set JAM_UBT_EXIT=%ERRORLEVEL%
popd
exit /B %JAM_UBT_EXIT%
