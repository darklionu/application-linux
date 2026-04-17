# Guide d'Installation - Gestionnaire d'Agents C++

## 📋 Prérequis

- **G++** (compilateur C++) - version 7+ recommandée
- **Make** - pour la gestion de la compilation
- **Linux/Unix** - macOS ou Windows (WSL)

### Vérifier les prérequis

```bash
g++ --version
make --version
```

## 🔧 Installation et Compilation

### Méthode 1 : Utiliser le Makefile directement

```bash
# Dans le répertoire racine du projet
cd /workspaces/application-linux

# Compiler
make -f Makefile_cpp all

# Compiler et nettoyer d'abord
make -f Makefile_cpp rebuild

# Nettoyer les fichiers compilés
make -f Makefile_cpp clean
```

### Méthode 2 : Utiliser le script de gestion (recommandé)

```bash
cd /workspaces/application-linux

# Compiler
./agent_manager.sh build

# Compiler et exécuter
./agent_manager.sh run

# Nettoyer
./agent_manager.sh clean

# Recompiler complètement
./agent_manager.sh rebuild

# Compiler avec symboles de debug
./agent_manager.sh debug

# Exécuter en mode verbeux
./agent_manager.sh verbose

# Afficher l'aide
./agent_manager.sh help
```

### Méthode 3 : Démarrage rapide

```bash
cd /workspaces/application-linux
./quick_start_manager.sh
```

## ✅ Vérification de l'installation

Après la compilation, vous devriez voir :

```
g++ -std=c++17 -Wall -Wextra -O2 -c agent_manager_cpp.cpp -o agent_manager_cpp.o
g++ -std=c++17 -Wall -Wextra -O2 -c main_agent_manager.cpp -o main_agent_manager.o
g++ -std=c++17 -Wall -Wextra -O2 -o agent_manager_app agent_manager_cpp.o main_agent_manager.o
[✓] Compilation réussie: agent_manager_app
```

Vérifier que l'exécutable existe :

```bash
ls -la ./agent_manager_app
# Output: -rwxr-xr-x ... agent_manager_app
```

## 🚀 Première exécution

```bash
./agent_manager_app
```

Vous devriez voir le menu principal avec les options disponibles.

## 📁 Structure des fichiers générés

Après la compilation :

```
/workspaces/application-linux/
├── agent_manager_cpp.h          # En-tête (classes)
├── agent_manager_cpp.cpp        # Implémentation
├── agent_manager_cpp.o          # Objet compilé
├── main_agent_manager.cpp       # Point d'entrée
├── main_agent_manager.o         # Objet compilé
├── agent_config.h               # Configuration des agents
├── agent_manager_app            # Exécutable final ✓
├── Makefile_cpp                 # Script de compilation
├── agent_manager.sh             # Script de gestion
├── quick_start_manager.sh       # Démarrage rapide
└── README_AGENT_MANAGER.md      # Documentation
```

## 🔍 Dépannage

### Erreur: "g++: command not found"

Installer g++:

```bash
# Ubuntu/Debian
sudo apt-get install build-essential

# macOS
brew install gcc
```

### Erreur: "make: command not found"

Installer make:

```bash
# Ubuntu/Debian
sudo apt-get install make

# macOS
brew install make
```

### Chemins des agents incorrects

Éditer `agent_manager_cpp.cpp` et modifier la fonction `initializeAgents()` :

```cpp
void AgentManager::initializeAgents() {
    addAgent(Agent("Nom", "/chemin/vers/agent", "type"));
}
```

Ensuite, recompiler :

```bash
make -f Makefile_cpp rebuild
```

### Permissions insuffisantes

Si l'exécutable ne s'exécute pas :

```bash
chmod +x agent_manager_app
./agent_manager_app
```

## 🎯 Options de compilation avancées

### Compiler avec optimisation maximale

Éditer `Makefile_cpp` et modifier :

```makefile
CXXFLAGS := -std=c++17 -Wall -Wextra -O3
```

### Compiler avec avertissements stricts

```makefile
CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -O2
```

### Compiler avec symboles de debug

```bash
./agent_manager.sh debug
```

## 📚 Utilisation après installation

### Exécution simple

```bash
./agent_manager_app
```

### Exécution avec options

```bash
./agent_manager_app --verbose
./agent_manager_app --help
```

### Via le script de gestion

```bash
./agent_manager.sh run          # Compile et exécute
./agent_manager.sh verbose      # Avec mode verbeux
./agent_manager.sh clean        # Nettoie les fichiers
```

## 🛠️ Customisation

### Ajouter un nouvel agent

1. Éditer `agent_manager_cpp.cpp` dans `initializeAgents()` :

```cpp
void AgentManager::initializeAgents() {
    // ... agents existants ...
    addAgent(Agent("Mon Agent", "./mon_agent", "mon_type"));
}
```

2. Recompiler :

```bash
make -f Makefile_cpp rebuild
```

### Modifier les chemins des agents

Éditer les chemins dans `agent_manager_cpp.cpp` (ligne ~58-62) ou utiliser `agent_config.h`.

## 📖 Documentation complète

Consulter : [README_AGENT_MANAGER.md](README_AGENT_MANAGER.md)

## ✨ C'est prêt !

L'application est maintenant installée et prête à être utilisée. Vous pouvez :

- Lancer des agents individuellement
- Lancer des équipes complètes
- Exécuter des simulations
- Créer des séquences personnalisées

Pour toute question, consultez la documentation ou les fichiers source.

---

**Dernière mise à jour**: 17 Avril 2026
