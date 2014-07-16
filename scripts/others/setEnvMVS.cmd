
@echo off

if %3 == cl (
    if %2 == x86 (
        if exist "%VS100COMNTOOLS%/vsvars32.bat" (
                call "%VS100COMNTOOLS%/vsvars32.bat"
        )
    )
    if %2 == amd64 (
        if exist "%VS100COMNTOOLS%/../../VC/vcvarsall.bat" (
                call "%VS100COMNTOOLS%/../../VC/vcvarsall.bat" amd64
        )
    )

    @nmake -nologo CFG=%1 ARCH=%2 COMP=%3 -f makefile
    @nmake -nologo CFG=%1 ARCH=%2 -f makefile-cuda

)


REM
REM if %3 == g++ (
REM    if %2 == x86 (
REM        if exist "mingwvars.bat" (
REM                call "mingwvars.bat"
REM        )
REM    )
REM )




