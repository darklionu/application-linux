# Gestionnaire d'Agents Centralisé (C++)

Une application C++ moderne qui regroupe et gère tous les agents (Red Team et Blue Team).

## Caractéristiques

✅ **Interface interactive** - Menu centralisé pour gérer tous les agents  
✅ **Agents Red Team** - Reconnaissance, exploitation, black hat  
✅ **Agents Blue Team** - Monitoring, réponse aux incidents  
✅ **Scénarios** - Simulation complète, séquences personnalisées  
✅ **Mode verbeux** - Informations détaillées d'exécution  

## Compilation

```bash
make -f Makefile_cpp all
```

Ou utiliser le target `run` pour compiler et exécuter directement :

```bash
make -f Makefile_cpp run
```

## Utilisation

### Lancer l'application

```bash
./agent_manager_app
```

### Options de ligne de commande

```bash
./agent_manager_app --verbose   # Mode verbeux
./agent_manager_app --help      # Aide
```

### Menu Principal

```
[RED TEAM]
  1. Exécuter Red Recon Agent
  2. Exécuter Red Exploit Agent
  3. Exécuter Red BlackHat Agent
  4. Exécuter tous les agents Red Team

[BLUE TEAM]
  5. Exécuter Blue Monitor Agent
  6. Exécuter Blue Response Agent
  7. Exécuter tous les agents Blue Team

[RED TEAM MANAGER]
  8. Exécuter Red Team Manager

[SCÉNARIOS]
  9. Simulation complète (Red Team → Blue Team)
  10. Exécuter une séquence personnalisée

[GESTION]
  11. Afficher les agents disponibles
  12. Basculer mode verbeux
  0. Quitter
```

## Agents Disponibles

| Agent | Type | Chemin |
|-------|------|--------|
| Red Recon Agent | red_recon | `./agent/red_recon_agent/red_recon_agent` |
| Red Exploit Agent | red_exploit | `./agent/red_exploit_agent/red_exploit_agent` |
| Red BlackHat Agent | red_blackhat | `./agent/red_blackhat_agent/red_blackhat_agent` |
| Blue Monitor Agent | blue_monitor | `./agent_blue_team/monitor_agent/blue_monitor_agent` |
| Blue Response Agent | blue_response | `./agent_blue_team/response_agent/blue_response_agent` |
| Red Team Manager | red_manager | `./agent_red_team/agent_manager_app` |

## Scénarios Disponibles

### 1. Exécution individuelle
Lancer un seul agent à la fois pour des tests ciblés.

### 2. Exécution par équipe
- **Red Team complète** : Reconnaissance → Exploitation → Black Hat
- **Blue Team complète** : Monitoring → Réponse

### 3. Simulation complète
Red Team lance les attaques, Blue Team détecte et répond automatiquement.

### 4. Séquence personnalisée
Construire une séquence d'exécution d'agents à la carte.

## Architecture

### Classes principales

#### `Agent`
Représente un agent exécutable avec :
- Nom et chemin
- Type (red_recon, red_exploit, etc.)
- État (activé/désactivé)
- Méthode `execute()` pour lancer l'agent

#### `AgentManager`
Gestionnaire principal qui :
- Initialise tous les agents
- Affiche le menu interactif
- Gère l'exécution des agents
- Permet les séquences personnalisées

## Compilation avancée

```bash
# Nettoyer les fichiers compilés
make -f Makefile_cpp clean

# Afficher l'aide
make -f Makefile_cpp help

# Compiler avec mode verbeux
make -f Makefile_cpp verbose
```

## Architecture C++

- **C++17** - Standard moderne
- **STL** - std::vector, std::map, std::string
- **Pas de dépendances externes** - Utilise seulement la stdlib

## Fichiers du projet

```
agent_manager_cpp.h          # En-têtes (classes Agent et AgentManager)
agent_manager_cpp.cpp        # Implémentation
main_agent_manager.cpp       # Point d'entrée
Makefile_cpp                 # Script de compilation
agent_manager_app           # Exécutable compilé
```

## Notes

- Les chemins des agents sont configurables dans `AgentManager::initializeAgents()`
- L'application utilise `system()` pour lancer les exécutables
- Les noms d'agents et leurs chemins peuvent être modifiés avant compilation
- Mode verbeux pour débogage et suivi des opérations

## Exemple d'exécution

```bash
$ ./agent_manager_app

======================================================================
                GESTIONNAIRE D'AGENTS CENTRALISÉ
======================================================================

[RED TEAM]
  1. Exécuter Red Recon Agent
  2. Exécuter Red Exploit Agent
  3. Exécuter Red BlackHat Agent
  ...

Choix: 9

[*] Simulation complète en cours...
[*] Phase 1: Red Team Actions
[+] Exécution de l'agent: Red Recon Agent
...
[✓] Simulation complète terminée
```

## Contact

Pour toute modification ou amélioration, éditer les fichiers source et recompiler avec `make -f Makefile_cpp all`.
