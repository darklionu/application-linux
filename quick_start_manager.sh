#!/bin/bash

################################################################################
# GESTIONNAIRE D'AGENTS INTERACTIF
# Lance réellement les agents avec sélection des cibles et attaques
################################################################################

set -e

# COULEURS
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
MAGENTA='\033[0;35m'
NC='\033[0m' # No Color

# VARIABLES GLOBALES
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
TARGETS_FILE="$SCRIPT_DIR/.targets.conf"
ATTACKS_FILE="$SCRIPT_DIR/.attacks.conf"
AGENTS_LOG="$SCRIPT_DIR/agents_execution.log"
SELECTED_TARGETS=()
SELECTED_ATTACKS=()

################################################################################
# FONCTIONS UTILITAIRES
################################################################################

print_header() {
    echo -e "${BLUE}════════════════════════════════════════════════════${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}════════════════════════════════════════════════════${NC}"
}

print_success() {
    echo -e "${GREEN}[✓]${NC} $1"
}

print_error() {
    echo -e "${RED}[✗]${NC} $1"
}

print_info() {
    echo -e "${CYAN}[*]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[!]${NC} $1"
}

print_agent() {
    echo -e "${MAGENTA}[Agent]${NC} $1"
}

# Initialiser les fichiers de configuration
init_config_files() {
    if [ ! -f "$TARGETS_FILE" ]; then
        cat > "$TARGETS_FILE" << 'EOF'
# Fichier de configuration des cibles
# Format: NOM_CIBLE|ADRESSE|DESCRIPTION

localhost|127.0.0.1|Machine locale
gateway|192.168.1.1|Passerelle réseau
webserver|192.168.1.100|Serveur web
database|192.168.1.50|Serveur base de données
workstation|192.168.1.25|Poste de travail
nas|192.168.1.200|Serveur NAS
DNS|8.8.8.8|Serveur DNS externe
SSH|192.168.1.30|Serveur SSH
EOF
    fi

    if [ ! -f "$ATTACKS_FILE" ]; then
        cat > "$ATTACKS_FILE" << 'EOF'
# Fichier de configuration des attaques disponibles
# Format: NOM_ATTAQUE|OUTIL|DESCRIPTION

Ping Test|nmap|Test de ping simple
DNS Lookup|nmap|Recherche DNS
Network Recon|nmap|Reconnaissance réseau
Banner Grab|nmap|Récupération de bannière
Asset Discovery|outils|Découverte d'actifs
Port Scan|nmap|Scan de ports
Brute Force SSH|john|Force brute SSH
Brute Force Web|hdra|Force brute Web
SQL Injection|hdra|Injection SQL
XSS Attack|hdra|Attaque XSS
DDoS Simulation|outils|Simulation DDoS
Vulnerability Scan|nmap|Scan de vulnérabilités
Malware Simulation|outils|Simulation malware
Privilege Escalation|hdra|Escalade de privilèges
Data Exfiltration|outils|Exfiltration de données
EOF
    fi
}

################################################################################
# MENU INTERACTIF - SÉLECTION DES CIBLES
################################################################################

select_targets() {
    print_header "SÉLECTION DES CIBLES"
    
    local -a targets=()
    local line_num=1
    
    # Lire les cibles depuis le fichier
    while IFS='|' read -r name ip description; do
        # Ignorer les commentaires et les lignes vides
        [[ "$name" =~ ^#.*$ ]] && continue
        [[ -z "$name" ]] && continue
        
        targets+=("$name" "$ip" "$description")
        printf "${CYAN}%2d)${NC} %-15s %-15s %s\n" "$line_num" "$name" "$ip" "$description"
        ((line_num++))
    done < "$TARGETS_FILE"
    
    echo ""
    echo -e "${YELLOW}Options:${NC}"
    echo "  A) Sélectionner TOUS les cibles"
    echo "  C) Personnalisé (entrez les numéros séparés par des espaces)"
    echo "  Q) Quitter"
    echo ""
    
    read -p "Votre choix: " choice
    
    case "$choice" in
        A|a)
            SELECTED_TARGETS=("${targets[@]}")
            print_success "Toutes les cibles sélectionnées: ${#targets[@]} cibles"
            ;;
        C|c)
            read -p "Entrez les numéros des cibles (ex: 1 2 3): " target_nums
            for num in $target_nums; do
                idx=$(( (num - 1) * 3 ))
                if [ $idx -ge 0 ] && [ $idx -lt ${#targets[@]} ]; then
                    SELECTED_TARGETS+=("${targets[$idx]}")
                fi
            done
            print_success "${#SELECTED_TARGETS[@]} cible(s) sélectionnée(s)"
            ;;
        Q|q)
            print_info "Annulation..."
            exit 0
            ;;
        *)
            print_error "Choix invalide"
            select_targets
            ;;
    esac
}

################################################################################
# MENU INTERACTIF - SÉLECTION DES ATTAQUES
################################################################################

select_attacks() {
    print_header "SÉLECTION DES ATTAQUES"
    
    local -a attacks=()
    local line_num=1
    
    # Lire les attaques depuis le fichier
    while IFS='|' read -r name tool description; do
        # Ignorer les commentaires et les lignes vides
        [[ "$name" =~ ^#.*$ ]] && continue
        [[ -z "$name" ]] && continue
        
        attacks+=("$name" "$tool" "$description")
        printf "${CYAN}%2d)${NC} %-25s [%-8s] %s\n" "$line_num" "$name" "$tool" "$description"
        ((line_num++))
    done < "$ATTACKS_FILE"
    
    echo ""
    echo -e "${YELLOW}Options:${NC}"
    echo "  A) Sélectionner TOUTES les attaques"
    echo "  B) Attaques basiques (reconnaissance seulement)"
    echo "  M) Attaques moyennes (mix reconnaissance/attaque)"
    echo "  C) Personnalisé (entrez les numéros séparés par des espaces)"
    echo "  Q) Quitter"
    echo ""
    
    read -p "Votre choix: " choice
    
    case "$choice" in
        A|a)
            SELECTED_ATTACKS=("${attacks[@]}")
            print_success "Toutes les attaques sélectionnées"
            ;;
        B|b)
            # Attaques basiques (reconnaissance)
            for i in {0..4}; do
                SELECTED_ATTACKS+=("${attacks[$i]}")
            done
            print_success "Attaques basiques sélectionnées (reconnaissance)"
            ;;
        M|m)
            # Attaques moyennes (indices sélectionnés)
            for i in {0..7}; do
                if [ $i -lt ${#attacks[@]} ]; then
                    SELECTED_ATTACKS+=("${attacks[$i]}")
                fi
            done
            print_success "Attaques moyennes sélectionnées"
            ;;
        C|c)
            read -p "Entrez les numéros des attaques (ex: 1 3 5): " attack_nums
            for num in $attack_nums; do
                idx=$(( (num - 1) * 3 ))
                if [ $idx -ge 0 ] && [ $idx -lt ${#attacks[@]} ]; then
                    SELECTED_ATTACKS+=("${attacks[$idx]}")
                fi
            done
            print_success "${#SELECTED_ATTACKS[@]} attaque(s) sélectionnée(s)"
            ;;
        Q|q)
            print_info "Annulation..."
            exit 0
            ;;
        *)
            print_error "Choix invalide"
            select_attacks
            ;;
    esac
}

################################################################################
# LANCEMENT DES AGENTS RÉELS
################################################################################

execute_red_recon_agent() {
    local target="$1"
    local attack="$2"
    
    print_agent "Lancement Red Recon Agent sur $target avec attaque: $attack"
    
    if [ -f "$SCRIPT_DIR/agent/red_recon_agent/red_recon_agent" ]; then
        {
            echo "=== RED RECON AGENT ==="
            echo "Cible: $target"
            echo "Attaque: $attack"
            echo "Date: $(date)"
            echo "---"
            cd "$SCRIPT_DIR/agent/red_recon_agent" && \
            ln -sf "$SCRIPT_DIR/tools" tools 2>/dev/null || true && \
            ./red_recon_agent << 'INPUT'
1
1
0
INPUT
            echo "---"
        } | tee -a "$AGENTS_LOG"
    else
        print_warning "Red Recon Agent non trouvé, compilation..."
        compile_agent "red_recon"
        if [ -f "$SCRIPT_DIR/agent/red_recon_agent/red_recon_agent" ]; then
            execute_red_recon_agent "$target" "$attack"
        fi
    fi
}

execute_red_exploit_agent() {
    local target="$1"
    local attack="$2"
    
    print_agent "Lancement Red Exploit Agent sur $target avec attaque: $attack"
    
    if [ -f "$SCRIPT_DIR/agent/red_exploit_agent/red_exploit_agent" ]; then
        {
            echo "=== RED EXPLOIT AGENT ==="
            echo "Cible: $target"
            echo "Attaque: $attack"
            echo "Date: $(date)"
            echo "---"
            cd "$SCRIPT_DIR/agent/red_exploit_agent" && \
            ln -sf "$SCRIPT_DIR/tools" tools 2>/dev/null || true && \
            ./red_exploit_agent << 'INPUT'
1
1
0
INPUT
            echo "---"
        } | tee -a "$AGENTS_LOG"
    else
        print_warning "Red Exploit Agent non trouvé, compilation..."
        compile_agent "red_exploit"
        if [ -f "$SCRIPT_DIR/agent/red_exploit_agent/red_exploit_agent" ]; then
            execute_red_exploit_agent "$target" "$attack"
        fi
    fi
}

execute_red_blackhat_agent() {
    local target="$1"
    local attack="$2"
    
    print_agent "Lancement Red BlackHat Agent sur $target avec attaque: $attack"
    
    if [ -f "$SCRIPT_DIR/agent/red_blackhat_agent/red_blackhat_agent" ]; then
        {
            echo "=== RED BLACKHAT AGENT ==="
            echo "Cible: $target"
            echo "Attaque: $attack"
            echo "Date: $(date)"
            echo "---"
            cd "$SCRIPT_DIR/agent/red_blackhat_agent" && \
            ln -sf "$SCRIPT_DIR/tools" tools 2>/dev/null || true && \
            ./red_blackhat_agent << 'INPUT'
1
1
0
INPUT
            echo "---"
        } | tee -a "$AGENTS_LOG"
    else
        print_warning "Red BlackHat Agent non trouvé, compilation..."
        compile_agent "red_blackhat"
        if [ -f "$SCRIPT_DIR/agent/red_blackhat_agent/red_blackhat_agent" ]; then
            execute_red_blackhat_agent "$target" "$attack"
        fi
    fi
}

execute_blue_monitor_agent() {
    print_agent "Lancement Blue Monitor Agent (surveillance)"
    
    if [ -f "$SCRIPT_DIR/agent_blue_team/monitor_agent/blue_monitor_agent" ]; then
        {
            echo "=== BLUE MONITOR AGENT ==="
            echo "Rôle: Surveillance et détection"
            echo "Date: $(date)"
            echo "---"
            cd "$SCRIPT_DIR/agent_blue_team/monitor_agent" && ./blue_monitor_agent
            echo "---"
        } | tee -a "$AGENTS_LOG"
    else
        print_warning "Blue Monitor Agent non trouvé"
    fi
}

execute_blue_response_agent() {
    print_agent "Lancement Blue Response Agent (réponse)"
    
    if [ -f "$SCRIPT_DIR/agent_blue_team/response_agent/blue_response_agent" ]; then
        {
            echo "=== BLUE RESPONSE AGENT ==="
            echo "Rôle: Réponse aux incidents"
            echo "Date: $(date)"
            echo "---"
            cd "$SCRIPT_DIR/agent_blue_team/response_agent" && ./blue_response_agent
            echo "---"
        } | tee -a "$AGENTS_LOG"
    else
        print_warning "Blue Response Agent non trouvé"
    fi
}

################################################################################
# COMPILATION DES AGENTS
################################################################################

compile_agent() {
    local agent_type="$1"
    local agent_path=""
    
    case "$agent_type" in
        red_recon)
            agent_path="$SCRIPT_DIR/agent/red_recon_agent"
            ;;
        red_exploit)
            agent_path="$SCRIPT_DIR/agent/red_exploit_agent"
            ;;
        red_blackhat)
            agent_path="$SCRIPT_DIR/agent/red_blackhat_agent"
            ;;
        *)
            print_error "Type d'agent inconnu: $agent_type"
            return 1
            ;;
    esac
    
    if [ -f "$agent_path/Makefile" ]; then
        print_info "Compilation de $agent_type..."
        cd "$agent_path" && make 2>&1 | grep -E "error|warning|\[" || true
        print_success "$agent_type compilé"
    fi
}

################################################################################
# EXÉCUTION PRINCIPALE
################################################################################

run_scenario() {
    print_header "LANCEMENT DE L'ATTAQUE"
    
    > "$AGENTS_LOG"  # Réinitialiser le fichier log
    
    local target_count=${#SELECTED_TARGETS[@]}
    local attack_count=${#SELECTED_ATTACKS[@]}
    
    if [ $target_count -eq 0 ] || [ $attack_count -eq 0 ]; then
        print_error "Aucune cible ou attaque sélectionnée"
        return 1
    fi
    
    print_info "Exécution avec $target_count cible(s) et $attack_count attaque(s)"
    echo ""
    
    local iteration=1
    
    for target in "${SELECTED_TARGETS[@]}"; do
        for attack in "${SELECTED_ATTACKS[@]}"; do
            print_info "[$iteration] Attaque sur $target: $attack"
            
            # Exécuter les agents Red Team
            execute_red_recon_agent "$target" "$attack"
            sleep 1
            
            execute_red_exploit_agent "$target" "$attack"
            sleep 1
            
            execute_red_blackhat_agent "$target" "$attack"
            sleep 1
            
            # Exécuter les agents Blue Team
            execute_blue_monitor_agent
            sleep 1
            
            execute_blue_response_agent
            sleep 1
            
            ((iteration++))
            echo ""
        done
    done
    
    print_success "Scénario complété!"
    print_info "Logs disponibles dans: $AGENTS_LOG"
}

################################################################################
# MENU PRINCIPAL
################################################################################

show_main_menu() {
    print_header "GESTIONNAIRE D'AGENTS CENTRALISÉ"
    
    echo ""
    echo -e "${CYAN}Options:${NC}"
    echo "  1) Lancer un scénario d'attaque personnalisé"
    echo "  2) Lancer Red Team uniquement"
    echo "  3) Lancer Blue Team uniquement"
    echo "  4) Voir le dernier log"
    echo "  5) Configurer les cibles"
    echo "  6) Configurer les attaques"
    echo "  0) Quitter"
    echo ""
    
    read -p "Votre choix: " choice
    
    case "$choice" in
        1)
            select_targets
            select_attacks
            run_scenario
            ;;
        2)
            select_targets
            print_header "SCÉNARIO RED TEAM"
            for target in "${SELECTED_TARGETS[@]}"; do
                execute_red_recon_agent "$target" "Recon"
                sleep 1
                execute_red_exploit_agent "$target" "Exploit"
                sleep 1
                execute_red_blackhat_agent "$target" "BlackHat"
                sleep 1
            done
            print_success "Red Team scénario complété!"
            ;;
        3)
            print_header "SCÉNARIO BLUE TEAM"
            execute_blue_monitor_agent
            sleep 1
            execute_blue_response_agent
            print_success "Blue Team scénario complété!"
            ;;
        4)
            if [ -f "$AGENTS_LOG" ]; then
                print_header "DERNIER LOG"
                tail -n 100 "$AGENTS_LOG"
            else
                print_warning "Aucun log disponible"
            fi
            ;;
        5)
            ${EDITOR:-nano} "$TARGETS_FILE"
            ;;
        6)
            ${EDITOR:-nano} "$ATTACKS_FILE"
            ;;
        0)
            print_success "Au revoir!"
            exit 0
            ;;
        *)
            print_error "Choix invalide"
            show_main_menu
            ;;
    esac
    
    echo ""
    read -p "Appuyez sur Entrée pour continuer..." dummy
    show_main_menu
}

################################################################################
# POINT D'ENTRÉE
################################################################################

main() {
    cd "$SCRIPT_DIR"
    
    init_config_files
    
    clear
    show_main_menu
}

# Lancer le programme
main "$@"
