#ifndef AGENT_MANAGER_HPP
#define AGENT_MANAGER_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <cstdlib>

/**
 * @class Agent
 * @brief Représente un agent exécutable
 */
class Agent {
private:
    std::string name;
    std::string path;
    std::string type;  // red_recon, red_exploit, red_blackhat, blue_monitor, blue_response
    bool enabled;

public:
    Agent(const std::string& agentName, const std::string& agentPath, const std::string& agentType)
        : name(agentName), path(agentPath), type(agentType), enabled(true) {}

    const std::string& getName() const { return name; }
    const std::string& getPath() const { return path; }
    const std::string& getType() const { return type; }
    bool isEnabled() const { return enabled; }
    void setEnabled(bool state) { enabled = state; }

    int execute() const;
    void printInfo() const;
};

/**
 * @class AgentManager
 * @brief Gestionnaire principal des agents
 */
class AgentManager {
private:
    std::vector<Agent> agents;
    std::map<std::string, int> agentIndex;
    bool isVerbose;

    void initializeAgents();
    void displayMenu();
    void displayAgents();
    int selectAgent();

public:
    AgentManager();
    ~AgentManager() = default;

    void start();
    void addAgent(const Agent& agent);
    void executeAgent(int index);
    void executeMultipleAgents(const std::vector<int>& indices);
    void executeAllAgents();
    void setVerbose(bool verbose) { isVerbose = verbose; }
    int getAgentCount() const { return agents.size(); }
};

#endif // AGENT_MANAGER_HPP
