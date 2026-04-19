#ifndef AGENT_JARVIS_INTERFACE_H
#define AGENT_JARVIS_INTERFACE_H

#include "agent_selection_system.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <string>

/**
 * @class JarvisInterface
 * @brief Interface simplifiée JARVIS pour sélection d'agents
 * 
 * Version fonctionnelle et compilable
 */
class JarvisInterface {
private:
    AgentSelectionSystem& agent_system;

public:
    explicit JarvisInterface(AgentSelectionSystem& sys) : agent_system(sys) {}

    /**
     * @brief Affiche l'interface JARVIS texte
     */
    void displayJarvis() {
        clearScreen();
        displayBanner();
        displayAgents();
        displayControls();
    }

    /**
     * @brief Lance l'interface interactive
     */
    void launch() {
        std::string cmd;
        
        while (true) {
            displayJarvis();
            std::cout << "\n[JARVIS] > ";
            std::getline(std::cin, cmd);
            
            // Supprimer les espaces en début et fin
            size_t start = cmd.find_first_not_of(" \t");
            if (start != std::string::npos) {
                cmd = cmd.substr(start);
                size_t end = cmd.find_last_not_of(" \t");
                cmd = cmd.substr(0, end + 1);
            }
            
            if (cmd == "exit" || cmd == "quit") {
                std::cout << "\n[JARVIS] Fermeture de l'interface...\n";
                break;
            }
            
            if (!cmd.empty()) {
                processCommand(cmd);
            }
        }
    }

private:
    void clearScreen() {
        #ifdef _WIN32
            std::system("cls");
        #else
            std::system("clear");
        #endif
    }

    void displayBanner() {
        std::cout << "\n";
        std::cout << " ╔═══════════════════════════════════════════════════════════════╗\n";
        std::cout << " ║                                                               ║\n";
        std::cout << " ║              J.A.R.V.I.S - AGENT SELECTION v2.0              ║\n";
        std::cout << " ║          Interactive Reality Vector Integration System         ║\n";
        std::cout << " ║                                                               ║\n";
        std::cout << " ╚═══════════════════════════════════════════════════════════════╝\n\n";
    }

    void displayAgents() {
        auto agents = agent_system.getAvailableAgents();
        auto selected = agent_system.getSelectedAgents();
        
        std::cout << " ┌─ AGENTS DISPONIBLES ──────────────────────────────────────────┐\n";
        
        for (size_t i = 0; i < agents.size(); i++) {
            const auto& agent = agents[i];
            
            // Vérifier si sélectionné
            bool is_selected = std::any_of(selected.begin(), selected.end(),
                [&](const auto& s) { return s.name == agent.name; });
            
            std::string checkbox = is_selected ? "[X]" : "[ ]";
            std::string status = agent.enabled ? "*" : "-";
            
            std::cout << " │ " << checkbox << " " << status << " " 
                      << agent.name << " (" << agent.type << ")\n";
        }
        
        std::cout << " └───────────────────────────────────────────────────────────────┘\n\n";
        
        // Afficher la sélection
        std::cout << " AGENTS SÉLECTIONNÉS (" << selected.size() << "/" 
                  << agent_system.getMaxAgentsAllowed() << "):\n";
        
        if (selected.empty()) {
            std::cout << "  (Aucun)\n";
        } else {
            for (size_t i = 0; i < selected.size(); i++) {
                std::cout << "  [" << (i+1) << "] " << selected[i].name 
                          << " (" << selected[i].type << ")\n";
            }
        }
    }

    void displayControls() {
        std::cout << "\n COMMANDES:\n";
        std::cout << "  select <nom>    - Sélectionner un agent\n";
        std::cout << "  deselect <nom>  - Désélectionner un agent\n";
        std::cout << "  clear           - Effacer la sélection\n";
        std::cout << "  swap <i1> <i2>  - Inverser l'ordre\n";
        std::cout << "  validate        - Vérifier les dépendances\n";
        std::cout << "  status          - État détaillé\n";
        std::cout << "  execute         - Exécuter les agents\n";
        std::cout << "  exit            - Quitter\n\n";
    }

    void processCommand(const std::string& cmd) {
        std::istringstream iss(cmd);
        std::string operation, arg1, arg2;
        iss >> operation >> arg1 >> arg2;
        
        // Capturer le reste de la ligne pour les arguments multi-mots
        std::string rest_args;
        if (operation == "select" || operation == "deselect") {
            // Capturer tout ce qui suit la commande
            size_t first_space = cmd.find(' ');
            if (first_space != std::string::npos) {
                rest_args = cmd.substr(first_space + 1);
                // Supprimer les espaces au début
                rest_args.erase(0, rest_args.find_first_not_of(" \t"));
            }
        }
        
        if (operation == "select") {
            if (!rest_args.empty()) {
                agent_system.selectAgent(rest_args);
            } else if (!arg1.empty()) {
                agent_system.selectAgent(arg1);
            } else {
                std::cerr << "Erreur: spécifiez un nom d'agent\n";
            }
        }
        else if (operation == "deselect") {
            if (!rest_args.empty()) {
                agent_system.deselectAgent(rest_args);
            } else if (!arg1.empty()) {
                agent_system.deselectAgent(arg1);
            } else {
                std::cerr << "Erreur: spécifiez un nom d'agent\n";
            }
        }
        else if (operation == "clear") {
            agent_system.clearSelection();
        }
        else if (operation == "swap") {
            try {
                int idx1 = std::stoi(arg1) - 1;
                int idx2 = std::stoi(arg2) - 1;
                agent_system.swapExecutionOrder(idx1, idx2);
            } catch (...) {
                std::cerr << "Erreur: indices invalides\n";
            }
        }
        else if (operation == "validate") {
            if (agent_system.validateDependencies()) {
                std::cout << "OK: Dépendances valides\n";
            } else {
                std::cout << "ERREUR: Dépendances non valides\n";
            }
        }
        else if (operation == "status") {
            agent_system.printSelectionStatus();
        }
        else if (operation == "execute") {
            std::cout << "Exécution des agents sélectionnés...\n";
            auto selected = agent_system.getSelectedAgents();
            for (const auto& agent : selected) {
                std::cout << "  -> " << agent.name << "\n";
            }
        }
        else if (operation == "exit") {
            std::cout << "Au revoir!\n";
            return; // Quitter le traitement des commandes
        }
        else if (!operation.empty()) {
            std::cerr << "Commande inconnue: " << operation << "\n";
        }
    }
};

#endif // AGENT_JARVIS_INTERFACE_H
