@echo off

rem -- Call this script as part of a post-build event with the following command:
rem  call "$(QsfDir)build\visual_studio_scripts\postbuild\delete_lastbuildstate.bat" $(ProjectName) $(IntDir) $(TargetExt)

rem -- Parameters as named variables (also remove surrounding quotes with the ~ notation)
set ProjectName=%~1
set IntDir=%~2
set TargetExt=%~3

rem -- Remove backslashes at the end
if %IntDir:~-1%==\ set IntDir=%IntDir:~0,-1%

rem -- Delete lastbuildstate file (only for dynamic builds)
rem -- TODO(fw): what about exe?
if %TargetExt%==".lib" goto done

	rem -- Worked all the time for VS 2012
	if exist "%IntDir%\%ProjectName%.lastbuildstate" del "%IntDir%\%ProjectName%.lastbuildstate"

	rem -- Needed for VS 2015
	if exist "%IntDir%\%ProjectName%.tlog\%ProjectName%.lastbuildstate" del "%IntDir%\%ProjectName%.tlog\%ProjectName%.lastbuildstate"

:done
