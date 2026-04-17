#include <iostream>
#include "include/gui_window.h"
#include "include/gui_agent_panel.h"

/**
 * Exemple d'utilisation de la GUI
 * À compléter avec SFML ou Qt
 */

int main(int argc, char* argv[]) {
    std::cout << "╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║  INTERFACE GRAPHIQUE - GESTIONNAIRE D'AGENTS          ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n";
    std::cout << "\n[*] Initialisation de l'interface graphique...\n\n";

    // Créer le gestionnaire de panneaux
    AgentPanelManager manager;

    std::cout << "[✓] Gestionnaire d'agents créé\n";
    std::cout << "[✓] Agents disponibles: " << manager.getPanelCount() << "\n\n";

    // Afficher les agents
    std::cout << "📊 Agents dans l'interface:\n";
    for (size_t i = 0; i < manager.getPanelCount(); ++i) {
        const auto& panel = manager.getPanels()[i];
        std::cout << "  " << (i+1) << ". " << panel.name 
                  << " (" << panel.type << ") - Status: " << panel.status << "\n";
    }

    std::cout << "\n[*] Architecture de l'interface:\n";
    std::cout << "┌─────────────────────────────────────────────────────┐\n";
    std::cout << "│ BARRE DE MENU                                       │\n";
    std::cout << "├─────────────────────────────────────────────────────┤\n";
    std::cout << "│                PANNEAUX D'AGENTS                    │\n";
    std::cout << "│ ┌──────────────┐ ┌──────────────┐ ┌──────────────┐ │\n";
    std::cout << "│ │   RED TEAM   │ │  BLUE TEAM   │ │  MANAGER     │ │\n";
    std::cout << "│ └──────────────┘ └──────────────┘ └──────────────┘ │\n";
    std::cout << "├─────────────────────────────────────────────────────┤\n";
    std::cout << "│          ZONE DE CONTRÔLE ET EXÉCUTION             │\n";
    std::cout << "├─────────────────────────────────────────────────────┤\n";
    std::cout << "│            LOGS EN TEMPS RÉEL                      │\n";
    std::cout << "└─────────────────────────────────────────────────────┘\n";

    std::cout << "\n[!] Cet exemple est une base de démarrage.\n";
    std::cout << "[!] Pour une GUI complète, utiliser SFML ou Qt.\n";
    std::cout << "[!] Voir gui/README.md pour les instructions.\n";

    return 0;
}
