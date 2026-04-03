@echo off
chcp 65001 >nul
title 强力清空目录 仅保留2个文件
color 0C
cls

echo.
echo Danger
echo Will Deleting ALL files and sub-dirs in Current Dir
echo Except:：
echo.

:: ====================== 在这里修改要保留的文件名 ======================
set "KEEP1=CMakeLists.txt"
set "KEEP2=clear.bat"
:: ======================================================================

echo  %KEEP1%
echo  %KEEP2%
echo.
echo Press Any Key to Continue。
pause >nul

:: 先删除所有文件夹
for /d %%d in (*) do (
    rd /s /q "%%d" 2>nul
)

:: 再删除所有文件（含隐藏、系统，排除两个保留文件）
for /f "delims=" %%f in ('dir /b /a-d') do (
    if /i not "%%~nxf"=="%KEEP1%" if /i not "%%~nxf"=="%KEEP2%" (
        del /f /q /a "%%f"
    )
)
rd /s /q ".vs"
cls
echo.
echo Done
echo Keep：%KEEP1%  %KEEP2%
echo.
pause
exit