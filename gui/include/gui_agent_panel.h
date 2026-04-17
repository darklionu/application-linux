#ifndef GUI_AGENT_PANEL_H
#define GUI_AGENT_PANEL_H

#include <string>
#include <vector>

/**
 * @struct AgentPanel
 * @brief Représente un panneau pour un agent dans l'interface
 */
struct AgentPanel {
    std::string name;
    std::string type;
    std::string status;      // "Ready", "Running", "Done", "Error"
    int x_pos;
    int y_pos;
    int width;
    int height;
    bool is_selected;
    bool is_enabled;

    AgentPanel(const std::string& agentName, const std::string& agentType, int xPos, int yPos)
        : name(agentName), type(agentType), status("Ready"), x_pos(xPos), y_pos(yPos),
          width(180), height(100), is_selected(false), is_enabled(true) {}
};

/**
 * @class AgentPanelManager
 * @brief Gère les panneaux d'agents dans l'interface
 */
class AgentPanelManager {
private:
    std::vector<AgentPanel> panels;

public:
    AgentPanelManager();
    ~AgentPanelManager() = default;

    void addPanel(const AgentPanel& panel) { panels.push_back(panel); }
    void updatePanelStatus(int index, const std::string& status);
    void selectPanel(int index);
    void unselectAllPanels();

    const std::vector<AgentPanel>& getPanels() const { return panels; }
    size_t getPanelCount() const { return panels.size(); }
};

#endif // GUI_AGENT_PANEL_H
