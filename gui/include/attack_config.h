#ifndef ATTACK_CONFIG_H
#define ATTACK_CONFIG_H

#include <string>
#include <vector>
#include <filesystem>

struct AttackScenario {
    std::string name;
    std::string tool;
    std::string description;
    std::string target;
    std::vector<std::string> command_args;
    bool requires_root;
    int max_duration;
};

class AttackConfig {
private:
    std::vector<AttackScenario> scenarios;
    std::string base_path;
    std::string tools_path;
    
    std::string getAbsolutePath(const std::string& relative_path) {
        std::filesystem::path p(relative_path);
        if (p.is_absolute()) return p.string();
        
        // Si relatif, résoudre par rapport au répertoire de travail
        return (std::filesystem::current_path() / p).string();
    }
    
public:
    AttackConfig(const std::string& base) : base_path(base) {
        tools_path = base_path + "/tools";
        initializeScenarios();
    }
    
    void initializeScenarios() {
        scenarios = {
            // Reconnaissance
            {
                "Ping Test",
                "nmap",
                "Test de ping simple sur cible",
                "localhost",
                {"-sn", "127.0.0.1"},
                false,
                10
            },
            {
                "Port Scan Rapide",
                "nmap",
                "Scan rapide des ports courants",
                "localhost",
                {"-p", "22,80,443,3306,5432", "-T4", "127.0.0.1"},
                false,
                30
            },
            {
                "Scan Complet",
                "nmap",
                "Scan complet avec détection de service",
                "localhost",
                {"-sV", "-O", "127.0.0.1"},
                false,
                60
            },
            {
                "DNS Lookup",
                "nmap",
                "Recherche DNS et résolution",
                "localhost",
                {"-sL", "127.0.0.1"},
                false,
                15
            },
            // Force brute
            {
                "SSH Brute Force Test",
                "john",
                "Test force brute SSH (simulation)",
                "localhost:22",
                {"--wordlist=/usr/share/dict/words", "--max-time=10"},
                false,
                20
            },
            // Attaques Web
            {
                "Test Injection SQL",
                "hdra",
                "Test simulation injection SQL",
                "http://localhost",
                {"-m", "sql", "-u", "http://localhost"},
                false,
                30
            },
            // Vulnerability Scan
            {
                "Scan Vulnérabilités",
                "nmap",
                "Recherche de vulnérabilités connues",
                "localhost",
                {"--script", "vuln", "127.0.0.1"},
                false,
                45
            }
        };
    }
    
    const std::vector<AttackScenario>& getScenarios() const {
        return scenarios;
    }
    
    std::string getToolPath(const std::string& tool_name) const {
        return getAbsolutePath(tools_path + "/" + tool_name);
    }
    
    std::string getBasePath() const {
        return base_path;
    }
    
    std::string getToolsPath() const {
        return tools_path;
    }
    
    bool toolExists(const std::string& tool_name) const {
        std::string path = getToolPath(tool_name);
        return std::filesystem::exists(path);
    }
};

#endif // ATTACK_CONFIG_H
