#!/bin/bash
#
# Exemple d'utilisation du Gestionnaire d'Agents
# Ce script montre différents cas d'usage
#

APP="./agent_manager_app"

if [ ! -f "$APP" ]; then
    echo "❌ L'application n'est pas compilée"
    echo "Compilation en cours..."
    make -f Makefile_cpp all || exit 1
fi

echo "═══════════════════════════════════════════════════════════"
echo "  EXEMPLES D'UTILISATION - GESTIONNAIRE D'AGENTS"
echo "═══════════════════════════════════════════════════════════"
echo ""

# Exemple 1: Afficher l'aide
echo "📌 Exemple 1: Afficher l'aide"
echo "Commande: $APP --help"
echo ""
$APP --help
echo ""

# Exemple 2: Lancer en mode verbeux (simulé via entrée)
echo "📌 Exemple 2: Utilisation interactif (non simulé)"
echo "Commande: $APP"
echo ""
echo "Instructions:"
echo "  1. Choisir une option (1-12)"
echo "  2. Option 11 pour voir les agents disponibles"
echo "  3. Option 9 pour simulation complète"
echo "  4. Option 0 pour quitter"
echo ""

# Exemple 3: Documentation des scripts de gestion
echo "═══════════════════════════════════════════════════════════"
echo "  SCRIPTS DE GESTION DISPONIBLES"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "1. agent_manager.sh (Gestion complète)"
echo "   ./agent_manager.sh build       - Compiler"
echo "   ./agent_manager.sh run         - Compiler et exécuter"
echo "   ./agent_manager.sh clean       - Nettoyer"
echo "   ./agent_manager.sh rebuild     - Recompiler"
echo "   ./agent_manager.sh debug       - Compile avec debug"
echo "   ./agent_manager.sh verbose     - Mode verbeux"
echo "   ./agent_manager.sh help        - Aide"
echo ""
echo "2. quick_start_manager.sh (Démarrage rapide)"
echo "   ./quick_start_manager.sh       - Compile et lance directement"
echo ""

# Exemple 4: Structure du projet
echo "═══════════════════════════════════════════════════════════"
echo "  STRUCTURE DU PROJET"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "Fichiers sources:"
echo "  - agent_manager_cpp.h     (Classes et interfaces)"
echo "  - agent_manager_cpp.cpp   (Implémentation)"
echo "  - main_agent_manager.cpp  (Point d'entrée)"
echo "  - agent_config.h          (Configuration)"
echo ""
echo "Scripts et configuration:"
echo "  - Makefile_cpp            (Compilation)"
echo "  - agent_manager.sh        (Gestion)"
echo "  - quick_start_manager.sh  (Démarrage rapide)"
echo ""
echo "Documentation:"
echo "  - README_AGENT_MANAGER.md (Documentation complète)"
echo "  - INSTALLATION.md         (Guide d'installation)"
echo ""

# Exemple 5: Cas d'usage typiques
echo "═══════════════════════════════════════════════════════════"
echo "  CAS D'USAGE TYPIQUES"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "🎯 Cas 1: Tester un agent Red Team"
echo "   ./agent_manager_app"
echo "   → Sélectionner option 1, 2 ou 3"
echo ""
echo "🎯 Cas 2: Tester la défense Blue Team"
echo "   ./agent_manager_app"
echo "   → Sélectionner option 5 ou 6"
echo ""
echo "🎯 Cas 3: Simulation d'attaque/défense"
echo "   ./agent_manager_app"
echo "   → Sélectionner option 9"
echo ""
echo "🎯 Cas 4: Séquence personnalisée"
echo "   ./agent_manager_app"
echo "   → Sélectionner option 10"
echo "   → Choisir les agents dans l'ordre voulu"
echo ""
echo "🎯 Cas 5: Mode développement"
echo "   ./agent_manager.sh debug"
echo "   gdb ./agent_manager_app"
echo ""

# Exemple 6: Ajouter un nouvel agent
echo "═══════════════════════════════════════════════════════════"
echo "  AJOUTER UN NOUVEL AGENT"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "1. Éditer agent_manager_cpp.cpp"
echo "2. Dans la fonction initializeAgents(), ajouter:"
echo ""
echo "   addAgent(Agent(\"Nom de l'agent\","
echo "                  \"./chemin/vers/agent\","
echo "                  \"type_agent\"));"
echo ""
echo "3. Recompiler:"
echo "   make -f Makefile_cpp rebuild"
echo ""
echo "4. Exécuter:"
echo "   ./agent_manager_app"
echo ""

# Exemple 7: Informations de compilation
echo "═══════════════════════════════════════════════════════════"
echo "  INFORMATIONS DE COMPILATION"
echo "═══════════════════════════════════════════════════════════"
echo ""
echo "Compilateur: g++"
echo "Standard C++: C++17"
echo "Flags: -std=c++17 -Wall -Wextra -O2"
echo ""

# Afficher les fichiers générés
if ls -la agent_manager_app *.o 2>/dev/null | head -10; then
    echo ""
    echo "✅ Application compilée et prête à être utilisée"
else
    echo ""
    echo "⚠️  Fichiers compilés non trouvés"
    echo "Compilez d'abord avec: make -f Makefile_cpp all"
fi

echo ""
echo "═══════════════════════════════════════════════════════════"
echo "  Pour plus de détails, consultez:"
echo "  - README_AGENT_MANAGER.md"
echo "  - INSTALLATION.md"
echo "═══════════════════════════════════════════════════════════"
