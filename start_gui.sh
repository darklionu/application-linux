#!/bin/bash

# 🚀 DÉMARRAGE RAPIDE - INTERFACE GRAPHIQUE
# Script ultra-simplifié pour lancer l'interface en 1 commande

PROJECT_DIR="/workspaces/application-linux"
GUI_DIR="$PROJECT_DIR/gui"
BUILD_DIR="$GUI_DIR/build"

mkdir -p "$BUILD_DIR"

echo "🔨 Compilation de l'interface graphique..."
cd "$GUI_DIR"

if g++ -std=c++11 -O2 -o "$BUILD_DIR/agent_manager_gui_unified" src/unified_gui_system.cpp 2>/dev/null; then
    echo "✓ Compilation réussie!"
    echo "🚀 Lancement de l'interface..."
    "$BUILD_DIR/agent_manager_gui_unified"
else
    echo "✗ Erreur de compilation"
    echo "Essai avec plus de dépendances..."
    g++ -std=c++11 -Wall -Wextra -O2 -o "$BUILD_DIR/agent_manager_gui_unified" src/unified_gui_system.cpp
fi
