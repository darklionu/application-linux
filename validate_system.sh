#!/bin/bash

###############################################################################
# VALIDATION FINALE - Vérifier que tout fonctionne
###############################################################################

echo "╔════════════════════════════════════════════════════════╗"
echo "║           VALIDATION SYSTÈME COMPLET                   ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

test_count=0
pass_count=0
fail_count=0

test_item() {
    local name=$1
    local cmd=$2
    ((test_count++))
    
    echo -n "[Test $test_count] $name... "
    if eval "$cmd" > /dev/null 2>&1; then
        echo -e "${GREEN}✓ PASS${NC}"
        ((pass_count++))
    else
        echo -e "${RED}✗ FAIL${NC}"
        ((fail_count++))
    fi
}

echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "1. Vérifications de Base"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "Répertoire agent_red_team existe" \
    "test -d /workspaces/application-linux/agent_red_team"

test_item "Exécutable build/attack_system existe" \
    "test -f /workspaces/application-linux/agent_red_team/build/attack_system"

test_item "Exécutable est accessible" \
    "test -x /workspaces/application-linux/agent_red_team/build/attack_system"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "2. Vérifications de Fichiers Source"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "main_attack_system.c existe" \
    "test -f /workspaces/application-linux/agent_red_team/main_attack_system.c"

test_item "physical_target.h existe" \
    "test -f /workspaces/application-linux/agent_red_team/physical_target.h"

test_item "real_attack_systems.h existe" \
    "test -f /workspaces/application-linux/agent_red_team/real_attack_systems.h"

test_item "real_port_scanner.h existe" \
    "test -f /workspaces/application-linux/agent_red_team/real_port_scanner.h"

test_item "agent_manager_secure.h existe" \
    "test -f /workspaces/application-linux/agent_red_team/agent_manager_secure.h"

test_item "tool_manager.c existe" \
    "test -f /workspaces/application-linux/agent_red_team/tool_manager.c"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "3. Vérifications de Documentation"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "LANCER_TESTS.md existe" \
    "test -f /workspaces/application-linux/LANCER_TESTS.md"

test_item "GUIDE_UTILISATION.md existe" \
    "test -f /workspaces/application-linux/GUIDE_UTILISATION.md"

test_item "INTEGRATION_COMPLETE.md existe" \
    "test -f /workspaces/application-linux/INTEGRATION_COMPLETE.md"

test_item "FINAL_SUMMARY.md existe" \
    "test -f /workspaces/application-linux/FINAL_SUMMARY.md"

test_item "INDEX_COMPLET.md existe" \
    "test -f /workspaces/application-linux/INDEX_COMPLET.md"

test_item "test_demo.sh existe et est exécutable" \
    "test -x /workspaces/application-linux/test_demo.sh"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "4. Vérifications de Compilation"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "Makefile_attack_system existe" \
    "test -f /workspaces/application-linux/agent_red_team/Makefile_attack_system"

test_item "Exécutable a la bonne taille (>100KB)" \
    "test $(stat -f%z /workspaces/application-linux/agent_red_team/build/attack_system 2>/dev/null || stat -c%s /workspaces/application-linux/agent_red_team/build/attack_system) -gt 100000"

test_item "Exécutable est un ELF 64-bit" \
    "file /workspaces/application-linux/agent_red_team/build/attack_system | grep -q 'ELF 64-bit'"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "5. Vérifications Fonctionnelles"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "Exécutable affiche le menu" \
    "timeout 3s /workspaces/application-linux/agent_red_team/build/attack_system 2>&1 | grep -q 'SYSTÈME D.AGENTS ATTAQUANTS'"

test_item "Menu affiche option DoS" \
    "timeout 3s /workspaces/application-linux/agent_red_team/build/attack_system 2>&1 | grep -q 'DoS/DDoS'"

test_item "Cibles sont initialisées" \
    "timeout 3s /workspaces/application-linux/agent_red_team/build/attack_system 2>&1 | grep -q 'Serveur Web 1'"

test_item "Agents sont initialisés" \
    "timeout 3s /workspaces/application-linux/agent_red_team/build/attack_system 2>&1 | grep -q 'Red Recon Agent'"

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo "6. Vérifications d'Outils"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

test_item "curl disponible" \
    "which curl > /dev/null"

test_item "bash disponible" \
    "which bash > /dev/null"

test_item "timeout disponible" \
    "which timeout > /dev/null"

test_item "grep disponible" \
    "which grep > /dev/null"

# Optional tools
echo ""
echo "[Info] Outils optionnels (non critiques):"
which nmap > /dev/null 2>&1 && echo "    ✓ nmap disponible" || echo "    ✗ nmap absent (fallback utilisé)"
which sshpass > /dev/null 2>&1 && echo "    ✓ sshpass disponible" || echo "    ✗ sshpass absent (fallback utilisé)"
which hping3 > /dev/null 2>&1 && echo "    ✓ hping3 disponible" || echo "    ✗ hping3 absent (fallback utilisé)"
which nc > /dev/null 2>&1 && echo "    ✓ nc disponible" || echo "    ✗ nc absent (fallback utilisé)"
which mysql > /dev/null 2>&1 && echo "    ✓ mysql disponible" || echo "    ✗ mysql absent (fallback utilisé)"
which psql > /dev/null 2>&1 && echo "    ✓ psql disponible" || echo "    ✗ psql absent (fallback utilisé)"

echo ""
echo "╔════════════════════════════════════════════════════════╗"
echo "║                 RÉSULTATS FINAUX                       ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

echo "Total des tests: $test_count"
echo -e "Réussis: ${GREEN}$pass_count${NC}"
echo -e "Échoués: ${RED}$fail_count${NC}"

if [ $fail_count -eq 0 ]; then
    echo ""
    echo -e "${GREEN}✓ TOUS LES TESTS RÉUSSIS!${NC}"
    echo ""
    echo "Le système est prêt à l'emploi."
    echo ""
    echo "Pour lancer:"
    echo "  1. Test rapide:"
    echo "     echo '4' | timeout 15s /workspaces/application-linux/agent_red_team/build/attack_system"
    echo ""
    echo "  2. Démonstration interactive:"
    echo "     bash /workspaces/application-linux/test_demo.sh"
    echo ""
    echo "  3. Lancer le programme:"
    echo "     cd /workspaces/application-linux/agent_red_team"
    echo "     ./build/attack_system"
    echo ""
    exit 0
else
    echo ""
    echo -e "${RED}✗ CERTAINS TESTS ONT ÉCHOUÉ${NC}"
    echo ""
    echo "Vérifiez les problèmes ci-dessus"
    exit 1
fi
