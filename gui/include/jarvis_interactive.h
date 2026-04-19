#ifndef JARVIS_INTERACTIVE_H
#define JARVIS_INTERACTIVE_H

#include "agent_selection_system.h"
#include "attack_config.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <filesystem>
#include <sstream>
#include <unistd.h>

class JarvisInteractive {
private:
    AgentSelectionSystem& agent_system;
    AttackConfig attack_config;
    
    // Historique des commandes
    std::vector<std::string> command_history;
    int history_index = -1;
    
    // Suggestions d'autocomplétion
    std::vector<std::string> getCommandSuggestions(const std::string& prefix) {
        std::vector<std::string> suggestions;
        const std::vector<std::string> commands = {
            "select", "deselect", "clear", "swap", "validate", 
            "status", "execute", "snapshot", "help", "exit"
        };
        
        std::string lower_prefix = prefix;
        std::transform(lower_prefix.begin(), lower_prefix.end(), 
                      lower_prefix.begin(), ::tolower);
        
        for (const auto& cmd : commands) {
            if (cmd.find(lower_prefix) == 0) {
                suggestions.push_back(cmd);
            }
        }
        return suggestions;
    }
    
    std::vector<std::string> getAgentSuggestions(const std::string& prefix) {
        std::vector<std::string> suggestions;
        const auto& agents = agent_system.getAvailableAgents();
        
        std::string lower_prefix = prefix;
        std::transform(lower_prefix.begin(), lower_prefix.end(), 
                      lower_prefix.begin(), ::tolower);
        
        for (const auto& agent : agents) {
            std::string agent_name_lower = agent.name;
            std::transform(agent_name_lower.begin(), agent_name_lower.end(), 
                          agent_name_lower.begin(), ::tolower);
            
            if (agent_name_lower.find(lower_prefix) != std::string::npos) {
                suggestions.push_back(agent.name);
            }
        }
        return suggestions;
    }
    
public:
    explicit JarvisInteractive(AgentSelectionSystem& sys, 
                              const std::string& workspace_base = "/workspaces/application-linux") 
        : agent_system(sys), attack_config(workspace_base) {}
    
    void displayBanner() {
        std::cout << "\n";
        std::cout << " ╔═══════════════════════════════════════════════════════════╗\n";
        std::cout << " ║                                                           ║\n";
        std::cout << " ║           J.A.R.V.I.S - INTERACTIVE AGENT CONTROL         ║\n";
        std::cout << " ║         (Just A Rather Very Intelligent System)           ║\n";
        std::cout << " ║                                                           ║\n";
        std::cout << " ╚═══════════════════════════════════════════════════════════╝\n\n";
    }
    
    void displayStatus() {
        const auto& agents = agent_system.getAvailableAgents();
        const auto& selected = agent_system.getSelectedAgents();
        
        std::cout << " ┌─ AGENTS DISPONIBLES ──────────────────────────────────────┐\n";
        
        for (size_t i = 0; i < agents.size(); i++) {
            const auto& agent = agents[i];
            bool is_selected = std::any_of(selected.begin(), selected.end(),
                [&](const auto& s) { return s.name == agent.name; });
            
            std::string checkbox = is_selected ? "[X]" : "[ ]";
            std::string status = agent.enabled ? "*" : "-";
            std::string type_info = agent.type;
            
            std::cout << " │ " << checkbox << " " << status << " ";
            std::cout << std::left << std::setw(30) << agent.name;
            std::cout << " (" << type_info << ")\n";
        }
        
        std::cout << " └───────────────────────────────────────────────────────────┘\n";
        std::cout << " Sélectionnés: " << selected.size() << "/" 
                  << agent_system.getMaxAgentsAllowed() << "\n\n";
    }
    
    void displayHelp() {
        std::cout << "\n ┌─ COMMANDES DISPONIBLES ───────────────────────────────────┐\n";
        std::cout << " │                                                             │\n";
        std::cout << " │  select <nom>          Sélectionner un agent               │\n";
        std::cout << " │  deselect <nom>        Désélectionner un agent             │\n";
        std::cout << " │  clear                 Effacer toute la sélection          │\n";
        std::cout << " │  swap <n1> <n2>        Inverser l'ordre d'exécution        │\n";
        std::cout << " │  validate              Vérifier les dépendances            │\n";
        std::cout << " │  status                Afficher l'état détaillé            │\n";
        std::cout << " │  execute               Lancer l'exécution                  │\n";
        std::cout << " │  snapshot              Sauvegarder l'état actuel           │\n";
        std::cout << " │  help                  Afficher cette aide                 │\n";
        std::cout << " │  exit                  Quitter J.A.R.V.I.S                │\n";
        std::cout << " │                                                             │\n";
        std::cout << " └─────────────────────────────────────────────────────────────┘\n\n";
    }
    
    void displaySuggestions(const std::vector<std::string>& suggestions) {
        if (suggestions.empty()) return;
        
        std::cout << "\n 💡 Suggestions:\n";
        for (size_t i = 0; i < suggestions.size() && i < 5; i++) {
            std::cout << "    " << (i + 1) << ". " << suggestions[i] << "\n";
        }
    }
    
    void displayAgentInfo(const std::string& agent_name) {
        const auto& agents = agent_system.getAvailableAgents();
        
        auto it = std::find_if(agents.begin(), agents.end(),
            [&](const auto& a) { return a.name == agent_name; });
        
        if (it == agents.end()) return;
        
        std::cout << "\n ┌─ INFO AGENT ───────────────────────────────────────────────┐\n";
        std::cout << " │ Nom:          " << std::left << std::setw(45) << it->name << "│\n";
        std::cout << " │ Type:         " << std::left << std::setw(45) << it->type << "│\n";
        std::cout << " │ Chemin:       " << std::left << std::setw(45) << it->path << "│\n";
        std::cout << " │ Priorité:     " << std::left << std::setw(45) << std::to_string(it->priority) << "│\n";
        std::cout << " │ Dépendances:  ";
        
        if (it->dependencies.empty()) {
            std::cout << std::left << std::setw(45) << "(Aucune)" << "│\n";
        } else {
            std::cout << std::left << std::setw(45) << it->dependencies[0] << "│\n";
            for (size_t i = 1; i < it->dependencies.size(); i++) {
                std::cout << " │               " << std::left << std::setw(45) 
                          << it->dependencies[i] << "│\n";
            }
        }
        
        std::cout << " └───────────────────────────────────────────────────────────┘\n\n";
    }
    
    void processCommandWithCompletion(const std::string& cmd) {
        std::istringstream iss(cmd);
        std::string operation, rest_of_line;
        iss >> operation;
        std::getline(iss, rest_of_line);
        
        // Trim leading/trailing spaces
        if (!rest_of_line.empty() && rest_of_line[0] == ' ') {
            rest_of_line = rest_of_line.substr(1);
        }
        
        if (operation == "select") {
            if (!rest_of_line.empty()) {
                if (agent_system.selectAgent(rest_of_line)) {
                    std::cout << " ✓ Agent sélectionné: " << rest_of_line << "\n";
                    displayAgentInfo(rest_of_line);
                } else {
                    std::cout << " ✗ Impossible de sélectionner cet agent\n";
                    auto suggestions = getAgentSuggestions(rest_of_line);
                    displaySuggestions(suggestions);
                }
            }
        }
        else if (operation == "deselect") {
            if (!rest_of_line.empty()) {
                if (agent_system.deselectAgent(rest_of_line)) {
                    std::cout << " ✓ Agent désélectionné: " << rest_of_line << "\n";
                } else {
                    std::cout << " ✗ Agent introuvable\n";
                }
            }
        }
        else if (operation == "clear") {
            agent_system.clearSelection();
            std::cout << " ✓ Sélection effacée\n";
        }
        else if (operation == "swap") {
            try {
                int idx1 = std::stoi(rest_of_line) - 1;
                int idx2 = 0;
                size_t space_pos = rest_of_line.find(' ');
                if (space_pos != std::string::npos) {
                    idx2 = std::stoi(rest_of_line.substr(space_pos)) - 1;
                    if (agent_system.swapExecutionOrder(idx1, idx2)) {
                        std::cout << " ✓ Ordre échangé: " << (idx1 + 1) 
                                  << " <-> " << (idx2 + 1) << "\n";
                    }
                }
            } catch (...) {
                std::cout << " ✗ Indices invalides (usage: swap <n1> <n2>)\n";
            }
        }
        else if (operation == "validate") {
            if (agent_system.validateDependencies()) {
                std::cout << " ✓ Dépendances valides\n";
            } else {
                std::cout << " ✗ Dépendances manquantes ou incompatibles\n";
            }
        }
        else if (operation == "status") {
            displayStatus();
        }
        else if (operation == "execute") {
            std::cout << "\n ⚠ EXÉCUTION DES AGENTS SÉLECTIONNÉS:\n";
            const auto& selected = agent_system.getSelectedAgents();
            for (size_t i = 0; i < selected.size(); i++) {
                std::cout << "   [" << (i + 1) << "] " << selected[i].name 
                          << " (" << selected[i].type << ")\n";
            }
            std::cout << "\n Confirmez? (y/n): ";
            std::string confirm;
            std::getline(std::cin, confirm);
            
            // Normaliser la réponse
            if (confirm.empty()) confirm = "n";
            else confirm = confirm[0];
            std::transform(confirm.begin(), confirm.end(), confirm.begin(), ::tolower);
            
            if (confirm == "y" || confirm == "yes") {
                std::cout << "\n ▶ Exécution en cours...\n\n";
                
                int successful = 0;
                int failed = 0;
                
                // Obtenir le répertoire courant et se déplacer si nécessaire
                std::filesystem::path original_cwd = std::filesystem::current_path();
                std::filesystem::path workspace_path = attack_config.getBasePath();
                
                for (size_t idx = 0; idx < selected.size(); idx++) {
                    const auto& agent = selected[idx];
                    std::cout << "   [" << (idx + 1) << "/" << selected.size() << "] " 
                              << agent.name << "\n";
                    
                    // Construire le chemin absolu
                    std::string absolute_path = agent.path;
                    if (!std::filesystem::path(absolute_path).is_absolute()) {
                        absolute_path = (workspace_path / agent.path).string();
                    }
                    
                    std::cout << "       Chemin: " << absolute_path << "\n";
                    
                    // Vérifier que l'agent existe
                    if (!std::filesystem::exists(absolute_path)) {
                        std::cout << "       [✗] Agent non trouvé: " << absolute_path << "\n\n";
                        failed++;
                        continue;
                    }
                    
                    // Changer le répertoire de travail
                    std::filesystem::path agent_dir = std::filesystem::path(absolute_path).parent_path();
                    std::filesystem::current_path(agent_dir);
                    
                    // EXÉCUTION RÉELLE DE L'AGENT avec gestion des outils
                    std::string cmd = absolute_path;
                    int result = std::system(cmd.c_str());
                    
                    if (result == 0) {
                        std::cout << "       [✓] Agent lancé avec succès\n\n";
                        successful++;
                    } else if (result == -1) {
                        std::cout << "       [✗] Erreur: Impossible de lancer l'agent\n\n";
                        failed++;
                    } else {
                        // Certains agents retournent un code non-zéro mais se sont exécutés
                        std::cout << "       [⚠] Agent exécuté (code " << result << ")\n\n";
                        successful++;
                    }
                    
                    // Restaurer le répertoire original
                    std::filesystem::current_path(original_cwd);
                }
                
                std::cout << " ╔════════════════════════════════════════════╗\n";
                std::cout << " ║ RÉSUMÉ D'EXÉCUTION                         ║\n";
                std::cout << " ╠════════════════════════════════════════════╣\n";
                std::cout << " ║ Total:     " << selected.size() << "\n";
                std::cout << " ║ Succès:    " << successful << "\n";
                std::cout << " ║ Erreurs:   " << failed << "\n";
                std::cout << " ╚════════════════════════════════════════════╝\n";
            } else {
                std::cout << " ✗ Exécution annulée\n";
            }
        }
        else if (operation == "snapshot") {
            agent_system.saveSelectionSnapshot();
            std::cout << " ✓ État sauvegardé\n";
        }
        else if (operation == "help") {
            displayHelp();
        }
        else if (operation == "attacks") {
            displayAvailableAttacks();
        }
        else if (operation == "launch") {
            launchAttack(rest_of_line);
        }
        else if (operation == "exit") {
            // Handled in launch()
        }
        else if (!operation.empty()) {
            std::cout << " ✗ Commande inconnue: " << operation << "\n";
            auto suggestions = getCommandSuggestions(operation);
            displaySuggestions(suggestions);
        }
    }
    
    std::string readLineWithCompletion() {
        std::string input;
        std::cout << " [JARVIS] > ";
        std::cout.flush();
        
        std::getline(std::cin, input);
        
        // Trim leading/trailing spaces
        size_t start = input.find_first_not_of(" \t");
        if (start != std::string::npos) {
            size_t end = input.find_last_not_of(" \t");
            input = input.substr(start, end - start + 1);
        } else {
            input = "";
        }
        
        return input;
    }
    
    void displayAvailableAttacks() {
        const auto& attacks = attack_config.getScenarios();
        
        std::cout << "\n ┌─ ATTAQUES DISPONIBLES ────────────────────────────────────┐\n";
        
        for (size_t i = 0; i < attacks.size(); i++) {
            const auto& attack = attacks[i];
            std::cout << " │ [" << (i + 1) << "] " << std::left << std::setw(35) 
                      << attack.name << " (Outil: " << attack.tool << ")\n";
            std::cout << " │     " << attack.description << "\n";
        }
        
        std::cout << " └───────────────────────────────────────────────────────────┘\n\n";
    }
    
    void launchAttack(const std::string& attack_spec) {
        const auto& attacks = attack_config.getScenarios();
        
        try {
            size_t attack_idx = std::stoi(attack_spec) - 1;
            
            if (attack_idx >= attacks.size()) {
                std::cout << " ✗ Attaque invalide\n";
                return;
            }
            
            const auto& attack = attacks[attack_idx];
            
            std::cout << "\n ⚠ LANCEMENT DE L'ATTAQUE:\n";
            std::cout << "   Nom: " << attack.name << "\n";
            std::cout << "   Outil: " << attack.tool << "\n";
            std::cout << "   Cible: " << attack.target << "\n";
            std::cout << "   Durée max: " << attack.max_duration << "s\n";
            
            std::cout << "\n Confirmez? (y/n): ";
            std::string confirm;
            std::getline(std::cin, confirm);
            
            if (confirm.empty()) confirm = "n";
            else confirm = confirm[0];
            std::transform(confirm.begin(), confirm.end(), confirm.begin(), ::tolower);
            
            if (confirm == "y") {
                std::cout << "\n ▶ Attaque en cours...\n\n";
                
                // Construire la commande
                std::string tool_path = attack_config.getToolsPath() + "/" + attack.tool;
                
                // Vérifier si l'outil existe
                if (!std::filesystem::exists(tool_path) && attack.tool != "nmap") {
                    // nmap est une commande système
                    if (attack.tool == "nmap") {
                        tool_path = "nmap";
                    } else {
                        std::cout << "   [✗] Outil non trouvé: " << tool_path << "\n";
                        return;
                    }
                }
                
                std::string cmd = "timeout " + std::to_string(attack.max_duration) + " " + tool_path;
                for (const auto& arg : attack.command_args) {
                    cmd += " " + arg;
                }
                
                std::cout << "   Commande: " << cmd << "\n\n";
                
                int result = std::system(cmd.c_str());
                
                std::cout << "\n   ────────────────────────────────\n";
                if (result == 0 || result == 124) { // 124 = timeout retval
                    std::cout << "   [✓] Attaque exécutée\n";
                } else {
                    std::cout << "   [⚠] Code retour: " << result << "\n";
                }
            } else {
                std::cout << " ✗ Attaque annulée\n";
            }
        } catch (...) {
            std::cout << " ✗ Usage: launch <numéro_attaque>\n";
        }
    }
    
    void launch() {
        displayBanner();
        displayStatus();
        displayHelp();
        
        while (true) {
            std::string cmd = readLineWithCompletion();
            
            if (cmd.empty()) continue;
            
            // Check for exit
            if (cmd == "exit" || cmd == "quit") {
                std::cout << "\n ✓ J.A.R.V.I.S fermé. Au revoir!\n\n";
                break;
            }
            
            // Add to history
            command_history.push_back(cmd);
            
            // Process command
            processCommandWithCompletion(cmd);
            std::cout << "\n";
        }
    }
};

#endif // JARVIS_INTERACTIVE_H
