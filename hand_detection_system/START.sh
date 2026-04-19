#!/bin/bash

# 🎥 SYSTÈME DE DÉTECTION DE MAINS
# Démarrage Automatisé

clear

echo ""
echo "╔════════════════════════════════════════════════════════════════╗"
echo "║                                                                ║"
echo "║     🎥 SYSTÈME DE DÉTECTION DE MAINS - DÉMARRAGE RAPIDE 🎥   ║"
echo "║                                                                ║"
echo "║              Interface Interactive avec Gestes                 ║"
echo "║                                                                ║"
echo "╚════════════════════════════════════════════════════════════════╝"
echo ""

# Vérifier si nous sommes dans le bon répertoire
if [ ! -f "main.py" ]; then
    echo "❌ Erreur: main.py non trouvé"
    echo "Lancez ce script depuis le répertoire hand_detection_system"
    exit 1
fi

# Menu principal
show_menu() {
    echo ""
    echo "┌────────────────────────────────────────────────────────────┐"
    echo "│               MENU PRINCIPAL - QUE FAIRE?                  │"
    echo "├────────────────────────────────────────────────────────────┤"
    echo "│                                                            │"
    echo "│  1) 🚀 Lancer le programme principal (main.py)           │"
    echo "│  2) 🧪 Faire un test rapide (test_quick.py)              │"
    echo "│  3) 📚 Voir les exemples (examples.py)                   │"
    echo "│  4) 📖 Lire la documentation (README.md)                 │"
    echo "│  5) ⚙️  Installer les dépendances (install.sh)           │"
    echo "│  6) 📋 Voir le résumé complet (SUMMARY.sh)               │"
    echo "│  7) 📑 Voir l'index du projet (INDEX.md)                 │"
    echo "│  8) 📝 Voir la liste des fichiers (FILES_SUMMARY.md)     │"
    echo "│  9) 🔍 Vérifier les fichiers (ls -la)                   │"
    echo "│  0) ❌ Quitter                                            │"
    echo "│                                                            │"
    echo "└────────────────────────────────────────────────────────────┘"
    echo ""
}

while true; do
    show_menu
    read -p "Choisir une option (0-9): " choice
    
    case $choice in
        1)
            echo ""
            echo "🚀 Lancement du programme principal..."
            echo ""
            source venv/bin/activate 2>/dev/null || {
                echo "⚠️  Environnement virtuel non initialisé"
                echo "Lancez d'abord: bash install.sh"
                continue
            }
            python3 main.py
            ;;
        
        2)
            echo ""
            echo "🧪 Exécution du test rapide..."
            echo ""
            source venv/bin/activate 2>/dev/null || {
                echo "⚠️  Environnement virtuel non initialisé"
                echo "Lancez d'abord: bash install.sh"
                continue
            }
            python3 test_quick.py
            read -p "Appuyer sur Entrée pour continuer..."
            ;;
        
        3)
            echo ""
            echo "📚 Affichage des exemples..."
            echo ""
            source venv/bin/activate 2>/dev/null || {
                echo "⚠️  Environnement virtuel non initialisé"
                echo "Lancez d'abord: bash install.sh"
                continue
            }
            python3 examples.py
            ;;
        
        4)
            echo ""
            echo "📖 Affichage du README..."
            echo ""
            if command -v less &> /dev/null; then
                less README.md
            elif command -v more &> /dev/null; then
                more README.md
            else
                cat README.md
            fi
            ;;
        
        5)
            echo ""
            echo "⚙️  Installation des dépendances..."
            echo ""
            bash install.sh
            echo ""
            read -p "Appuyer sur Entrée pour continuer..."
            ;;
        
        6)
            echo ""
            echo "📋 Affichage du résumé..."
            echo ""
            bash SUMMARY.sh
            read -p "Appuyer sur Entrée pour continuer..."
            ;;
        
        7)
            echo ""
            echo "📑 Affichage de l'index..."
            echo ""
            if command -v less &> /dev/null; then
                less INDEX.md
            elif command -v more &> /dev/null; then
                more INDEX.md
            else
                cat INDEX.md
            fi
            ;;
        
        8)
            echo ""
            echo "📝 Affichage de la liste des fichiers..."
            echo ""
            if command -v less &> /dev/null; then
                less FILES_SUMMARY.md
            elif command -v more &> /dev/null; then
                more FILES_SUMMARY.md
            else
                cat FILES_SUMMARY.md
            fi
            ;;
        
        9)
            echo ""
            echo "🔍 Fichiers du projet:"
            echo ""
            ls -lah
            echo ""
            read -p "Appuyer sur Entrée pour continuer..."
            ;;
        
        0)
            echo ""
            echo "Au revoir! 👋"
            echo ""
            exit 0
            ;;
        
        *)
            echo "❌ Choix invalide. Veuillez choisir entre 0 et 9."
            ;;
    esac
done
