#!/bin/bash

###############################################################################
# Script d'Installation - Système de Détection de Mains
###############################################################################

echo "╔════════════════════════════════════════════════════════╗"
echo "║    Installation - Système de Détection de Mains      ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Vérifier Python
if ! command -v python3 &> /dev/null; then
    echo "❌ Python3 est requis"
    exit 1
fi

echo "✓ Python3 trouvé"

# Créer l'environnement virtuel
echo ""
echo "Création de l'environnement virtuel..."
python3 -m venv venv

# Activer l'environnement
source venv/bin/activate

# Mettre à jour pip
echo "Mise à jour de pip..."
pip install --upgrade pip

# Installer les dépendances
echo ""
echo "Installation des dépendances..."
pip install -r requirements.txt

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║          ✓ Installation complétée                     ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""
echo "Pour lancer le système:"
echo "  1. Activer l'environnement: source venv/bin/activate"
echo "  2. Lancer le programme: python3 main.py"
echo ""
