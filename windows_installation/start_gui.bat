@echo off
REM 🖥️ DÉMARRAGE RAPIDE - INTERFACE GRAPHIQUE (WINDOWS)
REM Script ultra-simplifié pour lancer l'interface

setlocal enabledelayedexpansion

set PROJECT_DIR=%cd%
set GUI_DIR=%PROJECT_DIR%\gui
set BUILD_DIR=%GUI_DIR%\build

if not exist "%BUILD_DIR%" (
    mkdir "%BUILD_DIR%"
)

echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║         🖥️  INTERFACE GRAPHIQUE WINDOWS                    ║
echo ╚════════════════════════════════════════════════════════════╝
echo.

REM Chercher l'exécutable
if exist "%GUI_DIR%\agent_manager_unified.exe" (
    echo [✓] Exécutable trouvé: agent_manager_unified.exe
    echo [*] Lancement de l'interface...
    echo.
    cd /d "%GUI_DIR%"
    agent_manager_unified.exe
    exit /b 0
)

if exist "%GUI_DIR%\agent_manager_unified" (
    echo [✓] Exécutable trouvé: agent_manager_unified
    echo [*] Lancement de l'interface...
    echo.
    cd /d "%GUI_DIR%"
    agent_manager_unified
    exit /b 0
)

REM Chercher d'autres exécutables disponibles
echo [!] Exécutables principaux non trouvés
echo [*] Recherche des exécutables disponibles...
echo.

cd /d "%GUI_DIR%"

for /f "tokens=*" %%A in ('dir /b /s *.exe 2^>nul') do (
    echo [✓] Trouvé: %%A
    set FOUND_EXE=%%A
)

if defined FOUND_EXE (
    echo.
    echo [*] Lancement de: !FOUND_EXE!
    echo.
    call !FOUND_EXE!
    exit /b 0
)

REM Si aucun exécutable
echo [!] Aucun exécutable trouvé
echo.
echo Options:
echo   1. Utilisez launcher.bat pour plus d'options
echo   2. Consultez INSTALLATION_WINDOWS.md
echo   3. Compilez manuellement avec Visual Studio ou MinGW
echo.
pause
exit /b 1
