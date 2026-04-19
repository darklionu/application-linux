#!/bin/bash

# 🖐️ DÉMARRAGE RAPIDE - DÉTECTION DE MAINS
# Script ultra-simplifié pour lancer la détection

PROJECT_DIR="/workspaces/application-linux"
HAND_DIR="$PROJECT_DIR/hand_detection_system"

cd "$HAND_DIR"

echo "📦 Vérification des dépendances Python..."

if ! python3 -c "import cv2, mediapipe" 2>/dev/null; then
    echo "⚙️ Installation des dépendances..."
    pip3 install -q opencv-python mediapipe
fi

echo "✓ Dépendances OK"
echo "🖐️ Lancement du système de détection de mains..."
echo ""
echo "📌 Commandes:"
echo "  - Pointez une main vers la caméra"
echo "  - 'q' pour quitter"
echo ""

# Essayer le script optimisé d'abord
if [ -f "test_optimized.py" ]; then
    python3 test_optimized.py
else
    # Sinon utiliser le script par défaut
    python3 main.py
fi
