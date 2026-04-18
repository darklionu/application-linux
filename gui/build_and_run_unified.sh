#!/bin/bash

# Script de démarrage unifié du système GUI
# Ce script compile et lance l'interface graphique unifiée

set -e

PROJECT_DIR="/workspaces/application-linux"
GUI_DIR="$PROJECT_DIR/gui"
BUILD_DIR="$GUI_DIR/build"

echo "╔════════════════════════════════════════════════════════════╗"
echo "║         SYSTÈME GUI UNIFIÉ - SCRIPT DE DÉMARRAGE           ║"
echo "║              Gestionnaire d'Agents Centralisé              ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Créer le répertoire de build s'il n'existe pas
if [ ! -d "$BUILD_DIR" ]; then
    echo "[*] Création du répertoire de build..."
    mkdir -p "$BUILD_DIR"
fi

# Vérifier que g++ est installé
if ! command -v g++ &> /dev/null; then
    echo "[!] Erreur: g++ n'est pas installé"
    echo "[*] Installation: apt-get install build-essential"
    exit 1
fi

cd "$GUI_DIR"

echo "[*] Compilation du système GUI unifié..."
echo ""

# Compiler le système GUI unifié
if g++ -std=c++11 -Wall -Wextra -O2 -o "$BUILD_DIR/agent_manager_gui_unified" src/unified_gui_system.cpp; then
    echo ""
    echo "[✓] Compilation réussie!"
    echo "[✓] Exécutable: $BUILD_DIR/agent_manager_gui_unified"
    echo ""
else
    echo ""
    echo "[!] Erreur lors de la compilation"
    exit 1
fi

# Optionnel: Compiler aussi le programme d'intégration C
echo ""
echo "[*] Compilation du programme d'intégration (optional)..."

if gcc -o "$BUILD_DIR/agent_executor_integration" agent_executor_integration.c -Wall -Wextra; then
    echo "[✓] Programme d'intégration compilé"
    echo "[✓] Exécutable: $BUILD_DIR/agent_executor_integration"
    echo ""
else
    echo "[!] Erreur lors de la compilation du programme d'intégration (non-critique)"
fi

echo "╔════════════════════════════════════════════════════════════╗"
echo "║                 COMPILATION RÉUSSIE                        ║"
echo "╠════════════════════════════════════════════════════════════╣"
echo "║                                                            ║"
echo "║  Exécutables disponibles:                                  ║"
echo "║  1. $BUILD_DIR/agent_manager_gui_unified"
echo "║  2. $BUILD_DIR/agent_executor_integration (optionnel)      ║"
echo "║                                                            ║"
echo "║  Pour lancer l'interface:                                  ║"
echo "║  $ $BUILD_DIR/agent_manager_gui_unified                    ║"
echo "║                                                            ║"
echo "║  Pour tester l'intégration:                                ║"
echo "║  $ $BUILD_DIR/agent_executor_integration                   ║"
echo "║                                                            ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

# Demander si l'utilisateur veut lancer l'interface
echo -n "Voulez-vous lancer l'interface maintenant? (y/n): "
read -r response

if [ "$response" = "y" ] || [ "$response" = "Y" ]; then
    echo ""
    echo "[*] Lancement du système GUI unifié..."
    echo ""
    "$BUILD_DIR/agent_manager_gui_unified"
else
    echo ""
    echo "[*] Pour lancer plus tard:"
    echo "    $ $BUILD_DIR/agent_manager_gui_unified"
    echo ""
fi
