/**
 * Configuration des agents
 * Ce fichier peut être édité pour ajouter ou modifier les agents
 * Recompiler après modification: make -f Makefile_cpp clean && make -f Makefile_cpp all
 */

#ifndef AGENT_CONFIG_H
#define AGENT_CONFIG_H

#include <vector>
#include <string>

// Structure de configuration d'un agent
struct AgentConfig {
    std::string name;
    std::string path;
    std::string type;
    bool enabled;
};

// Configuration des agents
static const std::vector<AgentConfig> AGENTS_CONFIG = {
    // RED TEAM - Phase de Reconnaissance
    {
        .name = "Red Recon Agent",
        .path = "./agent/red_recon_agent/red_recon_agent",
        .type = "red_recon",
        .enabled = true
    },

    // RED TEAM - Phase d'Exploitation
    {
        .name = "Red Exploit Agent",
        .path = "./agent/red_exploit_agent/red_exploit_agent",
        .type = "red_exploit",
        .enabled = true
    },

    // RED TEAM - Black Hat Operations
    {
        .name = "Red BlackHat Agent",
        .path = "./agent/red_blackhat_agent/red_blackhat_agent",
        .type = "red_blackhat",
        .enabled = true
    },

    // BLUE TEAM - Monitoring et Détection
    {
        .name = "Blue Monitor Agent",
        .path = "./agent_blue_team/monitor_agent/blue_monitor_agent",
        .type = "blue_monitor",
        .enabled = true
    },

    // BLUE TEAM - Réponse aux Incidents
    {
        .name = "Blue Response Agent",
        .path = "./agent_blue_team/response_agent/blue_response_agent",
        .type = "blue_response",
        .enabled = true
    },

    // RED TEAM - Gestionnaire Principal
    {
        .name = "Red Team Manager",
        .path = "./agent_red_team/agent_manager_app",
        .type = "red_manager",
        .enabled = true
    }
};

// Configuration générale
struct GeneralConfig {
    static constexpr bool ENABLE_LOGGING = true;
    static constexpr bool SHOW_TIMESTAMPS = true;
    static constexpr int MAX_CONCURRENT_AGENTS = 1;
    static constexpr int TIMEOUT_SECONDS = 300;
};

#endif // AGENT_CONFIG_H
