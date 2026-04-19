@echo off
REM Lanceur Windows - Détection de Mains

color 0A
cls

echo.
echo ╔════════════════════════════════════════════════════════════════╗
echo ║                                                                ║
echo ║   🎥 DÉTECTION DE MAINS - LANCEUR WINDOWS 🎥                ║
echo ║                                                                ║
echo ╚════════════════════════════════════════════════════════════════╝
echo.

echo Quelle version voulez-vous lancer?
echo.
echo 1. Version Ultra-Légère (menu simple)
echo 2. Version Standard (recommandée)
echo 3. Version Complète (avec MediaPipe)
echo 4. Quitter
echo.

set /p choice="Choisir une option (1-4): "

if "%choice%"=="1" (
    echo.
    echo Lancement de la version ultra-légère...
    echo.
    python windows_ultra_light.py
) else if "%choice%"=="2" (
    echo.
    echo Lancement de la version standard...
    echo.
    python main_windows.py
) else if "%choice%"=="3" (
    echo.
    echo Lancement de la version complète...
    echo.
    python main.py
) else if "%choice%"=="4" (
    echo Au revoir!
) else (
    echo Choix invalide
)

pause
