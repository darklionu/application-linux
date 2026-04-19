#!/bin/bash

################################################################################
#                                                                              #
#                   🚀 LANCEUR SIMPLIFIÉ - APPLICATION LINUX                  #
#                     Démarrage facile de tous les systèmes                    #
#                                                                              #
################################################################################

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Configuration
PROJECT_ROOT="/workspaces/application-linux"
GUI_DIR="$PROJECT_ROOT/gui"
HAND_DIR="$PROJECT_ROOT/hand_detection_system"
BUILD_DIR="$GUI_DIR/build"

################################################################################
# FONCTIONS UTILITAIRES
################################################################################

print_header() {
    echo -e "\n${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║${NC}$1${CYAN}║${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}\n"
}

print_success() {
    echo -e "${GREEN}[✓]${NC} $1"
}

print_error() {
    echo -e "${RED}[✗]${NC} $1"
}

print_info() {
    echo -e "${BLUE}[*]${NC} $1"
}

print_option() {
    echo -e "${YELLOW}$1${NC}"
}

check_command() {
    if ! command -v "$1" &> /dev/null; then
        print_error "Commande '$1' non trouvée"
        return 1
    fi
    return 0
}

check_directory() {
    if [ ! -d "$1" ]; then
        print_error "Répertoire '$1' non trouvé"
        return 1
    fi
    return 0
}

################################################################################
# VÉRIFICATIONS PRÉALABLES
################################################################################

verify_environment() {
    print_info "Vérification de l'environnement..."
    
    # Vérifier les répertoires
    check_directory "$PROJECT_ROOT" || exit 1
    check_directory "$GUI_DIR" || exit 1
    check_directory "$HAND_DIR" || exit 1
    
    # Vérifier les outils
    check_command "g++" || exit 1
    check_command "gcc" || exit 1
    
    print_success "Environnement OK"
}

################################################################################
# MENU PRINCIPAL
################################################################################

show_main_menu() {
    print_header "      SÉLECTIONNEZ UN SYSTÈME À LANCER"
    
    echo -e "${CYAN}Systèmes disponibles:${NC}"
    print_option "  1) Interface Graphique GUI (C++)"
    print_option "  2) Détection de Mains (Python)"
    print_option "  3) Détection + Interface combinées"
    print_option "  4) Lancer tous les tests"
    print_option "  5) Configuration et réparation"
    print_option "  6) Afficher l'aide"
    print_option "  0) Quitter"
    echo ""
}

################################################################################
# LANCER L'INTERFACE GUI
################################################################################

launch_gui() {
    print_header "    🖥️  LANCEMENT DE L'INTERFACE GRAPHIQUE"
    
    # Créer le dossier build
    mkdir -p "$BUILD_DIR"
    
    # Vérifier si l'exécutable existe
    if [ -f "$BUILD_DIR/agent_manager_gui_unified" ]; then
        print_success "Exécutable trouvé"
        print_info "Lancement..."
        cd "$GUI_DIR"
        "$BUILD_DIR/agent_manager_gui_unified"
    else
        print_info "Compilation nécessaire..."
        compile_gui
        
        if [ -f "$BUILD_DIR/agent_manager_gui_unified" ]; then
            print_success "Compilation réussie"
            print_info "Lancement..."
            "$BUILD_DIR/agent_manager_gui_unified"
        else
            print_error "Compilation échouée"
            return 1
        fi
    fi
}

################################################################################
# COMPILER L'INTERFACE GUI
################################################################################

compile_gui() {
    print_header "    🔨 COMPILATION DE L'INTERFACE GRAPHIQUE"
    
    cd "$GUI_DIR"
    
    print_info "Compilation du système GUI unifié..."
    
    if g++ -std=c++11 -Wall -Wextra -O2 -o "$BUILD_DIR/agent_manager_gui_unified" src/unified_gui_system.cpp 2>/dev/null; then
        print_success "Compilation réussie"
        return 0
    else
        print_error "Erreur de compilation"
        print_info "Tentative avec compilation simplifiée..."
        
        if g++ -O2 -o "$BUILD_DIR/agent_manager_gui_unified" src/unified_gui_system.cpp 2>/dev/null; then
            print_success "Compilation simplifiée réussie"
            return 0
        else
            print_error "Compilation échouée malgré tout"
            return 1
        fi
    fi
}

################################################################################
# LANCER DÉTECTION DE MAINS
################################################################################

launch_hand_detection() {
    print_header "    🖐️  LANCEMENT DE LA DÉTECTION DE MAINS"
    
    # Vérifier Python
    if ! check_command "python3"; then
        print_error "Python3 non installé"
        return 1
    fi
    
    cd "$HAND_DIR"
    
    # Vérifier les dépendances
    print_info "Vérification des dépendances Python..."
    if python3 -c "import cv2, mediapipe" 2>/dev/null; then
        print_success "Dépendances OK"
    else
        print_error "Dépendances manquantes"
        print_info "Installation des dépendances..."
        pip3 install opencv-python mediapipe -q
        
        if python3 -c "import cv2, mediapipe" 2>/dev/null; then
            print_success "Dépendances installées"
        else
            print_error "Impossible d'installer les dépendances"
            return 1
        fi
    fi
    
    # Lancer le système
    print_info "Lancement du système de détection..."
    python3 main.py
}

################################################################################
# LANCER DÉTECTION OPTIMISÉE
################################################################################

launch_hand_detection_optimized() {
    print_header "    🖐️  LANCEMENT DE LA DÉTECTION DE MAINS (OPTIMISÉE)"
    
    if ! check_command "python3"; then
        print_error "Python3 non installé"
        return 1
    fi
    
    cd "$HAND_DIR"
    
    # Vérifier les dépendances
    if ! python3 -c "import cv2, mediapipe" 2>/dev/null; then
        print_error "Dépendances manquantes"
        pip3 install opencv-python mediapipe -q
    fi
    
    # Créer un script de test si nécessaire
    if [ ! -f "test_optimized.py" ]; then
        cat > test_optimized.py << 'EOF'
#!/usr/bin/env python3
"""Test du détecteur optimisé"""

import cv2
from hand_detector_optimized import HandDetectorOptimized
from gesture_recognizer_advanced import GestureRecognizerAdvanced

def main():
    detector = HandDetectorOptimized(detection_confidence=0.7)
    recognizer = GestureRecognizerAdvanced()
    
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
    cap.set(cv2.CAP_PROP_FPS, 30)
    
    print("\n🖐️  Système de détection optimisé lancé")
    print("Appuyez sur 'q' pour quitter\n")
    
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        hands, annotated = detector.detect(frame)
        annotated = detector.draw_performance_info(annotated)
        
        for i, hand in enumerate(hands):
            gesture_code, gesture_text, confidence = GestureRecognizerAdvanced.recognize(hand)
            annotated = detector.draw_hand_info(annotated, hand, (10, 30 + i*30))
            
            cv2.putText(
                annotated,
                f"Geste: {gesture_text} ({confidence:.0%})",
                (10, 90 + i*30),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.6,
                (0, 255, 0),
                2
            )
        
        cv2.imshow('Détection de Mains', annotated)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    cv2.destroyAllWindows()
    print("\n✓ Fermeture du système\n")

if __name__ == '__main__':
    main()
EOF
        chmod +x test_optimized.py
    fi
    
    print_info "Lancement..."
    python3 test_optimized.py
}

################################################################################
# LANCER COMBINÉ
################################################################################

launch_combined() {
    print_header "    🔗 SYSTÈME COMBINÉ: DÉTECTION + INTERFACE"
    
    print_info "Cette fonction lancera la détection de mains et l'interface"
    print_info "Assurez-vous que la caméra est connectée"
    echo ""
    
    print_option "  1) Lancer la détection d'abord"
    print_option "  2) Lancer l'interface d'abord"
    print_option "  0) Retour au menu"
    echo ""
    
    read -p "Choix: " choice
    
    case $choice in
        1) 
            launch_hand_detection_optimized &
            sleep 2
            launch_gui
            ;;
        2)
            launch_gui &
            sleep 2
            launch_hand_detection_optimized
            ;;
        0) return ;;
        *) print_error "Choix invalide" ;;
    esac
}

################################################################################
# TESTS ET VALIDATION
################################################################################

run_tests() {
    print_header "    🧪 EXÉCUTION DES TESTS"
    
    print_info "Test 1: Compilation GUI"
    if compile_gui; then
        print_success "Test 1 réussi"
    else
        print_error "Test 1 échoué"
    fi
    
    print_info "Test 2: Vérification Python"
    if check_command "python3"; then
        print_success "Test 2 réussi"
    else
        print_error "Test 2 échoué"
    fi
    
    print_info "Test 3: Vérification des fichiers"
    local files_ok=true
    [ -f "$GUI_DIR/src/unified_gui_system.cpp" ] || files_ok=false
    [ -f "$HAND_DIR/hand_detector.py" ] || files_ok=false
    [ -f "$HAND_DIR/gesture_recognizer.py" ] || files_ok=false
    
    if [ "$files_ok" = true ]; then
        print_success "Test 3 réussi"
    else
        print_error "Test 3 échoué"
    fi
}

################################################################################
# CONFIGURATION ET RÉPARATION
################################################################################

configure_system() {
    print_header "    ⚙️  CONFIGURATION ET RÉPARATION"
    
    print_option "  1) Installer les dépendances Python"
    print_option "  2) Recompiler l'interface GUI"
    print_option "  3) Nettoyer les fichiers compilés"
    print_option "  4) Afficher les logs"
    print_option "  0) Retour au menu"
    echo ""
    
    read -p "Choix: " choice
    
    case $choice in
        1)
            print_info "Installation des dépendances Python..."
            pip3 install opencv-python mediapipe -q
            print_success "Installation terminée"
            ;;
        2)
            compile_gui
            ;;
        3)
            print_info "Nettoyage..."
            rm -rf "$BUILD_DIR"/*
            print_success "Nettoyage terminé"
            ;;
        4)
            print_info "Affichage des derniers logs..."
            ls -lah "$BUILD_DIR" 2>/dev/null || print_info "Aucun log disponible"
            ;;
        0) return ;;
        *) print_error "Choix invalide" ;;
    esac
}

################################################################################
# AFFICHER L'AIDE
################################################################################

show_help() {
    print_header "    📖 AIDE"
    
    cat << 'EOF'

🎯 OPTIONS PRINCIPALES:

1. Interface Graphique GUI (C++)
   - Lance l'interface manager avec tous les agents
   - Compile automatiquement si nécessaire
   
2. Détection de Mains (Python)
   - Système de suivi des mains en temps réel
   - Reconnaissance des gestes
   
3. Système Combiné
   - Lance la détection et l'interface ensemble
   - Permet l'interaction manuelle avec l'interface
   
4. Tests et Validation
   - Vérifie tous les composants du système
   - Teste la compilation et les dépendances

5. Configuration
   - Installe les dépendances manquantes
   - Répare les problèmes de compilation
   - Affiche les informations du système

📋 STRUCTURE DU PROJET:
   /gui                    - Interface graphique C++
   /hand_detection_system  - Système de détection Python
   /agent                  - Agents d'attaque/défense
   /build                  - Fichiers compilés

💡 ASTUCES:
   - Vérifiez votre caméra avant le lancement
   - Assurez-vous d'avoir gcc, g++ et python3 installés
   - Les dépendances Python s'installeront automatiquement
   - Pour voir plus de logs: VERBOSE=1 ./launcher.sh

EOF

    read -p "Appuyez sur Entrée pour continuer..."
}

################################################################################
# BOUCLE PRINCIPALE
################################################################################

main() {
    # Vérification initiale
    verify_environment
    
    # Boucle principale
    while true; do
        show_main_menu
        read -p "Votre choix: " choice
        
        case $choice in
            1) launch_gui ;;
            2) launch_hand_detection ;;
            3) launch_hand_detection_optimized ;;
            4) launch_combined ;;
            5) run_tests ;;
            6) configure_system ;;
            7) show_help ;;
            0) 
                print_header "      ✓ AU REVOIR!"
                exit 0
                ;;
            *)
                print_error "Choix invalide"
                ;;
        esac
        
        echo ""
        read -p "Appuyez sur Entrée pour continuer..."
    done
}

################################################################################
# LANCER LE SCRIPT
################################################################################

if [ "${BASH_SOURCE[0]}" = "${0}" ]; then
    main
fi
