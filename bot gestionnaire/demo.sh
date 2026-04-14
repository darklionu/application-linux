#!/bin/bash

# Script de démonstration du Bot Gestionnaire Agent

echo "╔═════════════════════════════════════════════════════╗"
echo "║     DÉMO - Bot Gestionnaire Agent v1.0              ║"
echo "╚═════════════════════════════════════════════════════╝"
echo ""

# Vérifier le répertoire
if [ ! -f "bot_gestionnaire" ]; then
    echo "[!] Erreur: bot_gestionnaire non compilé!"
    echo "[*] Exécutez: make"
    exit 1
fi

echo "[*] Démarrage du programme..."
echo ""
echo "Instructions de démonstration:"
echo "1. Au premier menu, sélectionnez '3' pour gérer les outils"
echo "2. Activez les outils nmap et john"
echo "3. Sélectionnez '2' pour exécuter une technique"
echo "4. Sélectionnez '4' pour le mode automatique (10 itérations)"
echo ""
echo "Démarrage du programme..."
echo "══════════════════════════════════════════════════════"
echo ""

./bot_gestionnaire
