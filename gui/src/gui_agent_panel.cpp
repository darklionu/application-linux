#include "../include/gui_agent_panel.h"

AgentPanelManager::AgentPanelManager() {
    // Initialiser avec les agents par défaut
    panels.push_back(AgentPanel("Red Recon Agent", "red_recon", 50, 100));
    panels.push_back(AgentPanel("Red Exploit Agent", "red_exploit", 250, 100));
    panels.push_back(AgentPanel("Red BlackHat Agent", "red_blackhat", 450, 100));
    
    panels.push_back(AgentPanel("Blue Monitor Agent", "blue_monitor", 50, 250));
    panels.push_back(AgentPanel("Blue Response Agent", "blue_response", 250, 250));
}

void AgentPanelManager::updatePanelStatus(int index, const std::string& status) {
    if (index >= 0 && index < panels.size()) {
        panels[index].status = status;
    }
}

void AgentPanelManager::selectPanel(int index) {
    if (index >= 0 && index < panels.size()) {
        unselectAllPanels();
        panels[index].is_selected = true;
    }
}

void AgentPanelManager::unselectAllPanels() {
    for (auto& panel : panels) {
        panel.is_selected = false;
    }
}
