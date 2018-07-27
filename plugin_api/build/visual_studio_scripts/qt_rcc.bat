
rem This script is used to call the Qt user interface compiler (uic) and also create the output directory if needed.

@echo off

rem -- Parameters as named variables
set QtRccExecutable=%~1
set FullPath=%~2
set QtRccOutput=%~3
set Filename=%~4

rem -- Execute commands ("-name" to get unique "qInitResources" and "qCleanupResources" function names so we can link in multiple Qt resource files)
md "%QtRccOutput%"
"%QtRccExecutable%" "%FullPath%" -o "%QtRccOutput%res_%Filename%.cpp" -name "%Filename%"
