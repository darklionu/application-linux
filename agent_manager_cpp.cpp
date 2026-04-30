#include "agent_manager_cpp.h"
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// ============================================================================
// Implémentation de la classe Agent
// ============================================================================

int Agent::execute() const {
    if (!enabled) {
        std::cerr << "[!] Agent " << name << " est désactivé" << std::endl;
        return 1;
    }

    // Validation du chemin
    if (access(path.c_str(), F_OK) != 0) {
        std::cerr << "[!] Agent introuvable: " << path << std::endl;
        return 1;
    }
    
    if (access(path.c_str(), X_OK) != 0) {
        std::cerr << "[!] Agent non exécutable: " << path << std::endl;
        return 1;
    }

    std::cout << "\n[+] Exécution de l'agent: " << name << std::endl;
    std::cout << "[+] Chemin: " << path << std::endl;
    std::cout << "[+] Type: " << type << std::endl;
    std::cout << std::string(60, '-') << std::endl;

    // Utiliser fork/exec au lieu de system pour la sécurité
    pid_t pid = fork();
    
    if (pid == -1) {
        std::cerr << "[!] Erreur lors du fork de " << name << std::endl;
        return 1;
    }
    
    if (pid == 0) {
        // Processus enfant
        char* const argv[] = {const_cast<char*>(path.c_str()), NULL};
        execv(path.c_str(), argv);
        // Si execv retourne, il y a une erreur
        exit(127);
    } else {
        // Processus parent
        int status;
        waitpid(pid, &status, 0);
        
        int exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        
        std::cout << std::string(60, '-') << std::endl;
        std::cout << "[+] Exécution terminée pour " << name 
                  << " (code: " << exit_code << ")" << std::endl;
        
        return exit_code != 0;
    }
}

void Agent::printInfo() const {
    std::cout << std::left << std::setw(25) << name
              << std::setw(15) << type
              << std::setw(10) << (enabled ? "Actif" : "Inactif") << std::endl;
}

// ============================================================================
// Implémentation de la classe AgentManager
// ============================================================================

AgentManager::AgentManager() : isVerbose(false) {
    initializeAgents();
}

void AgentManager::initializeAgents() {
    // Red Team Agents
    addAgent(Agent("Red Recon Agent", "./agent/red_recon_agent/red_recon_agent", "red_recon"));
    addAgent(Agent("Red Exploit Agent", "./agent/red_exploit_agent/red_exploit_agent", "red_exploit"));
    addAgent(Agent("Red BlackHat Agent", "./agent/red_blackhat_agent/red_blackhat_agent", "red_blackhat"));

    // Blue Team Agents
    addAgent(Agent("Blue Monitor Agent", "./agent_blue_team/monitor_agent/blue_monitor_agent", "blue_monitor"));
    addAgent(Agent("Blue Response Agent", "./agent_blue_team/response_agent/blue_response_agent", "blue_response"));

    // Agent Red Team Manager
    addAgent(Agent("Red Team Manager", "./agent_red_team/agent_manager_app", "red_manager"));
}

void AgentManager::addAgent(const Agent& agent) {
    agentIndex[agent.getName()] = agents.size();
    agents.push_back(agent);
}

void AgentManager::displayMenu() {
    std::cout << "\n" << std::string(70, '=') << std::endl;
    std::cout << std::setw(35) << "GESTIONNAIRE D'AGENTS CENTRALISÉ" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    std::cout << "\n[RED TEAM]" << std::endl;
    std::cout << "  1. Exécuter Red Recon Agent" << std::endl;
    std::cout << "  2. Exécuter Red Exploit Agent" << std::endl;
    std::cout << "  3. Exécuter Red BlackHat Agent" << std::endl;
    std::cout << "  4. Exécuter tous les agents Red Team" << std::endl;

    std::cout << "\n[BLUE TEAM]" << std::endl;
    std::cout << "  5. Exécuter Blue Monitor Agent" << std::endl;
    std::cout << "  6. Exécuter Blue Response Agent" << std::endl;
    std::cout << "  7. Exécuter tous les agents Blue Team" << std::endl;

    std::cout << "\n[RED TEAM MANAGER]" << std::endl;
    std::cout << "  8. Exécuter Red Team Manager" << std::endl;

    std::cout << "\n[SCÉNARIOS]" << std::endl;
    std::cout << "  9. Simulation complète (Red Team → Blue Team)" << std::endl;
    std::cout << "  10. Exécuter une séquence personnalisée" << std::endl;

    std::cout << "\n[GESTION]" << std::endl;
    std::cout << "  11. Afficher les agents disponibles" << std::endl;
    std::cout << "  12. Basculer mode verbeux" << std::endl;
    std::cout << "  0. Quitter" << std::endl;
    std::cout << std::string(70, '=') << std::endl;
    std::cout << "Choix: ";
}

void AgentManager::displayAgents() {
    std::cout << "\n" << std::string(70, '-') << std::endl;
    std::cout << "AGENTS DISPONIBLES:" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    std::cout << std::left << std::setw(25) << "Nom"
              << std::setw(15) << "Type"
              << std::setw(10) << "Etat" << std::endl;
    std::cout << std::string(70, '-') << std::endl;

    for (int i = 0; i < (int)agents.size(); ++i) {
        std::cout << std::setw(2) << (i + 1) << ". ";
        agents[i].printInfo();
    }
    std::cout << std::string(70, '-') << std::endl;
}

void AgentManager::executeAgent(int index) {
    if (index < 0 || index >= (int)agents.size()) {
        std::cerr << "[!] Index d'agent invalide" << std::endl;
        return;
    }
    agents[index].execute();
}

void AgentManager::executeMultipleAgents(const std::vector<int>& indices) {
    for (int index : indices) {
        executeAgent(index);
        std::cout << std::endl;
    }
}

void AgentManager::executeAllAgents() {
    std::cout << "\n[*] Exécution de tous les agents..." << std::endl;
    std::vector<int> allIndices;
    for (int i = 0; i < (int)agents.size(); ++i) {
        allIndices.push_back(i);
    }
    executeMultipleAgents(allIndices);
}

int AgentManager::selectAgent() {
    int choice;
    std::cout << "Entrez le numéro de l'agent (1-" << agents.size() << ", 0 pour annuler): ";
    if (!(std::cin >> choice) || choice < 0 || choice > (int)agents.size()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return -1;
    }
    return choice - 1;
}

void AgentManager::start() {
    int choice = 1;

    while (choice != 0) {
        displayMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                executeAgent(0);  // Red Recon
                break;
            case 2:
                executeAgent(1);  // Red Exploit
                break;
            case 3:
                executeAgent(2);  // Red BlackHat
                break;
            case 4: {
                std::vector<int> redTeam = {0, 1, 2};
                executeMultipleAgents(redTeam);
                break;
            }
            case 5:
                executeAgent(3);  // Blue Monitor
                break;
            case 6:
                executeAgent(4);  // Blue Response
                break;
            case 7: {
                std::vector<int> blueTeam = {3, 4};
                executeMultipleAgents(blueTeam);
                break;
            }
            case 8:
                executeAgent(5);  // Red Team Manager
                break;
            case 9: {
                std::cout << "\n[*] Simulation complète en cours..." << std::endl;
                std::cout << "[*] Phase 1: Red Team Actions" << std::endl;
                std::vector<int> redTeam = {0, 1, 2};
                executeMultipleAgents(redTeam);
                std::cout << "\n[*] Phase 2: Blue Team Detection et Réponse" << std::endl;
                std::vector<int> blueTeam = {3, 4};
                executeMultipleAgents(blueTeam);
                std::cout << "\n[✓] Simulation complète terminée" << std::endl;
                break;
            }
            case 10: {
                std::cout << "\n[*] Construction d'une séquence personnalisée" << std::endl;
                displayAgents();
                std::vector<int> customSequence;
                int agentChoice;
                while (true) {
                    agentChoice = selectAgent();
                    if (agentChoice == -1) break;
                    customSequence.push_back(agentChoice);
                    std::cout << "[+] Agent ajouté à la séquence" << std::endl;
                }
                if (!customSequence.empty()) {
                    executeMultipleAgents(customSequence);
                }
                break;
            }
            case 11:
                displayAgents();
                break;
            case 12:
                isVerbose = !isVerbose;
                std::cout << "[*] Mode verbeux: " << (isVerbose ? "ACTIVÉ" : "DÉSACTIVÉ") << std::endl;
                break;
            case 0:
                std::cout << "\n[*] Au revoir!" << std::endl;
                break;
            default:
                std::cout << "[!] Choix invalide" << std::endl;
        }
    }
}
