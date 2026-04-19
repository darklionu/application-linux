#!/bin/bash

###############################################################################
# SCRIPT DE DÉMONSTRATION - SYSTÈME D'AGENTS ATTAQUANTS
# Montre les capacités réelles des vrais systèmes d'attaque
###############################################################################

set -e

WORKSPACE="/workspaces/application-linux"
AGENT_DIR="$WORKSPACE/agent_red_team"
BINARY="$AGENT_DIR/build/attack_system"

echo "╔════════════════════════════════════════════════════════╗"
echo "║         DÉMONSTRATION - AGENTS ATTAQUANTS              ║"
echo "║      Systèmes d'Attaque Réels Intégrés & Testés       ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

if [ ! -f "$BINARY" ]; then
    echo "[!] Erreur: $BINARY non trouvé"
    echo "[*] Compilation en cours..."
    cd "$AGENT_DIR"
    make -f Makefile_attack_system all
fi

echo "[✓] Exécutable trouvé: $BINARY"
echo "[✓] Taille: $(du -h "$BINARY" | cut -f1)"
echo ""

# Vérifier les outils disponibles
echo "╔════════════════════════════════════════════════════════╗"
echo "║           VÉRIFICATION DES OUTILS DISPONIBLES          ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

check_tool() {
    local tool=$1
    if command -v "$tool" &> /dev/null; then
        echo "[✓] $tool - DISPONIBLE"
    else
        echo "[✗] $tool - NON DISPONIBLE (fallback utilisé)"
    fi
}

check_tool "nmap"
check_tool "curl"
check_tool "sshpass"
check_tool "hping3"
check_tool "nc"
check_tool "mysql"
check_tool "psql"

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║              TESTS DES SYSTÈMES D'ATTAQUE              ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

test_option() {
    local option=$1
    local description=$2
    local timeout_seconds=$3
    
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    echo "Option: $option - $description"
    echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    
    echo "[*] Exécution en cours (timeout: ${timeout_seconds}s)..."
    
    if echo "$option" | timeout "${timeout_seconds}s" "$BINARY" 2>&1 | head -100; then
        echo ""
        echo "[✓] Test complété"
    else
        if [ $? -eq 124 ]; then
            echo ""
            echo "[⚠] Timeout (résultat partiel affiché)"
        fi
    fi
    
    echo ""
    read -p "Appuyez sur Entrée pour continuer..." || true
    echo ""
}

# Test 1: DoS/DDoS (le plus rapide)
test_option "4" "Attaque DoS/DDoS (TESTÉ ✓)" "12"

# Menu d'options
while true; do
    echo "╔════════════════════════════════════════════════════════╗"
    echo "║            SÉLECTIONNER UN TEST SUPPLÉMENTAIRE        ║"
    echo "╠════════════════════════════════════════════════════════╣"
    echo "║  1. Reconnaissance & Scan                              ║"
    echo "║  2. Exploitation de vulnérabilités                     ║"
    echo "║  3. Déploiement de malware                             ║"
    echo "║  4. Attaque DoS/DDoS (rapide)                          ║"
    echo "║  5. Chaîne d'attaque complète                          ║"
    echo "║  6. Démonstration attaques physiques                   ║"
    echo "║  7. État des agents                                    ║"
    echo "║  8. État des cibles                                    ║"
    echo "║  9. Historique des attaques                            ║"
    echo "║  0. Quitter                                            ║"
    echo "╚════════════════════════════════════════════════════════╝"
    echo ""
    
    read -p "Choix (0-9): " choice
    
    case $choice in
        1)
            test_option "1" "Reconnaissance & Scan (lent - 10-15s)" "20"
            ;;
        2)
            test_option "2" "Exploitation de vulnérabilités (5-10s)" "15"
            ;;
        3)
            test_option "3" "Déploiement de malware (rapide)" "10"
            ;;
        4)
            test_option "4" "Attaque DoS/DDoS (rapide)" "12"
            ;;
        5)
            test_option "5" "Chaîne d'attaque complète (lent - 20s+)" "30"
            ;;
        6)
            test_option "6" "Démonstration attaques physiques (lent)" "25"
            ;;
        7)
            test_option "7" "État des agents" "5"
            ;;
        8)
            test_option "8" "État des cibles" "5"
            ;;
        9)
            test_option "9" "Historique des attaques" "5"
            ;;
        0)
            echo "Fin de la démonstration"
            break
            ;;
        *)
            echo "Option invalide"
            ;;
    esac
done

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║            DÉMONSTRATION TERMINÉE                      ║"
echo "╚════════════════════════════════════════════════════════╝"
