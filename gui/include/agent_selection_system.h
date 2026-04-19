#ifndef AGENT_SELECTION_SYSTEM_H
#define AGENT_SELECTION_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <algorithm>

/**
 * @struct SelectedAgent
 * @brief Représente un agent sélectionné avec sa configuration
 */
struct SelectedAgent {
    std::string name;
    std::string type;
    std::string path;
    bool enabled;
    bool selected;
    bool is_active;                               // Pour JARVIS
    int execution_order;
    int priority;                                 // Pour JARVIS (1-10)
    std::vector<std::string> dependencies;        // Pour JARVIS
    std::map<std::string, std::string> parameters;

    SelectedAgent(const std::string& n, const std::string& t, const std::string& p)
        : name(n), type(t), path(p), enabled(true), selected(false), 
          is_active(true), execution_order(0), priority(5) {}
};

/**
 * @class AgentSelectionSystem
 * @brief Système amélioré de sélection d'agents avec support de multi-sélection
 * 
 * Features:
 * - Sélection/désélection d'agents
 * - Ordre d'exécution
 * - Filtrage par type
 * - Validation des dépendances
 * - Historique de sélection
 */
class AgentSelectionSystem {
private:
    std::vector<SelectedAgent> available_agents;
    std::vector<SelectedAgent> selected_agents;
    std::vector<std::vector<SelectedAgent>> selection_history;
    std::map<std::string, std::vector<std::string>> dependencies;
    int max_agents_selected;
    
public:
    AgentSelectionSystem(int max_agents = 5)
        : max_agents_selected(max_agents) {
        initializeDependencies();
    }
    
    ~AgentSelectionSystem() = default;

    /**
     * @brief Ajoute un agent à la liste des agents disponibles
     */
    void addAvailableAgent(const std::string& name, const std::string& type, 
                          const std::string& path, bool enabled = true) {
        available_agents.emplace_back(name, type, path);
        auto& agent = available_agents.back();
        agent.enabled = enabled;
        
        // Initialiser les dépendances en fonction du type
        if (type == "red_exploit") {
            agent.dependencies = {"red_recon"};
            agent.priority = 7;
        } else if (type == "red_blackhat") {
            agent.dependencies = {"red_exploit", "red_recon"};
            agent.priority = 9;
        } else if (type == "red_recon") {
            agent.priority = 6;
        } else if (type == "blue_response") {
            agent.dependencies = {"blue_monitor"};
            agent.priority = 8;
        } else if (type == "blue_monitor") {
            agent.priority = 6;
        } else {
            agent.priority = 5;
        }
    }

    /**
     * @brief Sélectionne un agent (multi-sélection supportée)
     */
    bool selectAgent(const std::string& agent_name) {
        // Vérifier si l'agent existe
        auto it = std::find_if(available_agents.begin(), available_agents.end(),
            [&agent_name](const SelectedAgent& a) { return a.name == agent_name; });
        
        if (it == available_agents.end() || !it->enabled) {
            std::cerr << "❌ Agent '" << agent_name << "' non trouvé ou désactivé\n";
            return false;
        }

        // Vérifier si déjà sélectionné
        auto exists = std::find_if(selected_agents.begin(), selected_agents.end(),
            [&agent_name](const SelectedAgent& a) { return a.name == agent_name; });
        
        if (exists != selected_agents.end()) {
            std::cout << "⚠️  Agent '" << agent_name << "' déjà sélectionné\n";
            return false;
        }

        // Vérifier la limite
        if ((int)selected_agents.size() >= max_agents_selected) {
            std::cerr << "❌ Limite d'agents sélectionnés atteinte (" << max_agents_selected << ")\n";
            return false;
        }

        // Ajouter l'agent
        SelectedAgent new_agent(*it);
        new_agent.selected = true;
        new_agent.execution_order = selected_agents.size() + 1;
        selected_agents.push_back(new_agent);

        std::cout << "✓ Agent '" << agent_name << "' sélectionné (ordre: " 
                  << new_agent.execution_order << ")\n";
        return true;
    }

    /**
     * @brief Désélectionne un agent
     */
    bool deselectAgent(const std::string& agent_name) {
        auto it = std::find_if(selected_agents.begin(), selected_agents.end(),
            [&agent_name](const SelectedAgent& a) { return a.name == agent_name; });
        
        if (it == selected_agents.end()) {
            std::cerr << "❌ Agent '" << agent_name << "' n'est pas sélectionné\n";
            return false;
        }

        selected_agents.erase(it);
        
        // Réajuster l'ordre d'exécution
        for (size_t i = 0; i < selected_agents.size(); i++) {
            selected_agents[i].execution_order = i + 1;
        }

        std::cout << "✓ Agent '" << agent_name << "' désélectionné\n";
        return true;
    }

    /**
     * @brief Sélectionne tous les agents d'un type donné
     */
    void selectAgentsByType(const std::string& type) {
        for (auto& agent : available_agents) {
            if (agent.type == type && agent.enabled) {
                selectAgent(agent.name);
            }
        }
        std::cout << "✓ Tous les agents de type '" << type << "' ont été sélectionnés\n";
    }

    /**
     * @brief Désélectionne tous les agents
     */
    void clearSelection() {
        selected_agents.clear();
        std::cout << "✓ Toute la sélection a été effacée\n";
    }

    /**
     * @brief Change l'ordre d'exécution de deux agents
     */
    bool swapExecutionOrder(int index1, int index2) {
        if (index1 < 0 || index1 >= (int)selected_agents.size() ||
            index2 < 0 || index2 >= (int)selected_agents.size()) {
            std::cerr << "❌ Index invalide\n";
            return false;
        }

        // Inverser l'ordre
        std::swap(selected_agents[index1], selected_agents[index2]);
        
        // Réajuster les numéros d'ordre
        for (size_t i = 0; i < selected_agents.size(); i++) {
            selected_agents[i].execution_order = i + 1;
        }

        return true;
    }

    /**
     * @brief Filtre les agents disponibles par type
     */
    std::vector<SelectedAgent> getAgentsByType(const std::string& type) const {
        std::vector<SelectedAgent> filtered;
        for (const auto& agent : available_agents) {
            if (agent.type == type) {
                filtered.push_back(agent);
            }
        }
        return filtered;
    }

    /**
     * @brief Valide les dépendances entre agents sélectionnés
     */
    bool validateDependencies() {
        for (const auto& agent : selected_agents) {
            auto it = dependencies.find(agent.type);
            if (it != dependencies.end()) {
                for (const auto& dep_type : it->second) {
                    bool dep_found = false;
                    for (const auto& sel_agent : selected_agents) {
                        if (sel_agent.type == dep_type) {
                            dep_found = true;
                            break;
                        }
                    }
                    if (!dep_found) {
                        std::cerr << "❌ L'agent '" << agent.name 
                                  << "' dépend d'un agent de type '" << dep_type << "'\n";
                        return false;
                    }
                }
            }
        }
        return true;
    }

    /**
     * @brief Sauvegarde l'état actuel de la sélection dans l'historique
     */
    void saveSelectionSnapshot() {
        selection_history.push_back(selected_agents);
        std::cout << "✓ Snapshot de sélection sauvegardé\n";
    }

    /**
     * @brief Restaure une sélection précédente
     */
    bool restoreSelectionSnapshot(int history_index) {
        if (history_index < 0 || history_index >= (int)selection_history.size()) {
            std::cerr << "❌ Index d'historique invalide\n";
            return false;
        }
        selected_agents = selection_history[history_index];
        std::cout << "✓ Sélection restaurée du snapshot " << history_index << "\n";
        return true;
    }

    /**
     * @brief Ajoute un paramètre à un agent sélectionné
     */
    bool setAgentParameter(const std::string& agent_name, 
                          const std::string& param_name,
                          const std::string& param_value) {
        auto it = std::find_if(selected_agents.begin(), selected_agents.end(),
            [&agent_name](const SelectedAgent& a) { return a.name == agent_name; });
        
        if (it == selected_agents.end()) {
            std::cerr << "❌ Agent '" << agent_name << "' n'est pas sélectionné\n";
            return false;
        }

        it->parameters[param_name] = param_value;
        return true;
    }

    // ========== Getters ==========
    
    const std::vector<SelectedAgent>& getSelectedAgents() const {
        return selected_agents;
    }

    const std::vector<SelectedAgent>& getAvailableAgents() const {
        return available_agents;
    }

    size_t getSelectedCount() const {
        return selected_agents.size();
    }

    size_t getAvailableCount() const {
        return available_agents.size();
    }

    int getMaxAgentsAllowed() const {
        return max_agents_selected;
    }

    /**
     * @brief Affiche l'état actuel de la sélection
     */
    void printSelectionStatus() const {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           ÉTAT DE LA SÉLECTION D'AGENTS\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";
        std::cout << "║ Agents sélectionnés: " << selected_agents.size() 
                  << " / " << max_agents_selected << "\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";

        if (selected_agents.empty()) {
            std::cout << "║ ⚠️  Aucun agent sélectionné\n";
        } else {
            for (const auto& agent : selected_agents) {
                std::cout << "║ [" << agent.execution_order << "] " 
                          << agent.name << " (" << agent.type << ")\n";
            }
        }

        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }

    /**
     * @brief Affiche les agents disponibles par type
     */
    void printAvailableAgents() const {
        std::cout << "\n╔════════════════════════════════════════════════════════════╗\n";
        std::cout << "║           AGENTS DISPONIBLES\n";
        std::cout << "╠════════════════════════════════════════════════════════════╣\n";

        std::map<std::string, std::vector<SelectedAgent>> by_type;
        for (const auto& agent : available_agents) {
            by_type[agent.type].push_back(agent);
        }

        for (const auto& [type, agents] : by_type) {
            std::cout << "║ Type: " << type << "\n";
            for (const auto& agent : agents) {
                std::string status = agent.enabled ? "[OK]" : "[NO]";
                std::cout << "║   " << status << " " << agent.name << "\n";
            }
        }

        std::cout << "╚════════════════════════════════════════════════════════════╝\n";
    }

private:
    /**
     * @brief Initialise les dépendances entre types d'agents
     */
    void initializeDependencies() {
        // Les agents d'exploitation dépendent des agents de reconnaissance
        dependencies["red_exploit"] = {"red_recon"};
        dependencies["red_blackhat"] = {"red_exploit", "red_recon"};
        // Les agents de réponse blue dépendent des agents de monitoring
        dependencies["blue_response"] = {"blue_monitor"};
    }
};

#endif // AGENT_SELECTION_SYSTEM_H
