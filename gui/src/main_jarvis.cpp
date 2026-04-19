/**
 * main_jarvis.cpp
 * Lance l'interface JARVIS avec vos agents réels
 * 
 * Compilation:
 *   cd /workspaces/application-linux/gui
 *   g++ -std=c++17 -pthread -I./include src/main_jarvis.cpp -o jarvis
 *   ou: make -f Makefile_jarvis all
 * 
 * Exécution:
 *   ./jarvis
 */

#include "agent_selection_system.h"
#include "jarvis_interface.h"
#include <iostream>

int main() {
    std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "║          J.A.R.V.I.S SYSTEM INITIALIZATION                ║\n";
    std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";

    // Créer le système de sélection
    std::cout << "📦 Création du système de sélection d'agents...\n";
    AgentSelectionSystem system(10);

    // Charger les agents
    std::cout << "🔗 Chargement des agents réels...\n\n";
    
    // ÉQUIPE RED
    system.addAvailableAgent(
        "Agent de Reconnaissance Red",
        "red_recon",
        "./agent/red_recon_agent/red_recon_agent",
        true
    );

    system.addAvailableAgent(
        "Agent d'Exploitation Red",
        "red_exploit",
        "./agent/red_exploit_agent/red_exploit_agent",
        true
    );

    system.addAvailableAgent(
        "Agent Red BlackHat",
        "red_blackhat",
        "./agent/red_blackhat_agent/red_blackhat_agent",
        true
    );

    // ÉQUIPE BLUE
    system.addAvailableAgent(
        "Agent de Surveillance Blue",
        "blue_monitor",
        "./agent_blue_team/monitor_agent/blue_monitor_agent",
        true
    );

    system.addAvailableAgent(
        "Agent de Réponse Blue",
        "blue_response",
        "./agent_blue_team/response_agent/blue_response_agent",
        true
    );

    // Afficher les agents
    system.printAvailableAgents();

    // Lancer JARVIS
    std::cout << "\n🚀 Lancement de l'interface JARVIS...\n\n";
    JarvisInterface jarvis(system);
    jarvis.launch();

    std::cout << "\n✓ J.A.R.V.I.S fermé.\n";
    return 0;
}
