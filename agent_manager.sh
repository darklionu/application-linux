#!/bin/bash
#
# Script de gestion du Gestionnaire d'Agents
# Compilation, exécution et gestion de l'application C++
#

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MAKEFILE="Makefile_cpp"
TARGET="agent_manager_app"
BUILD_DIR="build"

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Fonctions
print_header() {
    echo -e "${BLUE}════════════════════════════════════════${NC}"
    echo -e "${BLUE}  $1${NC}"
    echo -e "${BLUE}════════════════════════════════════════${NC}"
}

print_success() {
    echo -e "${GREEN}[✓] $1${NC}"
}

print_error() {
    echo -e "${RED}[✗] $1${NC}"
}

print_info() {
    echo -e "${YELLOW}[*] $1${NC}"
}

# Commandes disponibles
usage() {
    echo "Usage: $0 {build|run|clean|help|rebuild|debug}"
    echo ""
    echo "Commandes:"
    echo "  build       Compiler l'application"
    echo "  run         Compiler et exécuter l'application"
    echo "  clean       Nettoyer les fichiers compilés"
    echo "  rebuild     Nettoyer et recompiler"
    echo "  debug       Compiler avec les symboles de debug"
    echo "  verbose     Exécuter en mode verbeux"
    echo "  help        Afficher cette aide"
    exit 1
}

build() {
    print_header "Compilation de l'application"
    cd "$SCRIPT_DIR"
    
    if make -f "$MAKEFILE" all; then
        print_success "Compilation réussie"
        return 0
    else
        print_error "Erreur lors de la compilation"
        return 1
    fi
}

run() {
    print_header "Exécution de l'application"
    cd "$SCRIPT_DIR"
    
    if [ ! -f "$TARGET" ]; then
        print_info "L'exécutable n'existe pas, compilation en cours..."
        build || exit 1
    fi
    
    print_info "Lancement de $TARGET..."
    ./"$TARGET"
}

run_verbose() {
    print_header "Exécution avec mode verbeux"
    cd "$SCRIPT_DIR"
    
    if [ ! -f "$TARGET" ]; then
        print_info "L'exécutable n'existe pas, compilation en cours..."
        build || exit 1
    fi
    
    print_info "Lancement de $TARGET avec mode verbeux..."
    ./"$TARGET" --verbose
}

clean() {
    print_header "Nettoyage des fichiers compilés"
    cd "$SCRIPT_DIR"
    
    make -f "$MAKEFILE" clean
    print_success "Nettoyage terminé"
}

rebuild() {
    print_header "Reconstruction (clean + build)"
    clean
    build
}

debug() {
    print_header "Compilation avec symboles de debug"
    cd "$SCRIPT_DIR"
    print_info "Ajustement des flags de compilation pour le debug..."
    
    # Compiler avec -g pour les symboles de debug
    g++ -std=c++17 -Wall -Wextra -g -O0 -c agent_manager_cpp.cpp -o agent_manager_cpp.o
    g++ -std=c++17 -Wall -Wextra -g -O0 -c main_agent_manager.cpp -o main_agent_manager.o
    g++ -std=c++17 -Wall -Wextra -g -O0 -o "$TARGET" agent_manager_cpp.o main_agent_manager.o
    
    print_success "Compilation debug réussie"
    print_info "Vous pouvez maintenant utiliser gdb: gdb ./$TARGET"
}

show_info() {
    print_header "Information sur l'application"
    echo ""
    echo "Gestionnaire d'Agents Centralisé"
    echo "Langage: C++17"
    echo "Exécutable: $TARGET"
    echo "Makefile: $MAKEFILE"
    echo ""
    echo "Agents disponibles:"
    echo "  - Red Recon Agent"
    echo "  - Red Exploit Agent"
    echo "  - Red BlackHat Agent"
    echo "  - Blue Monitor Agent"
    echo "  - Blue Response Agent"
    echo "  - Red Team Manager"
    echo ""
}

# Main
case "${1:-help}" in
    build)
        build
        ;;
    run)
        run
        ;;
    clean)
        clean
        ;;
    rebuild)
        rebuild
        ;;
    debug)
        debug
        ;;
    verbose)
        run_verbose
        ;;
    help|--help|-h)
        show_info
        usage
        ;;
    *)
        print_error "Commande inconnue: $1"
        usage
        ;;
esac
