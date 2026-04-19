#include "agent_selection_system.h"
#include "jarvis_interactive.h"
#include <iostream>

int main() {
    // Créer le système
    AgentSelectionSystem system(10);
    
    // Charger les 5 agents réels
    system.addAvailableAgent("Agent de Reconnaissance Red", "red_recon", 
                            "./agent/red_recon_agent/red_recon_agent", true);
    system.addAvailableAgent("Agent d'Exploitation Red", "red_exploit", 
                            "./agent/red_exploit_agent/red_exploit_agent", true);
    system.addAvailableAgent("Agent Red BlackHat", "red_blackhat", 
                            "./agent/red_blackhat_agent/red_blackhat_agent", true);
    system.addAvailableAgent("Agent de Surveillance Blue", "blue_monitor", 
                            "./agent_blue_team/monitor_agent/blue_monitor_agent", true);
    system.addAvailableAgent("Agent de Réponse Blue", "blue_response", 
                            "./agent_blue_team/response_agent/blue_response_agent", true);
    
    // Créer et lancer l'interface interactive
    JarvisInteractive interface(system);
    interface.launch();
    
    return 0;
}
