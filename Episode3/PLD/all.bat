@echo off

echo.
echo Compiling: CPUSpeedSel.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL CPUSpeedSel.pld
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
echo Compiling: MemDecode.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL MemDecode.pld
if %ERRORLEVEL% EQU 0 (
    echo Passed!
) else (
    echo FAILED! Look up the.lst and .doc to debug.
)

echo.
echo Compiling: REGdecode.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL REGdecode.pld
if %ERRORLEVEL% EQU 0 (
    echo Passed!
) else (
    echo FAILED! Look up the.lst and .doc to debug.
)

echo.
echo Compiling: WaitStateGen.pld
echo ======================================
cupl -m3lxudfjab -u C:\Wincupl\Shared\Atmel.DL WaitStateGen.pld
if %ERRORLEVEL% EQU 0 (
    echo Passed!
) else (
    echo FAILED! Look up the.lst and .doc to debug.
)
