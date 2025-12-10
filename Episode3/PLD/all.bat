@echo off

echo.
echo Compiling: MemDecode.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL MemDecode.pld
if %ERRORLEVEL% EQU 0 (
    echo Passed!
) else (
    echo FAILED! Look up the.lst and .doc to debug.
)

echo.
echo Compiling: IOdecode.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL IOdecode.pld
if %ERRORLEVEL% EQU 0 (
    echo Passed!
) else (
    echo FAILED! Look up the.lst and .doc to debug.
)

echo.
echo Compiling: WaitStateGen.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL NewWaitStateGen.pld
if %ERRORLEVEL% EQU 0 (
    echo Passed!
) else (
    echo FAILED! Look up the.lst and .doc to debug.
)

echo.
echo Compiling: ShadowPlus.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL ShadowPlus.pld
if %ERRORLEVEL% EQU 0 (
    echo Passed!
) else (
    echo FAILED! Look up the.lst and .doc to debug.
)
