#!/bin/bash
# Script de démarrage rapide du Système d'Agents Attaquants v2.0
# Corrige tous les bugs et ajoute les fonctionnalités d'attaque physique

set -e

echo "╔════════════════════════════════════════════════════════╗"
echo "║   SYSTÈME D'AGENTS ATTAQUANTS - GUIDE DE DÉMARRAGE   ║"
echo "║              VERSION 2.0 - BUGS FIXÉS                ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Vérifier le répertoire
if [ ! -d "agent_red_team" ]; then
    echo "[!] Erreur: Exécutez ce script depuis le répertoire racine du projet"
    exit 1
fi

echo "[1/4] Nettoyage des anciens fichiers compilés..."
cd agent_red_team
make -f Makefile_attack_system clean > /dev/null 2>&1 || true
echo "[OK] Nettoyage terminé"

echo ""
echo "[2/4] Compilation du système..."
if make -f Makefile_attack_system all; then
    echo "[OK] Compilation réussie"
else
    echo "[!] Erreur lors de la compilation"
    exit 1
fi

echo ""
echo "[3/4] Vérification de l'exécutable..."
if [ -f "build/attack_system" ]; then
    echo "[OK] Exécutable trouvé: build/attack_system"
    ls -lh build/attack_system
else
    echo "[!] Erreur: Exécutable non trouvé"
    exit 1
fi

echo ""
echo "[4/4] Lancement du système..."
echo ""
echo "╔════════════════════════════════════════════╗"
echo "║          MENU PRINCIPAL DISPONIBLE         ║"
echo "╠════════════════════════════════════════════╣"
echo "║ 1. Reconnaissance & Scan                  ║"
echo "║ 2. Exploitation de vulnérabilités        ║"
echo "║ 3. Déploiement de malware                ║"
echo "║ 4. Attaque DoS/DDoS                      ║"
echo "║ 5. Chaîne d'attaque complète             ║"
echo "║ 6. Démonstration attaques physiques      ║"
echo "║ 7. Voir l'état des agents                ║"
echo "║ 8. Voir l'état des cibles                ║"
echo "║ 9. Voir l'historique des attaques        ║"
echo "║ 10. Mode interactif                      ║"
echo "║ 0. Quitter                               ║"
echo "╚════════════════════════════════════════════╝"
echo ""

./build/attack_system
