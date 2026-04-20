@echo off
REM 🖐️ DÉMARRAGE RAPIDE - DÉTECTION DE MAINS (WINDOWS)
REM Script ultra-simplifié pour lancer la détection

setlocal enabledelayedexpansion

set PROJECT_DIR=%cd%
set HAND_DIR=%PROJECT_DIR%\hand_detection_system

echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║       🖐️  DÉTECTION DE MAINS WINDOWS                       ║
echo ╚════════════════════════════════════════════════════════════╝
echo.

REM Vérifier Python
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [!] ERREUR: Python n'est pas installé ou n'est pas dans PATH
    echo.
    echo Solution:
    echo   1. Téléchargez Python 3.9+: https://www.python.org/downloads/
    echo   2. IMPORTANT: Cochez "Add Python to PATH" lors de l'installation
    echo   3. Redémarrez cmd.exe
    echo   4. Relancez ce script
    echo.
    pause
    exit /b 1
)

echo [✓] Python trouvé
echo [*] Installation des dépendances...
echo.

cd /d "%HAND_DIR%"

REM Installer les dépendances
python -m pip install opencv-python mediapipe -q 2>nul
if %errorlevel% neq 0 (
    echo [!] Erreur lors de l'installation des dépendances
    echo.
    echo Essayez manuellement:
    echo   python -m pip install --upgrade opencv-python mediapipe
    echo.
    pause
    exit /b 1
)

echo [✓] Dépendances OK
echo.

REM Lancer le système
if exist "main.py" (
    echo [*] Lancement du système de détection...
    echo.
    echo Commandes:
    echo   - Pointez une main vers la caméra
    echo   - Appuyez sur 'q' pour quitter
    echo.
    python main.py
) else (
    echo [!] Erreur: main.py non trouvé
    pause
    exit /b 1
)

exit /b 0
