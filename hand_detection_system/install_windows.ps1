# Installation pour Windows PowerShell - Détection de Mains
# Exécuter avec: powershell -ExecutionPolicy Bypass -File install_windows.ps1

Write-Host ""
Write-Host "╔════════════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║                                                                ║" -ForegroundColor Green
Write-Host "║   🎥 INSTALLATION WINDOWS - DÉTECTION DE MAINS 🎥            ║" -ForegroundColor Green
Write-Host "║                                                                ║" -ForegroundColor Green
Write-Host "║          Installation Automatique pour Windows 10/11          ║" -ForegroundColor Green
Write-Host "║                                                                ║" -ForegroundColor Green
Write-Host "╚════════════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""

# Vérifier Python
Write-Host "[1/4] Vérification de Python..." -ForegroundColor Cyan
try {
    $pythonVersion = python --version 2>&1
    Write-Host "✅ Python trouvé: $pythonVersion" -ForegroundColor Green
} catch {
    Write-Host "❌ Python n'est pas installé" -ForegroundColor Red
    Write-Host ""
    Write-Host "Installez Python depuis: https://www.python.org/downloads/" -ForegroundColor Yellow
    Write-Host "Assurez-vous de cocher 'Add Python to PATH'" -ForegroundColor Yellow
    Read-Host "Appuyer sur Entrée pour terminer"
    exit 1
}

# Mettre à jour pip
Write-Host ""
Write-Host "[2/4] Mise à jour de pip..." -ForegroundColor Cyan
python -m pip install --upgrade pip
if ($LASTEXITCODE -ne 0) {
    Write-Host "⚠️  Erreur lors de la mise à jour de pip" -ForegroundColor Yellow
}
Write-Host "✅ pip mis à jour" -ForegroundColor Green

# Installer OpenCV
Write-Host ""
Write-Host "[3/4] Installation d'OpenCV (pour Windows)..." -ForegroundColor Cyan
python -m pip install opencv-python
if ($LASTEXITCODE -ne 0) {
    Write-Host "❌ Erreur lors de l'installation d'OpenCV" -ForegroundColor Red
    Read-Host "Appuyer sur Entrée pour terminer"
    exit 1
}
Write-Host "✅ OpenCV installé" -ForegroundColor Green

# Installer MediaPipe
Write-Host ""
Write-Host "[4/4] Installation de MediaPipe..." -ForegroundColor Cyan
python -m pip install mediapipe
if ($LASTEXITCODE -ne 0) {
    Write-Host "⚠️  Erreur lors de l'installation de MediaPipe" -ForegroundColor Yellow
}
Write-Host "✅ MediaPipe installé" -ForegroundColor Green

# Terminer
Write-Host ""
Write-Host "╔════════════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║                                                                ║" -ForegroundColor Green
Write-Host "║                ✅ INSTALLATION TERMINÉE ✅                    ║" -ForegroundColor Green
Write-Host "║                                                                ║" -ForegroundColor Green
Write-Host "║                 Vous pouvez maintenant lancer:                ║" -ForegroundColor Green
Write-Host "║                                                                ║" -ForegroundColor Green
Write-Host "║                  python main_windows.py                       ║" -ForegroundColor Green
Write-Host "║                                                                ║" -ForegroundColor Green
Write-Host "╚════════════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""

Read-Host "Appuyer sur Entrée pour terminer"
