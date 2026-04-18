#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <cstring>
#include <fstream>
#include <sstream>
#include <algorithm>

// ============================================================================
// SECTION 1 : CLASSES DE BASE POUR L'INTERFACE
// ============================================================================

/**
 * @class Button
 * @brief Classe pour les boutons de l'interface
 */
class Button {
private:
    std::string label;
    int x_pos;
    int y_pos;
    int width;
    int height;
    bool is_hovered;
    bool is_clicked;
    std::function<void()> callback;

public:
    Button(const std::string& text, int x, int y, int w = 120, int h = 40)
        : label(text), x_pos(x), y_pos(y), width(w), height(h),
          is_hovered(false), is_clicked(false) {}
    ~Button() = default;

    void render() {
        // Placeholder pour le rendu (à implémenter avec SFML/Qt)
    }

    void update(int mouse_x, int mouse_y, bool mouse_clicked) {
        bool hovered = (mouse_x >= x_pos && mouse_x <= x_pos + width &&
                       mouse_y >= y_pos && mouse_y <= y_pos + height);
        
        is_hovered = hovered;
        
        if (hovered && mouse_clicked) {
            is_clicked = true;
            if (callback) {
                callback();
            }
        } else {
            is_clicked = false;
        }
    }

    void setCallback(std::function<void()> func) { callback = func; }
    bool isHovered() const { return is_hovered; }
    bool wasClicked() const { return is_clicked; }
    const std::string& getLabel() const { return label; }
    void setPosition(int x, int y) { x_pos = x; y_pos = y; }
    void setSize(int w, int h) { width = w; height = h; }
};

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
    AgentPanelManager() {
        // Initialiser avec les agents par défaut
        panels.push_back(AgentPanel("Red Recon Agent", "red_recon", 50, 100));
        panels.push_back(AgentPanel("Red Exploit Agent", "red_exploit", 250, 100));
        panels.push_back(AgentPanel("Red BlackHat Agent", "red_blackhat", 450, 100));
        
        panels.push_back(AgentPanel("Blue Monitor Agent", "blue_monitor", 50, 250));
        panels.push_back(AgentPanel("Blue Response Agent", "blue_response", 250, 250));
    }
    ~AgentPanelManager() = default;

    void addPanel(const AgentPanel& panel) { panels.push_back(panel); }
    void updatePanelStatus(int index, const std::string& status) {
        if (index >= 0 && index < panels.size()) {
            panels[index].status = status;
        }
    }

    void selectPanel(int index) {
        if (index >= 0 && index < panels.size()) {
            unselectAllPanels();
            panels[index].is_selected = true;
        }
    }

    void unselectAllPanels() {
        for (auto& panel : panels) {
            panel.is_selected = false;
        }
    }

    const std::vector<AgentPanel>& getPanels() const { return panels; }
    size_t getPanelCount() const { return panels.size(); }
};

/**
 * @class GUIWindow
 * @brief Classe principale pour la fenêtre graphique du gestionnaire d'agents
 */
class GUIWindow {
private:
    int window_width;
    int window_height;
    std::string window_title;
    bool is_running;

public:
    GUIWindow(int width = 1200, int height = 800, const std::string& title = "Agent Manager - GUI")
        : window_width(width), window_height(height), window_title(title), is_running(true) {}
    ~GUIWindow() = default;

    void create() {
        std::cout << "Fenêtre créée: " << window_title << std::endl;
    }

    void run() {
        is_running = true;
    }

    void close() {
        is_running = false;
    }

    bool isRunning() const { return is_running; }

    void renderMainMenu() {
        std::cout << "Rendu du menu principal" << std::endl;
    }

    void renderAgentList() {
        std::cout << "Rendu de la liste des agents" << std::endl;
    }

    void renderExecutionPanel() {
        std::cout << "Rendu du panneau d'exécution" << std::endl;
    }

    void renderLogs() {
        std::cout << "Rendu des logs" << std::endl;
    }

    int getWidth() const { return window_width; }
    int getHeight() const { return window_height; }
};

// ============================================================================
// SECTION 2 : SYSTÈME DE CONFIGURATION D'ATTAQUES
// ============================================================================

/**
 * @struct AttackTechnique
 * @brief Représente une technique d'attaque disponible
 */
struct AttackTechnique {
    std::string id;
    std::string name;
    std::string phase;      // reconnaissance, scan, exploit, lateral, persist
    int success_rate;
    bool available;
    bool selected;

    AttackTechnique(const std::string& _id, const std::string& _name, 
                   const std::string& _phase, int _rate)
        : id(_id), name(_name), phase(_phase), success_rate(_rate), 
          available(true), selected(false) {}
};

/**
 * @class AgentAttackConfig
 * @brief Configuration des attaques pour un agent spécifique
 */
class AgentAttackConfig {
private:
    std::string agent_id;
    std::string agent_name;
    std::vector<AttackTechnique> available_techniques;
    std::vector<std::string> selected_techniques;

public:
    AgentAttackConfig(const std::string& id, const std::string& name)
        : agent_id(id), agent_name(name) {
        initDefaultTechniques();
    }

    void initDefaultTechniques() {
        // Techniques de reconnaissance
        available_techniques.push_back(AttackTechnique("recon_001", "Port Scanning (nmap)", "reconnaissance", 90));
        available_techniques.push_back(AttackTechnique("recon_002", "DNS Enumeration", "reconnaissance", 85));
        available_techniques.push_back(AttackTechnique("recon_003", "OSINT Gathering", "reconnaissance", 80));
        
        // Techniques de scan
        available_techniques.push_back(AttackTechnique("scan_001", "Vulnerability Scan", "scan", 75));
        available_techniques.push_back(AttackTechnique("scan_002", "Service Enumeration", "scan", 85));
        available_techniques.push_back(AttackTechnique("scan_003", "Web Application Scan", "scan", 70));
        
        // Techniques d'exploitation
        available_techniques.push_back(AttackTechnique("exploit_001", "Credential Brute Force", "exploit", 65));
        available_techniques.push_back(AttackTechnique("exploit_002", "SQL Injection", "exploit", 60));
        available_techniques.push_back(AttackTechnique("exploit_003", "Remote Code Execution", "exploit", 50));
        available_techniques.push_back(AttackTechnique("exploit_004", "Password Cracking (john)", "exploit", 75));
        
        // Techniques de mouvement latéral
        available_techniques.push_back(AttackTechnique("lateral_001", "Privilege Escalation", "lateral", 70));
        available_techniques.push_back(AttackTechnique("lateral_002", "Pass-the-Hash", "lateral", 65));
        available_techniques.push_back(AttackTechnique("lateral_003", "Kerberos Exploitation", "lateral", 55));
        
        // Techniques de persistance
        available_techniques.push_back(AttackTechnique("persist_001", "Backdoor Installation", "persist", 60));
        available_techniques.push_back(AttackTechnique("persist_002", "Rootkit Deployment", "persist", 50));
        available_techniques.push_back(AttackTechnique("persist_003", "Scheduled Task Creation", "persist", 75));
    }

    void selectTechnique(const std::string& tech_id) {
        for (auto& tech : available_techniques) {
            if (tech.id == tech_id) {
                tech.selected = true;
                // Vérifier si pas déjà dans la liste
                if (std::find(selected_techniques.begin(), selected_techniques.end(), tech_id) 
                    == selected_techniques.end()) {
                    selected_techniques.push_back(tech_id);
                }
                return;
            }
        }
    }

    void deselectTechnique(const std::string& tech_id) {
        for (auto& tech : available_techniques) {
            if (tech.id == tech_id) {
                tech.selected = false;
                break;
            }
        }
        selected_techniques.erase(
            std::remove(selected_techniques.begin(), selected_techniques.end(), tech_id),
            selected_techniques.end()
        );
    }

    void toggleTechnique(const std::string& tech_id) {
        for (auto& tech : available_techniques) {
            if (tech.id == tech_id) {
                if (tech.selected) {
                    deselectTechnique(tech_id);
                } else {
                    selectTechnique(tech_id);
                }
                return;
            }
        }
    }

    const std::vector<AttackTechnique>& getAvailableTechniques() const {
        return available_techniques;
    }

    const std::vector<std::string>& getSelectedTechniques() const {
        return selected_techniques;
    }

    std::string getAgentId() const { return agent_id; }
    std::string getAgentName() const { return agent_name; }

    void printConfiguration() const {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║     Configuration d'attaques - " << agent_name << "\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        
        // Afficher les techniques par phase
        std::vector<std::string> phases = {"reconnaissance", "scan", "exploit", "lateral", "persist"};
        
        for (const auto& phase : phases) {
            std::cout << "║\n║  [" << phase << "]\n";
            int count = 0;
            for (const auto& tech : available_techniques) {
                if (tech.phase == phase) {
                    count++;
                    char marker = tech.selected ? '✓' : ' ';
                    std::cout << "║    [" << marker << "] " << tech.name 
                             << " (" << tech.success_rate << "%)\n";
                }
            }
            if (count == 0) {
                std::cout << "║    Aucune technique\n";
            }
        }
        
        std::cout << "║\n╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  Techniques sélectionnées: " << selected_techniques.size() << "\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }
};

/**
 * @class AttackConfigurationManager
 * @brief Gestionnaire central des configurations d'attaques pour tous les agents
 */
class AttackConfigurationManager {
private:
    std::vector<AgentAttackConfig> agent_configs;
    std::string config_file;

public:
    AttackConfigurationManager(const std::string& file = "attack_config.txt")
        : config_file(file) {
        // Initialiser les configurations pour les agents par défaut
        agent_configs.push_back(AgentAttackConfig("red_recon", "Red Recon Agent"));
        agent_configs.push_back(AgentAttackConfig("red_exploit", "Red Exploit Agent"));
        agent_configs.push_back(AgentAttackConfig("red_blackhat", "Red BlackHat Agent"));
    }

    AgentAttackConfig* getAgentConfig(const std::string& agent_id) {
        for (auto& config : agent_configs) {
            if (config.getAgentId() == agent_id) {
                return &config;
            }
        }
        return nullptr;
    }

    void saveConfiguration() {
        std::ofstream file(config_file);
        if (!file.is_open()) {
            std::cerr << "Erreur: Impossible d'ouvrir le fichier de configuration\n";
            return;
        }

        for (const auto& config : agent_configs) {
            file << "[" << config.getAgentId() << "]\n";
            const auto& selected = config.getSelectedTechniques();
            for (const auto& tech_id : selected) {
                file << tech_id << "\n";
            }
            file << "\n";
        }

        file.close();
        std::cout << "[✓] Configuration sauvegardée dans: " << config_file << "\n";
    }

    void loadConfiguration() {
        std::ifstream file(config_file);
        if (!file.is_open()) {
            std::cout << "[i] Fichier de configuration non trouvé, utilisant les defaults\n";
            return;
        }

        std::string line;
        std::string current_agent;

        while (std::getline(file, line)) {
            // Trim les espaces
            line.erase(0, line.find_first_not_of(" \t"));
            line.erase(line.find_last_not_of(" \t") + 1);

            if (line.empty() || line[0] == '#') continue;

            if (line[0] == '[' && line[line.length()-1] == ']') {
                current_agent = line.substr(1, line.length() - 2);
            } else if (!current_agent.empty()) {
                auto* config = getAgentConfig(current_agent);
                if (config) {
                    config->selectTechnique(line);
                }
            }
        }

        file.close();
        std::cout << "[✓] Configuration chargée depuis: " << config_file << "\n";
    }

    void printAllConfigurations() const {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           CONFIGURATIONS D'ATTAQUES PAR AGENT              ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
        
        for (const auto& config : agent_configs) {
            std::cout << "\n  Agent: " << config.getAgentName() << "\n";
            std::cout << "  Techniques sélectionnées: " << config.getSelectedTechniques().size() << "\n";
            const auto& selected = config.getSelectedTechniques();
            for (size_t i = 0; i < selected.size(); ++i) {
                std::cout << "    " << (i+1) << ". " << selected[i] << "\n";
            }
        }
    }

    const std::vector<AgentAttackConfig>& getAllConfigs() const {
        return agent_configs;
    }
};

// ============================================================================
// SECTION 3 : INTERFACE UTILISATEUR SIMPLIFIÉE
// ============================================================================

class SimpleGUI {
private:
    int width;
    int height;
    std::string title;
    AttackConfigurationManager attack_manager;

public:
    SimpleGUI(int w = 1200, int h = 800, const std::string& t = "Agent Manager - Unified GUI")
        : width(w), height(h), title(t) {}

    void displayWelcome() {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           INTERFACE GRAPHIQUE - GESTIONNAIRE D'AGENTS      ║\n";
        std::cout << "║                  Système Unifié v1.0                       ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
        std::cout << "Dimensions: " << width << "x" << height << "\n";
        std::cout << "Titre: " << title << "\n\n";
    }

    void displayMenu() {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                     MENU PRINCIPAL                         ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  1. Afficher la liste des agents                          ║\n";
        std::cout << "║  2. Configurer les attaques d'un agent                    ║\n";
        std::cout << "║  3. Afficher toutes les configurations                    ║\n";
        std::cout << "║  4. Sauvegarder les configurations                        ║\n";
        std::cout << "║  5. Charger les configurations                            ║\n";
        std::cout << "║  6. Afficher l'architecture                               ║\n";
        std::cout << "║  7. Quitter                                               ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Choisir une option (1-7):                                  ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }

    void displayArchitecture() {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              ARCHITECTURE DE L'INTERFACE GRAPHIQUE          ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";
        std::cout << "┌─────────────────────────────────────────────────────┐\n";
        std::cout << "│ BARRE DE MENU                                       │\n";
        std::cout << "├─────────────────────────────────────────────────────┤\n";
        std::cout << "│                                                     │\n";
        std::cout << "│  ┌──────────────┐  ┌──────────────┐  ┌──────────┐  │\n";
        std::cout << "│  │  RED TEAM    │  │  BLUE TEAM   │  │  MANAGER │  │\n";
        std::cout << "│  │              │  │              │  │          │  │\n";
        std::cout << "│  │ ◻ Recon     │  │ ◻ Monitor   │  │ ◻ Tools  │  │\n";
        std::cout << "│  │ ◻ Exploit   │  │ ◻ Response  │  │ ◻ Config │  │\n";
        std::cout << "│  │ ◻ BlackHat  │  │             │  │ ◻ Logs   │  │\n";
        std::cout << "│  └──────────────┘  └──────────────┘  └──────────┘  │\n";
        std::cout << "│                                                     │\n";
        std::cout << "├──────────────┬──────────────────────┬───────────────┤\n";
        std::cout << "│   CONTRÔLES  │    ZONE PRINCIPALE   │    LOGS       │\n";
        std::cout << "│              │                      │               │\n";
        std::cout << "│  [▶ Lancer]  │ [Agent sélectionné]  │ [Historique]  │\n";
        std::cout << "│  [⏸ Pause]   │ [Détails exécution]  │ [Erreurs]     │\n";
        std::cout << "│  [⏹ Stop]    │ [État en temps réel]  │ [Status]      │\n";
        std::cout << "│  [⚙ Config]  │                      │               │\n";
        std::cout << "└──────────────┴──────────────────────┴───────────────┘\n";
    }

    void configureAgentAttacks() {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║              CONFIGURATION DES ATTAQUES PAR AGENT           ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════╝\n\n";

        // Afficher les agents disponibles
        const auto& configs = attack_manager.getAllConfigs();
        std::cout << "Agents disponibles:\n";
        for (size_t i = 0; i < configs.size(); ++i) {
            std::cout << "  " << (i+1) << ". " << configs[i].getAgentName() << "\n";
        }

        std::cout << "\nChoisir un agent (1-" << configs.size() << "): ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice < 1 || choice > configs.size()) {
            std::cout << "[!] Choix invalide\n";
            return;
        }

        auto* config = const_cast<AgentAttackConfig*>(&configs[choice-1]);
        
        // Afficher la configuration actuelle
        config->printConfiguration();

        // Menu de sélection
        bool configuring = true;
        while (configuring) {
            std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
            std::cout << "║              MENU DE CONFIGURATION                         ║\n";
            std::cout << "╠════════════════════════════════════════════════════════════╣\n";
            std::cout << "║  1. Ajouter une technique                                 ║\n";
            std::cout << "║  2. Supprimer une technique                               ║\n";
            std::cout << "║  3. Afficher toutes les techniques                        ║\n";
            std::cout << "║  4. Afficher la configuration actuelle                    ║\n";
            std::cout << "║  5. Retour au menu principal                              ║\n";
            std::cout << "╠════════════════════════════════════════════════════════════╣\n";
            std::cout << "║ Choisir une option (1-5):                                  ║\n";
            std::cout << "╚════════════════════════════════════════════════════════════╝\n";

            int subchoice;
            std::cin >> subchoice;
            std::cin.ignore();

            if (subchoice == 1) {
                // Ajouter une technique
                std::cout << "\nTechniques disponibles:\n";
                const auto& techs = config->getAvailableTechniques();
                for (size_t i = 0; i < techs.size(); ++i) {
                    char marker = techs[i].selected ? '✓' : ' ';
                    std::cout << "  " << (i+1) << ". [" << marker << "] " 
                             << techs[i].name << " (" << techs[i].phase << ")\n";
                }

                std::cout << "\nChoisir une technique (1-" << techs.size() << "): ";
                int tech_choice;
                std::cin >> tech_choice;
                std::cin.ignore();

                if (tech_choice > 0 && tech_choice <= techs.size()) {
                    config->selectTechnique(techs[tech_choice-1].id);
                    std::cout << "[✓] Technique ajoutée\n";
                } else {
                    std::cout << "[!] Choix invalide\n";
                }
            } else if (subchoice == 2) {
                // Supprimer une technique
                const auto& selected = config->getSelectedTechniques();
                if (selected.empty()) {
                    std::cout << "[!] Aucune technique sélectionnée\n";
                    continue;
                }

                std::cout << "\nTechniques sélectionnées:\n";
                for (size_t i = 0; i < selected.size(); ++i) {
                    std::cout << "  " << (i+1) << ". " << selected[i] << "\n";
                }

                std::cout << "\nChoisir une technique à supprimer (1-" << selected.size() << "): ";
                int del_choice;
                std::cin >> del_choice;
                std::cin.ignore();

                if (del_choice > 0 && del_choice <= selected.size()) {
                    config->deselectTechnique(selected[del_choice-1]);
                    std::cout << "[✓] Technique supprimée\n";
                } else {
                    std::cout << "[!] Choix invalide\n";
                }
            } else if (subchoice == 3) {
                // Afficher toutes les techniques
                const auto& techs = config->getAvailableTechniques();
                std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
                std::cout << "║            TOUTES LES TECHNIQUES DISPONIBLES               ║\n";
                std::cout << "╠════════════════════════════════════════════════════════════╣\n";

                std::vector<std::string> phases = {"reconnaissance", "scan", "exploit", "lateral", "persist"};
                for (const auto& phase : phases) {
                    std::cout << "║\n║  [" << phase << "]\n";
                    for (const auto& tech : techs) {
                        if (tech.phase == phase) {
                            char marker = tech.selected ? '✓' : ' ';
                            std::cout << "║    [" << marker << "] " << tech.name 
                                     << " (" << tech.success_rate << "%)\n";
                        }
                    }
                }
                std::cout << "║\n╚════════════════════════════════════════════════════════════╝\n";
            } else if (subchoice == 4) {
                // Afficher la configuration actuelle
                config->printConfiguration();
            } else if (subchoice == 5) {
                configuring = false;
            } else {
                std::cout << "[!] Option invalide\n";
            }
        }
    }

    void displayAgents() {
        AgentPanelManager manager;
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                 LISTE DES AGENTS                           ║\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";

        const auto& panels = manager.getPanels();
        for (size_t i = 0; i < panels.size(); ++i) {
            std::cout << "║ " << (i+1) << ". " << panels[i].name << "\n";
            std::cout << "║    Type: " << panels[i].type << "\n";
            std::cout << "║    Status: " << panels[i].status << "\n";
            if (i < panels.size() - 1) {
                std::cout << "╠════════════════════════════════════════════════════════════╣\n";
            }
        }
        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }

    void run() {
        displayWelcome();
        attack_manager.loadConfiguration();

        bool running = true;
        while (running) {
            displayMenu();
            
            int choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice) {
                case 1:
                    displayAgents();
                    break;
                case 2:
                    configureAgentAttacks();
                    break;
                case 3:
                    attack_manager.printAllConfigurations();
                    break;
                case 4:
                    attack_manager.saveConfiguration();
                    break;
                case 5:
                    attack_manager.loadConfiguration();
                    break;
                case 6:
                    displayArchitecture();
                    break;
                case 7:
                    std::cout << "[✓] Au revoir!\n";
                    running = false;
                    break;
                default:
                    std::cout << "[!] Option invalide\n";
            }
        }
    }
};

// ============================================================================
// SECTION 4 : POINT D'ENTRÉE
// ============================================================================

int main(int argc, char* argv[]) {
    std::cout << "\n";
    SimpleGUI gui(1200, 800, "Agent Manager - Unified GUI System");
    gui.run();
    
    return 0;
}
