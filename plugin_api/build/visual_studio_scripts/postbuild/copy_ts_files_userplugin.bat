@echo off

rem -- Call this script as a post-build event, e.g. with the following command:
rem  call "$(QsfScriptDir)postbuild\copy_ts_files.bat" ".." "data\localization" "..\run\sample_game\data\qsf\content\localization" "$(QsfScriptDir)postbuild\binaries_sample_only.listing"

rem -- Parameters as named variables (also remove surrounding quotes with the ~ notation)
set CurrentInputDir=%~1
set CurrentOutputDir=%~2

rem -- Remove backslashes at the end
if %CurrentInputDir:~-1%==\    set CurrentInputDir=%CurrentInputDir:~0,-1%
if %CurrentOutputDir:~-1%==\   set CurrentOutputDir=%CurrentOutputDir:~0,-1%

rem -- Defines languages
set AllLanguages=de en


for %%a in (%AllLanguages%) do (

	robocopy "%CurrentInputDir%\%%a" "%CurrentOutputDir%\%%a" > nul

)
