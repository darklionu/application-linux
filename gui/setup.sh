#!/bin/bash
# Script de setup pour la GUI

echo "🎨 Configuration de l'interface graphique..."
echo ""

# Vérifier et installer les dépendances
echo "[1/4] Vérification des dépendances..."

if ! command -v g++ &> /dev/null; then
    echo "❌ g++ introuvable. Installation..."
    sudo apt-get update
    sudo apt-get install -y build-essential
fi

if ! pkg-config --exists sfml-all 2>/dev/null; then
    echo "⚠️  SFML non installé. Installation..."
    sudo apt-get install -y libsfml-dev
else
    echo "✅ SFML trouvé"
fi

echo ""
echo "[2/4] Création de la structure..."
mkdir -p src include assets build
echo "✅ Dossiers créés"

echo ""
echo "[3/4] Compilation de la bibliothèque GUI..."
make clean
make all

echo ""
echo "[4/4] Finalisation..."
echo "✅ Configuration complète!"

echo ""
echo "📚 Prochaines étapes:"
echo "1. Éditer gui/README.md pour choisir le framework"
echo "2. Implémenter la fenêtre principale"
echo "3. Ajouter les événements et interactions"
echo "4. Intégrer avec l'application principale"
echo ""
echo "Commande pour démarrer:"
echo "  make -C gui/ all"
echo "  cd gui && make help"
