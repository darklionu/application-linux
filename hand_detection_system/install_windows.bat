@echo off
REM Installation pour Windows - Système de Détection de Mains

color 0A
cls

echo.
echo ╔════════════════════════════════════════════════════════════════╗
echo ║                                                                ║
echo ║   🎥 INSTALLATION WINDOWS - DÉTECTION DE MAINS 🎥            ║
echo ║                                                                ║
echo ║          Installation Automatique pour Windows 10/11          ║
echo ║                                                                ║
echo ╚════════════════════════════════════════════════════════════════╝
echo.

REM Vérifier Python
echo [1/4] Vérification de Python...
python --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Python n'est pas installé
    echo.
    echo Installez Python depuis: https://www.python.org/downloads/
    echo Assurez-vous de cocher "Add Python to PATH"
    pause
    exit /b 1
)
echo ✅ Python trouvé
python --version

REM Mettre à jour pip
echo.
echo [2/4] Mise à jour de pip...
python -m pip install --upgrade pip
if errorlevel 1 (
    echo ⚠️  Erreur lors de la mise à jour de pip
)
echo ✅ pip mis à jour

REM Installer OpenCV
echo.
echo [3/4] Installation d'OpenCV (pour Windows)...
python -m pip install opencv-python
if errorlevel 1 (
    echo ❌ Erreur lors de l'installation d'OpenCV
    pause
    exit /b 1
)
echo ✅ OpenCV installé

REM Installer MediaPipe
echo.
echo [4/4] Installation de MediaPipe...
python -m pip install mediapipe
if errorlevel 1 (
    echo ⚠️  Erreur lors de l'installation de MediaPipe
)
echo ✅ MediaPipe installé

echo.
echo ╔════════════════════════════════════════════════════════════════╗
echo ║                                                                ║
echo ║                ✅ INSTALLATION TERMINÉE ✅                    ║
echo ║                                                                ║
echo ║                 Vous pouvez maintenant lancer:                ║
echo ║                                                                ║
echo ║                  python main_windows.py                       ║
echo ║                                                                ║
echo ╚════════════════════════════════════════════════════════════════╝
echo.

pause
