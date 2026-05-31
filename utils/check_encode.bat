@echo off
chcp 65001 >nul
powershell -ExecutionPolicy Bypass -File "_utf8nbchecker.ps1"
pause