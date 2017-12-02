@echo off
REM :: ______________________________________________________________________
REM :: |                                                                    |__
REM :: | Objetive: Call to Tiny C Compiler a fast compiler.                 | |
REM :: |  Version: 0.1 for Windows NT family and Linux Wine. Copyleft 2014  | |
REM :: |   Author: Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>   | |
REM :: |   Syntax: %0 [[path\]filename]                                     | |
REM :: | Observation: The TCC 64 bits for Windows don't work properly.      | |
REM :: |____________________________________________________________________| |
REM ::   |____________________________________________________________________|
SETLOCAL EnableDelayedExpansion
if "%~1"=="$#·H·E·L·P·#$" goto Help
set "_ProgramFiles86_=%ProgramFiles(x86)%"
if     "%ProgramFiles(x86)%"==""  set "_bits=32" 
REM if not "%ProgramFiles(x86)%"==""  set "_bits=64"
if not "%ProgramFiles(x86)%"==""  set "_bits=32"
set "_PathTCC=%~dp0tcc%_bits%"

if     defined WINELOADERNOEXEC 	set _start="%WinDir%\Command\start.exe"
if not defined WINELOADERNOEXEC 	set _start=start "."

:: /---------------------[ Parse Options ]---------------------\
if "%~1"=="/?"     goto HelpAux
if "%~1"=="/h"     goto HelpAux
if "%~1"=="-h"     goto HelpAux
if "%~1"=="--help" goto HelpAux

if "%~1"=="/??"  goto Documentation
if "%~1"=="/doc" goto Documentation
if "%~1"=="/v"   goto Version
if "%~1"=="-v"   goto Version

set "_FlagTarget="
if "%~1"=="/run"   set "_FlagTarget=RUN"   & shift /1
if "%~1"=="/exe"   set "_FlagTarget=EXE"   & shift /1
REM if "%~1"=="/win"   set "_FlagTarget=WIN"   & shift /1
REM if "%~1"=="/wince" set "_FlagTarget=WINCE" & shift /1
if "%~1"=="/dlib"  set "_FlagTarget=DLIB"  & shift /1
if "%~1"=="/slib"  set "_FlagTarget=SLIB"  & shift /1
if "%~1"=="/make"  set "_FlagTarget=MAKE"  & shift /1
if not defined _FlagTarget set "_FlagTarget=RUN"

set _ShowTime=FALSE
if "%~1"=="/time"  set _ShowTime=TRUE
if "%~1"=="/t"     set _ShowTime=TRUE
if "%~1"=="-t"     set _ShowTime=TRUE
if "%~1"=="--time" set _ShowTime=TRUE
if %_ShowTime%==TRUE shift /1

set _FlagFile=FALSE
if "%~1"=="/file"  set _FlagFile=TRUE
if "%~1"=="/f"     set _FlagFile=TRUE
if "%~1"=="-f"     set _FlagFile=TRUE
if "%~1"=="--file" set _FlagFile=TRUE
if %_FlagFile%==TRUE shift /1

:: /--------------------[ Parse Arguments ]--------------------\
set _cscript="%~f1"
if not exist %_cscript% goto ErrExistProg
set  _CscriptName="%~dpn1"
set _csBin="%~n1.exe"
set _csDlib="%~n1.dll"
set _csSlib="%~n1.lib"
set _csMake="%~n1.make"

REM if not defined WINELOADERNOEXEC  set  _csBin="%~dpn1.exe"
REM if not defined WINELOADERNOEXEC  set _csSlib="%~dpn1.lib"
REM if not defined WINELOADERNOEXEC  set _csDlib="%~dpn1.dll"

REM if     defined WINELOADERNOEXEC  set  _csBin="%~dpn1"
REM if     defined WINELOADERNOEXEC  set _csSlib="%~dpn1.a"
REM if     defined WINELOADERNOEXEC  set _csDlib="%~dpn1.so"


set "_Output="
if %_FlagFile%==TRUE call :CreateFile _Output _Filelog %_cscript%

set "_args="
:Get_Args
    if "%~2"=="" goto Continue
    set _args= %_args% %2
    shift /2
    goto :Get_Args
:Continue

:: /--------------------[ Setting TCC ]--------------------\
if %_FlagTarget%==RUN  set _BinTarget=-l"tcc" -run
if %_FlagTarget%==EXE  set _BinTarget=-l"tcc" -Wl,-subsystem=console -o "%_CscriptName%.exe"
if %_FlagTarget%==DLIB set _BinTarget=-shared -l"tcc" -Wl,-subsystem=console -soname "%_CscriptName%.dll.0.0.1" -rdynamic -o "%_CscriptName%.dll"
if %_FlagTarget%==SLIB set _BinTarget=-static -Wl,-subsystem=console -o "%_CscriptName%.lib" 
REM if %_FlagTarget%==DLIB set _BinTarget=-shared -Wl,--oformat=coff -Wl,-subsystem=console -soname "%_CscriptName%.dll.0.0.1" -rdynamic -o "%_CscriptName%.dll"
REM if %_FlagTarget%==SLIB set _BinTarget=-static -Wl,--oformat=coff -Wl,-subsystem=console -o "%_CscriptName%.lib" 
if %_FlagTarget%==MAKE set _BinTarget=-MD -MF "%_CscriptName%.make" -Wl,-subsystem=console

:: \-----------------------------------------------------------/

set _HeaderPath=-I"%_PathTCC%\include" -I"%_PathTCC%\include\winapi" -I"%_PathTCC%\libtcc"
set    _LibPath=-L"%_PathTCC%\lib" -L"%_PathTCC%\libtcc"
set   _ResPath="%_PathTCC%\lib;%_PathTCC%\libtcc"
:: \-----------------------------------------------------------/

echo.
"%_PathTCC%\tcc.exe" -v
echo ..............................................................................

if defined _Output goto :Call_CScript

call :GetTime _nStart
"%_PathTCC%\tcc.exe" %_HeaderPath% %_LibPath% -Wl -Wl,-rpath=%_ResPath% %_BinTarget% %1 %_args%
set _errorlevel=%errorlevel%
call :GetTime _nEnd
call :Error_Msg %_errorlevel%  %_cscript%
if %_ShowTime%==TRUE call :TimeElapsed %_nStart% %_nEnd%

ENDLOCAL & exit /b %_errorlevel%

:: --------------------------------------------------------

:Call_CScript
set "_errorlevel="
call :GetTime _nStart
"%_PathTCC%\tcc.exe" %_HeaderPath% %_LibPath% -Wl -Wl,-rpath=%_ResPath% %_BinTarget%  %1 %_args% >%_Output% 2>%_Filelog%
set _errorlevel=%errorlevel%
call :GetTime _nEnd
if %_ShowTime%==TRUE call :TimeElapsed %_nStart% %_nEnd%
if %_errorlevel%==0 if %_FlagTarget%==RUN (
    for %%a in ( %_Output% )  do   if exist %%a  if "%%~za"=="0"  del /f /q %_Output%
    for %%a in ( %_Filelog% ) do   if exist %%a  if "%%~za"=="0"  del /f /q %_Filelog%

    if exist %_Output%   %_start% "%_PathTCC%\..\LogView\logview.exe" %_Output%
    if exist %_Filelog%  %_start% "%_PathTCC%\..\LogView\logview.exe" %_Filelog%
)
call :Error_Msg %_errorlevel%  %_cscript%
ENDLOCAL & exit /b %_errorlevel%

:: --------------------------------------------------------

:Error_Msg :: <Error_Code>  <Cscript-name>
	if %1 gtr 0 (
		echo.
		echo   --^> Errors [%1] were found in "%~nx2".
		echo.
	) else (
		if %_FlagTarget%==RUN  goto :EOF
		if %_FlagTarget%==EXE  set _msgTarget=%_csBin%
		if %_FlagTarget%==DLIB set _msgTarget=%_csDlib%
		if %_FlagTarget%==SLIB set _msgTarget=%_csSlib%
		if %_FlagTarget%==MAKE set _msgTarget=%_csMake%
		echo  Source: %2
		if %_FlagTarget%==MAKE (
			call  echo  Target: %%_csBin%%
		)
		call  echo  Target: %%_msgTarget%%		...created successfully.
		if %_FlagTarget%==MAKE (
		if exist %_csMake%  %_start% "%_PathTCC%\..\LogView\logview.exe" %_csMake%
		)
	)
goto :EOF

:CreateFile :: <io_var1> <io_var2> <cscript_name>
SETLOCAL
    set "_PathOut=%~dp3Output"
    if not exist "%_PathOut%" mkdir "%_PathOut%"
    set /a _n=0
    set "_base=%_PathOut%\%~nx3_"
    set "_lastFile="
    for /F "tokens=*" %%F in ( 'dir /o /b "%_base%*.txt" 2^>nul' ) do set "_lastFile=%%F"
    if     defined _lastFile call :Get_Nextile    _Next_File "%_base%" "%_lastFile%"
    if not defined _lastFile call :Set_NumberFile _Next_File "%_base%" %_n% 
    set __File_err=%_Next_File:.txt=.log%
ENDLOCAL & set %1=%_Next_File%& set %2=%__File_err%
goto :EOF

:Set_NumberFile :: <io_var> <base_name> <number>
    set "_n=00000%~3"
    set %1="%~2%_n:~-4%.txt"
goto :EOF

:Get_Nextile :: <io_var> <base_name> <last_file_text>
SETLOCAL
    set "_name=%~n3"
    set "_name0=%_name:*.=%"
    set "_name1=%_name0:*-=%"
    set "_name2=%_name1:*-=%"
    set "_name3=%_name2:*-=%"
    set "_name4=%_name3:*_0000=%"
    set "_name5=%_name4:*_000=%"
    set "_name6=%_name5:*_00=%"
    set "_name7=%_name6:*_0=%"
    set /a _next = _name7 + 1
    call :Set_NumberFile _fileout %2 %_next%
ENDLOCAL & set %1=%_fileout%
goto :EOF
:: --------------------------------------------------------

:Version
echo.
"%_PathTCC%\tcc.exe" -v
ENDLOCAL
exit /b 0

:Documentation
echo.
%_start%  "%_PathTCC%\doc\tcc-doc.chm"
ENDLOCAL
exit /b 0
:: ---------------------------------------------------------------------------------
:: Get Time in hundredth of a second.
:GetTime :: <IO-var>
SETLOCAL
set "_Cnt=0"
for /F "tokens=1* delims=:" %%a in ( 'echo.^|time' ) do  call :ParseTime _time %%b
ENDLOCAL & set "%1=%_time%"
goto  :EOF

:ParseTime :: <io-var>  <os-time>
    if not "%_Cnt%"=="0" goto :EOF
    for /F "tokens=1-3 delims=:" %%a in ( "%2" ) do set "hh=%%a" & set "mm=%%b" & set "ss=%%c"
    set ss=%ss:.=%
    set /a mm = mm * 60 * 100 
    set /a hh = hh * 3600 * 100
    set /a %1=hh + mm +ss
    set /a _Cnt += 1
goto :EOF

:TimeElapsed :: <Time1> <Time2>
SETLOCAL
    set /a _t1=%1
    set /a _t2=%2
    if %_t1% gtr %_t2% set /a _t1=%2 & set /a _t2=%1
    set /a _dif= _t2 - _t1
    set "_ms1=000%_dif%"
    set "cs=%_ms1:~-2%"
    set "tt=%_dif:~0,-2%"
    set /a  hh1 = tt / 3600
    set /a tt1 = tt - hh1 * 3600 
    set /a  mm1 = tt1 / 60
    set /a ss1 = tt1 - mm1 * 60
    set ss2=00%ss1%.%cs%
    set "hh2=00%hh1%"
    set "hh=%hh2:~-2%"
    set "mm2=00%mm1%"
    set "mm=%mm2:~-2%"
    set "ss=%ss2:~-5%"
    echo                                             .-=( Time elapsed %hh%:%mm%:%ss% )=-.
ENDLOCAL
goto :EOF
:: ---------------------------------------------------------------------------------

:ErrExistProg
echo.
echo.  ____________________________________________________________________________
echo. /                                                                            \
echo.   Error in %~n0.
echo    the program %_cscript% not exist.
echo. \____________________________________________________________________________/
ENDLOCAL
exit /b 0

:HelpAux
set _call_Help=%_start% "%~f0" "$#·H·E·L·P·#$"
%_start% cmd /c ""%~f0" "$#·H·E·L·P·#$""
ENDLOCAL
exit /b 0

:Help
echo.  ____________________________________________________________________________
echo. /                                                                            \
echo.  %~n0 v1.0 for Windows NT family and Linux Wine. Copyleft 2016.
echo.  By Edgard Bernardo Medina Quispe, [edgardmedina@yahoo.com]. Arequipa, Peru.
echo. ______________________________________________________________________________
echo.
echo.  This batch-script do a fast call of Tiny C Compiler (TCC/ISO-C99) 32-64 bits
echo.  to run C-script or made compile programs on Windows (.exe;.dll;.lib;.def) or
echo.  on Linux  (.bin;.so;.a;.def) or on OS X (.bin;.dylib;.x).
echo.
echo.  TCC is portable, don't need be installed, just copy where you need it and 
echo.  execute it, for ease of use, add the path of '%~nx0' to the PATH environment
echo.  variable, or use a Batch script to access to %~nx0 (see the NOTE).

echo.
echo.  Syntax:
echo.   %~n0 {INFO}
echo.   %~n0 [/run] [/time] [/file] {C-Program} [arg1] [arg2] ...
echo.   %~n0 {OPT_GEN}      {C-Program} {C-Program} [C-Program] [C-Program] ...
echo.
echo.  Where BNF:
echo.      INFO ::= /?         // Call help about this script.
echo.             ^| /??        // Call documentation about TCC.
echo.             ^| /v         // Show version about TCC.
echo.   
echo.   OPT_GEN ::= /exe       // Create a console binary executable 
echo.             ^| /dlib      // Create a Shared Library: .dll (Win) or .so (Linux)
echo.             ^| /slib      // Create a Static Library: .lib (Win) or .a (Linux)
echo.             ^| /make      // Create a binary executable and make file 
echo.                          // from {C-Program} list
echo.  Where:
REM echo.   /debug       Generate run time debug information.
echo.   /run         (Default) Compile in RAM and execute the {C-Program} directly 
echo.                from RAM, a behavior similar to interpreter language.
echo    /time ^| /t   (Optional) Show the time (in hundredth of a second) of the
echo                 program execution.
echo    /file ^| /f   (Optional) The output in console is sending to a file with name
echo                 "Output\{C-Program}-0xxx.txt" in a directory called 'Output' and
echo                 later is show it in a window.
echo.   [arg1] [arg2] ...
echo                 Optional arguments for {C-Program}.
echo.   {C-Program}  User program in GNU C99.
echo.
REM pause
echo.  NOTE:
echo.   For access to '%~nx0' without use the  PATH environment variable, you can 
echo.   create the custom Batch script called 'tc.cmd' in your favorite directory.
echo.   ________________________________________________________________________
echo.   ^| @echo off                                                            ^|
echo.   ^| SETLOCAL                                                             ^|
echo.   ^| :: [You can customize the path in TCCPATH for access to 'tcc.cmsd']  ^|
echo.   ^| set TCCPATH=.\tcc                                                    ^|
echo.   ^| if "%%~1"=="/?"    goto Help                                          ^|
echo.   ^| if "%%~1"=="/??"   goto Doc                                           ^|
echo.   ^| call %%TCCPATH%%\tcc.cmd %%*                                            ^|
echo.   ^| exit /b %%errorlevel%%                                                 ^|
echo.   ^|                                                                      ^|
echo.   ^| :Help                                                                ^|
echo.   ^| call %%TCCPATH%%\tcc.cmd "/?"                                          ^|
echo.   ^| exit /b %%errorlevel%%                                                 ^|
echo.   ^|                                                                      ^|
echo.   ^| :Doc                                                                 ^|
echo.   ^| call %%TCCPATH%%\tcc.cmd "/??"                                         ^|
echo.   ^| exit /b %%errorlevel%%                                                 ^|
echo.   ^|______________________________________________________________________^|
echo.  
echo. \____________________________________________________________________________/
ENDLOCAL
REM for /L %%a in (1,1,100) do pause >nul
pause >nul
exit /b 0

