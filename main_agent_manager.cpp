#include "agent_manager_cpp.h"

int main(int argc, char* argv[]) {
    AgentManager manager;

    // Vérifier les arguments
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--verbose" || arg == "-v") {
            manager.setVerbose(true);
            std::cout << "[*] Mode verbeux activé" << std::endl;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Gestionnaire d'Agents Centralisé" << std::endl;
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -v, --verbose   Activer le mode verbeux" << std::endl;
            std::cout << "  -h, --help      Afficher cette aide" << std::endl;
            return 0;
        }
    }

    // Démarrer le gestionnaire
    manager.start();

    return 0;
}
