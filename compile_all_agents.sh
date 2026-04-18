#!/bin/bash

################################################################################
# Script de compilation de tous les agents
# Compile les agents Red Team et Blue Team
################################################################################

set -e

# COULEURS
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_header() {
    echo -e "${BLUE}═══════════════════════════════════════════${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}═══════════════════════════════════════════${NC}"
}

print_success() {
    echo -e "${GREEN}[✓]${NC} $1"
}

print_error() {
    echo -e "${RED}[✗]${NC} $1"
}

print_info() {
    echo -e "${YELLOW}[*]${NC} $1"
}

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

################################################################################
# Compiler les agents
################################################################################

compile_red_agents() {
    print_header "COMPILATION DES AGENTS RED TEAM"
    
    # Red Recon Agent
    print_info "Compilation du Red Recon Agent..."
    cd "$SCRIPT_DIR/agent/red_recon_agent"
    if make; then
        print_success "Red Recon Agent compilé"
    else
        print_error "Erreur compilation Red Recon Agent"
        return 1
    fi
    
    # Red Exploit Agent
    print_info "Compilation du Red Exploit Agent..."
    cd "$SCRIPT_DIR/agent/red_exploit_agent"
    if make; then
        print_success "Red Exploit Agent compilé"
    else
        print_error "Erreur compilation Red Exploit Agent"
        return 1
    fi
    
    # Red BlackHat Agent
    print_info "Compilation du Red BlackHat Agent..."
    cd "$SCRIPT_DIR/agent/red_blackhat_agent"
    if make; then
        print_success "Red BlackHat Agent compilé"
    else
        print_error "Erreur compilation Red BlackHat Agent"
        return 1
    fi
}

compile_blue_agents() {
    print_header "COMPILATION DES AGENTS BLUE TEAM"
    
    # Blue Monitor Agent
    print_info "Compilation du Blue Monitor Agent..."
    if [ -d "$SCRIPT_DIR/agent_blue_team/monitor_agent" ]; then
        cd "$SCRIPT_DIR/agent_blue_team/monitor_agent"
        if [ -f "Makefile" ]; then
            if make; then
                print_success "Blue Monitor Agent compilé"
            else
                print_error "Erreur compilation Blue Monitor Agent"
                return 1
            fi
        else
            print_info "Pas de Makefile pour Blue Monitor Agent"
        fi
    fi
    
    # Blue Response Agent
    print_info "Compilation du Blue Response Agent..."
    if [ -d "$SCRIPT_DIR/agent_blue_team/response_agent" ]; then
        cd "$SCRIPT_DIR/agent_blue_team/response_agent"
        if [ -f "Makefile" ]; then
            if make; then
                print_success "Blue Response Agent compilé"
            else
                print_error "Erreur compilation Blue Response Agent"
                return 1
            fi
        else
            print_info "Pas de Makefile pour Blue Response Agent"
        fi
    fi
}

################################################################################
# Vérification des exécutables
################################################################################

verify_agents() {
    print_header "VÉRIFICATION DES AGENTS"
    
    local all_ok=true
    
    if [ -f "$SCRIPT_DIR/agent/red_recon_agent/red_recon_agent" ]; then
        print_success "Red Recon Agent"
    else
        print_error "Red Recon Agent manquant"
        all_ok=false
    fi
    
    if [ -f "$SCRIPT_DIR/agent/red_exploit_agent/red_exploit_agent" ]; then
        print_success "Red Exploit Agent"
    else
        print_error "Red Exploit Agent manquant"
        all_ok=false
    fi
    
    if [ -f "$SCRIPT_DIR/agent/red_blackhat_agent/red_blackhat_agent" ]; then
        print_success "Red BlackHat Agent"
    else
        print_error "Red BlackHat Agent manquant"
        all_ok=false
    fi
    
    if [ -f "$SCRIPT_DIR/agent_blue_team/monitor_agent/blue_monitor_agent" ]; then
        print_success "Blue Monitor Agent"
    else
        print_error "Blue Monitor Agent manquant"
        all_ok=false
    fi
    
    if [ -f "$SCRIPT_DIR/agent_blue_team/response_agent/blue_response_agent" ]; then
        print_success "Blue Response Agent"
    else
        print_error "Blue Response Agent manquant"
        all_ok=false
    fi
    
    return $all_ok
}

################################################################################
# Main
################################################################################

main() {
    cd "$SCRIPT_DIR"
    
    echo ""
    print_header "COMPILATION DE TOUS LES AGENTS"
    echo ""
    
    compile_red_agents || exit 1
    echo ""
    
    compile_blue_agents || exit 1
    echo ""
    
    if verify_agents; then
        echo ""
        print_success "Tous les agents sont compilés et prêts!"
        echo ""
    else
        echo ""
        print_error "Certains agents ne sont pas disponibles"
        exit 1
    fi
}

main "$@"
