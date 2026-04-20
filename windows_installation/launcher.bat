@echo off
REM ================================================================================
REM                    LANCEUR WINDOWS - APPLICATION LINUX
REM                     Démarrage facile de tous les systèmes
REM ================================================================================

setlocal enabledelayedexpansion

REM Configuration
set PROJECT_ROOT=%cd%
set GUI_DIR=%PROJECT_ROOT%\gui
set HAND_DIR=%PROJECT_ROOT%\hand_detection_system
set BUILD_DIR=%GUI_DIR%\build

REM Couleurs (émulation)
set RED=[91m
set GREEN=[92m
set YELLOW=[93m
set CYAN=[96m
set NC=[0m

REM ================================================================================
REM TITRE
REM ================================================================================

cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║         SYSTÈME GUI UNIFIÉ - LANCEUR WINDOWS               ║
echo ║              Gestionnaire d'Agents Centralisé              ║
echo ╚════════════════════════════════════════════════════════════╝
echo.

REM ================================================================================
REM VÉRIFICATIONS PRÉALABLES
REM ================================================================================

echo [*] Vérification de l'environnement...

if not exist "%GUI_DIR%" (
    echo [!] Erreur: Répertoire GUI non trouvé
    echo.
    pause
    exit /b 1
)

if not exist "%HAND_DIR%" (
    echo [!] Erreur: Répertoire hand_detection_system non trouvé
    echo.
    pause
    exit /b 1
)

REM Vérifier Python
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [!] Avertissement: Python n'est pas dans PATH
)

echo [✓] Environnement OK
echo.

REM ================================================================================
REM MENU PRINCIPAL
REM ================================================================================

:menu
cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║      SÉLECTIONNEZ UN SYSTÈME À LANCER                     ║
echo ╚════════════════════════════════════════════════════════════╝
echo.
echo Systèmes disponibles:
echo.
echo   1) Interface Graphique GUI ^(C++^)
echo   2) Détection de Mains ^(Python^)
echo   3) Détection Optimisée ^(Python^)
echo   4) Système Combiné ^(Détection + Interface^)
echo   5) Aide et Configuration
echo   6) Explorer les dossiers
echo   0) Quitter
echo.
set /p choice="Votre choix: "

if "%choice%"=="1" goto gui
if "%choice%"=="2" goto hand_detection
if "%choice%"=="3" goto hand_detection_optimized
if "%choice%"=="4" goto combined
if "%choice%"=="5" goto help
if "%choice%"=="6" goto explorer
if "%choice%"=="0" goto quit

echo [!] Choix invalide
timeout /t 2 /nobreak
goto menu

REM ================================================================================
REM LANCER L'INTERFACE GUI
REM ================================================================================

:gui
cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║    🖥️  LANCEMENT DE L'INTERFACE GRAPHIQUE                   ║
echo ╚════════════════════════════════════════════════════════════╝
echo.

if exist "%GUI_DIR%\agent_manager_unified.exe" (
    echo [✓] Exécutable trouvé: agent_manager_unified.exe
    echo [*] Lancement...
    echo.
    cd /d "%GUI_DIR%"
    call agent_manager_unified.exe
) else if exist "%GUI_DIR%\agent_manager_unified" (
    echo [✓] Exécutable trouvé: agent_manager_unified
    echo [*] Lancement...
    echo.
    cd /d "%GUI_DIR%"
    call agent_manager_unified
) else (
    echo [!] Exécutable non trouvé
    echo.
    echo Options:
    echo   1) Chercher les exécutables disponibles
    echo   2) Consulter le guide de compilation
    echo   0) Retour
    echo.
    set /p subchoice="Votre choix: "
    
    if "!subchoice!"=="1" (
        cd /d "%GUI_DIR%"
        dir /s *.exe 2>nul || dir /s agent_manager* 2>nul
        pause
    )
)

timeout /t 2 /nobreak
goto menu

REM ================================================================================
REM LANCER DÉTECTION DE MAINS
REM ================================================================================

:hand_detection
cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║    🖐️  LANCEMENT DE LA DÉTECTION DE MAINS                   ║
echo ╚════════════════════════════════════════════════════════════╝
echo.

where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [!] Erreur: Python n'est pas installé
    echo [*] Téléchargez Python: https://www.python.org/downloads/
    echo.
    pause
    goto menu
)

echo [*] Installation des dépendances...
cd /d "%HAND_DIR%"

python -m pip install opencv-python mediapipe -q 2>nul
if %errorlevel% neq 0 (
    echo [!] Erreur lors de l'installation
    echo [*] Installez manuellement: pip install opencv-python mediapipe
    pause
    goto menu
)

echo [✓] Dépendances OK
echo [*] Lancement du système de détection...
echo.

python main.py
goto menu

REM ================================================================================
REM LANCER DÉTECTION OPTIMISÉE
REM ================================================================================

:hand_detection_optimized
cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║    🖐️  DÉTECTION DE MAINS OPTIMISÉE ^(NEW^)                  ║
echo ╚════════════════════════════════════════════════════════════╝
echo.

where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [!] Erreur: Python n'est pas installé
    echo [*] Téléchargez Python: https://www.python.org/downloads/
    pause
    goto menu
)

echo [*] Installation des dépendances...
cd /d "%HAND_DIR%"

python -m pip install opencv-python mediapipe -q 2>nul

echo [*] Lancement avec le détecteur optimisé...
echo.

python -c "
import cv2
from hand_detector_optimized import HandDetectorOptimized
from gesture_recognizer_advanced import GestureRecognizerAdvanced

detector = HandDetectorOptimized(detection_confidence=0.7)
recognizer = GestureRecognizerAdvanced()

cap = cv2.VideoCapture(0)
cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
cap.set(cv2.CAP_PROP_FPS, 30)

print('\n🖐️  Système de détection optimisé lancé')
print('Appuyez sur q pour quitter\n')

while True:
    ret, frame = cap.read()
    if not ret: break
    
    hands, annotated = detector.detect(frame)
    annotated = detector.draw_performance_info(annotated)
    
    for i, hand in enumerate(hands):
        gesture_code, gesture_text, confidence = GestureRecognizerAdvanced.recognize(hand)
        cv2.putText(annotated, f'Geste: {gesture_text} ({confidence:.0%})', (10, 90 + i*30), cv2.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
    
    cv2.imshow('Détection de Mains', annotated)
    if cv2.waitKey(1) & 0xFF == ord('q'): break

cap.release()
cv2.destroyAllWindows()
"

goto menu

REM ================================================================================
REM SYSTÈME COMBINÉ
REM ================================================================================

:combined
cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║    🔗 SYSTÈME COMBINÉ: DÉTECTION + INTERFACE               ║
echo ╚════════════════════════════════════════════════════════════╝
echo.
echo Cette fonction demande une caméra connectée
echo.
echo   1) Lancer la détection optimisée d'abord
echo   2) Lancer l'interface d'abord
echo   0) Retour
echo.
set /p subchoice="Votre choix: "

if "!subchoice!"=="1" (
    start cmd /k "cd /d %HAND_DIR% && python -c ..."
    timeout /t 2 /nobreak
    call :gui
) else if "!subchoice!"=="2" (
    call :gui
    start cmd /k "cd /d %HAND_DIR% && python main.py"
)

goto menu

REM ================================================================================
REM AIDE ET CONFIGURATION
REM ================================================================================

:help
cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║                      📖 AIDE                               ║
echo ╚════════════════════════════════════════════════════════════╝
echo.
echo INSTALLATION REQUISE:
echo.
echo 1) Python 3.7+ (https://www.python.org/downloads/)
echo    Cochez "Add Python to PATH" lors de l'installation
echo.
echo 2) Dépendances Python:
echo    pip install opencv-python mediapipe
echo.
echo 3) Compilateur C++ ^(pour compiler l'interface^):
echo    Visual Studio Community ^(https://visualstudio.microsoft.com/downloads/^)
echo    OU MinGW ^(https://mingw-w64.org/^)
echo.
echo FICHIERS IMPORTANTS:
echo.
echo   - gui\agent_manager_unified.exe       ^(Interface^)
echo   - hand_detection_system\main.py       ^(Détection^)
echo   - launcher.bat                         ^(Ce menu^)
echo.
echo DÉPANNAGE:
echo.
echo   Q: "Python n'est pas reconnu"
echo   R: Réinstallez Python en cochant "Add to PATH"
echo.
echo   Q: "Erreur de dépendances"
echo   R: Ouvrez CMD et tapez:
echo      pip install --upgrade opencv-python mediapipe
echo.
echo   Q: "Caméra non trouvée"
echo   R: Vérifiez les paramètres de caméra dans Paramètres Windows
echo.
echo.
pause
goto menu

REM ================================================================================
REM EXPLORATEUR DE FICHIERS
REM ================================================================================

:explorer
cls
echo.
echo Ouverture de l'explorateur...
start "" "%PROJECT_ROOT%"
timeout /t 1 /nobreak
goto menu

REM ================================================================================
REM QUITTER
REM ================================================================================

:quit
cls
echo.
echo ╔════════════════════════════════════════════════════════════╗
echo ║              ✓ AU REVOIR!                                 ║
echo ╚════════════════════════════════════════════════════════════╝
echo.
exit /b 0
