
rem This script is used to call the Qt user interface compiler (uic) and also create the output directory if needed.

@echo off

rem -- Parameters as named variables
set QtUicExecutable=%~1
set FullPath=%~2
set QtUicOutput=%~3
set Filename=%~4

rem -- Execute commands
md "%QtUicOutput%"
"%QtUicExecutable%" "%FullPath%" -o "%QtUicOutput%ui_%Filename%.h"
