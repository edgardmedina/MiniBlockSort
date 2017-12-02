@echo off
REM :: _____________________________________________________________________
REM :: |                                                                   |__
REM :: | Objetive: Call to Tiny C Compiler  to fast compiler.              | |
REM :: |  Version: 0.1 for Windows NT family and Linux Wine. Copyleft 2014 | |
REM :: |   Author: Edgard Bernardo Medina Quispe <edgardmedina@yahoo.com>  | |
REM :: |   Syntax: %0 [[path\]filename]                                    | |
REM :: |___________________________________________________________________| |
REM ::   |___________________________________________________________________|
SETLOCAL

:: [You can customize the path in TCCPATH for access to 'tcc.cmsd']
set TCCPATH=.\tcc

if "%~1"=="/?"    goto Help
if "%~1"=="/??"   goto Doc

call %TCCPATH%\tcc.cmd %*
exit /b %errorlevel%

:Help
call %TCCPATH%\tcc.cmd "/?"
exit /b %errorlevel%

:Doc
call %TCCPATH%\tcc.cmd "/??"
exit /b %errorlevel%
