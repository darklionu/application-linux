#!/bin/bash
# Démarrage rapide du Gestionnaire d'Agents
# Ce script compile et exécute directement l'application

cd "$(dirname "$0")" || exit

echo "🚀 Gestionnaire d'Agents Centralisé"
echo "===================================="
echo ""

# Vérifier si déjà compilé
if [ -f "./agent_manager_app" ]; then
    echo "✓ Exécutable trouvé"
    echo "Démarrage..."
    ./agent_manager_app
else
    echo "⚙️  Compilation nécessaire..."
    make -f Makefile_cpp all && ./agent_manager_app
fi
