#include "../include/agent_selection_system.h"
#include "../include/agent_selection_interface.h"
#include <iostream>

/**
 * Example: Système amélioré de sélection d'agents
 * Ce fichier montre comment utiliser le nouveau système
 */

int main() {
    std::cout << "\n╔═══════════════════════════════════════════════════════════════╗\n";
    std::cout << "║     GESTIONNAIRE AMÉLIORÉ DE SÉLECTION D'AGENTS\n";
    std::cout << "║              Version 2.0\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════╝\n";

    // Créer le système de sélection
    AgentSelectionSystem selection_system(10); // Max 10 agents

    // Ajouter les agents disponibles (basé sur agent_config.h)
    std::cout << "\n📦 Chargement des agents disponibles...\n";
    
    // RED TEAM
    selection_system.addAvailableAgent(
        "Red Recon Agent", 
        "red_recon", 
        "./agent/red_recon_agent/red_recon_agent",
        true
    );
    
    selection_system.addAvailableAgent(
        "Red Exploit Agent",
        "red_exploit",
        "./agent/red_exploit_agent/red_exploit_agent",
        true
    );
    
    selection_system.addAvailableAgent(
        "Red BlackHat Agent",
        "red_blackhat",
        "./agent/red_blackhat_agent/red_blackhat_agent",
        true
    );

    // BLUE TEAM
    selection_system.addAvailableAgent(
        "Blue Monitor Agent",
        "blue_monitor",
        "./agent_blue_team/monitor_agent/blue_monitor_agent",
        true
    );
    
    selection_system.addAvailableAgent(
        "Blue Response Agent",
        "blue_response",
        "./agent_blue_team/response_agent/blue_response_agent",
        true
    );

    std::cout << "✓ " << selection_system.getAvailableCount() << " agents chargés\n";

    // Créer l'interface
    AgentSelectionInterface interface(selection_system);

    // Afficher les agents disponibles
    selection_system.printAvailableAgents();

    // Lancer le menu interactif
    std::cout << "\n🎮 Démarrage du menu interactif...\n";
    interface.runTextMenu();

    return 0;
}
