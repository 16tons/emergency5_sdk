
rem This script is used to call the Qt meta object compiler (MOC) and also create the output directory if needed.

@echo off

rem -- Parameters as named variables
set QtMocExecutable=%~1
set FullPath=%~2
set QtMocOutput=%~3
set Filename=%~4
set TargetName=%~5

rem -- Execute commands
md "%QtMocOutput%"
"%QtMocExecutable%" "%FullPath%" -o "%QtMocOutput%moc_%Filename%.cpp" -b "%TargetName%/PrecompiledHeader.h"
