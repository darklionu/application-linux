# Système GUI Unifié - Documentation Complète

## 📋 Vue d'ensemble

Le système GUI unifié consolide tous les fichiers d'interface graphique en un **seul fichier monolithique** (`unified_gui_system.cpp`) qui contient:

- ✅ Classes Button (boutons)
- ✅ Classes AgentPanel (panneaux d'agents)
- ✅ Classes GUIWindow (fenêtre principale)
- ✅ Classes SimpleGUI (interface simplifiée)
- ✅ **Nouveau: Système de configuration d'attaques par agent**
- ✅ **Nouveau: Gestionnaire central d'attaques**

## 🎯 Fonctionnalités Principales

### 1. **Interface Graphique Unifiée**
- Une seule source de vérité pour l'UI
- Réduction de la complexité des dépendances
- Maintenance plus facile

### 2. **Configuration d'Attaques par Agent** ⭐ NOUVEAU

Vous pouvez maintenant **sélectionner précisément** ce que chaque robot attaque:

```
Agent: Red Recon Agent
Techniques disponibles:
  [reconnaissance]
    ✓ Port Scanning (nmap)
    ✓ DNS Enumeration
    [ ] OSINT Gathering
  [scan]
    ✓ Vulnerability Scan
    [ ] Service Enumeration
  [exploit]
    ✓ Credential Brute Force
    [ ] SQL Injection
```

### 3. **Structure des Attaques**

Chaque technique a:
- **ID unique** (ex: `recon_001`)
- **Nom descriptif** (ex: `Port Scanning (nmap)`)
- **Phase de la kill chain** (reconnaissance, scan, exploit, lateral, persist)
- **Taux de succès** (pourcentage)
- **État** (sélectionnée ou non)

### 4. **Phases d'Attaque Disponibles**

```
1. Reconnaissance    - Collecte d'informations (reconnaissance, OSINT)
2. Scan             - Enumération des services et vulnérabilités
3. Exploit          - Exploitation des vulnérabilités trouvées
4. Lateral          - Mouvement latéral et escalade de privilèges
5. Persist          - Établissement de la persistance
```

## 🚀 Guide d'Utilisation

### Compilation

```bash
# Méthode 1: Makefile unifié
cd /workspaces/application-linux/gui
make -f Makefile_unified all

# Méthode 2: Compilation manuelle
g++ -std=c++11 -Wall -Wextra -O2 -o build/agent_manager_gui_unified src/unified_gui_system.cpp
```

### Exécution

```bash
# Exécution directe
./build/agent_manager_gui_unified

# Ou avec make
make -f Makefile_unified run
```

### Menu Principal

```
1. Afficher la liste des agents
2. Configurer les attaques d'un agent     ⭐ NOUVELLE FONCTIONNALITÉ
3. Afficher toutes les configurations     ⭐ NOUVELLE FONCTIONNALITÉ
4. Sauvegarder les configurations         ⭐ NOUVELLE FONCTIONNALITÉ
5. Charger les configurations             ⭐ NOUVELLE FONCTIONNALITÉ
6. Afficher l'architecture
7. Quitter
```

## 📝 Exemple d'Utilisation

### Étape 1: Configurer Red Recon Agent

```
[Menu Principal]
Choisir une option: 2

[Configuration d'attaques par agent]
Agents disponibles:
  1. Red Recon Agent
  2. Red Exploit Agent
  3. Red BlackHat Agent

Choisir un agent (1-3): 1

[Configuration de Red Recon Agent]
  [reconnaissance]
    [✓] Port Scanning (nmap) (90%)
    [ ] DNS Enumeration (85%)
    [ ] OSINT Gathering (80%)
  ...
```

### Étape 2: Ajouter une Technique

```
[Menu de configuration]
Choisir une option: 1

Techniques disponibles:
  1. [✓] Port Scanning
  2. [ ] DNS Enumeration
  3. [ ] OSINT Gathering
  ...

Choisir une technique (1-20): 2
[✓] Technique ajoutée
```

### Étape 3: Sauvegarder la Configuration

```
[Menu Principal]
Choisir une option: 4
[✓] Configuration sauvegardée dans: attack_config.txt
```

## 💾 Format de Sauvegarde

Fichier `attack_config.txt`:
```
[red_recon]
recon_001
recon_003
scan_001
exploit_002

[red_exploit]
scan_001
exploit_001
exploit_003
lateral_001

[red_blackhat]
exploit_003
lateral_002
persist_001
```

## 🏗️ Architecture du Code

### Hiérarchie des Classes

```
SimpleGUI (Interface principale)
    ├── AgentPanelManager
    │   └── AgentPanel[] (panneaux d'agents)
    ├── GUIWindow (fenêtre graphique)
    ├── Button (boutons de contrôle)
    └── AttackConfigurationManager ⭐
        └── AgentAttackConfig[] (configurations par agent)
            └── AttackTechnique[] (techniques disponibles)
```

### Flot de Données

```
User Input
    ↓
Menu Handler
    ↓
AgentAttackConfig Manager
    ↓
Select/Deselect Technique
    ↓
Save/Load Configuration
    ↓
attack_config.txt
```

## 🔧 Intégration avec les Agents C

### Comment utiliser la configuration côté agent

Les fichiers C des agents peuvent lire le fichier `attack_config.txt`:

```c
// Dans agent_engine.c
#include <stdio.h>

void load_agent_attacks(const char *agent_id) {
    FILE *f = fopen("attack_config.txt", "r");
    char line[256];
    char current_agent[64] = "";
    
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '[') {
            sscanf(line, "[%[^]]", current_agent);
        } else if (strcmp(current_agent, agent_id) == 0) {
            // Charger la technique pour cet agent
            printf("Attacking with: %s\n", line);
        }
    }
    fclose(f);
}
```

## 📊 Statistiques des Attaques

Le système propose **17 techniques** différentes:

| Phase | Nombre | Exemples |
|-------|--------|----------|
| Reconnaissance | 3 | Port Scanning, DNS Enum, OSINT |
| Scan | 3 | Vulnerability Scan, Service Enum, Web Scan |
| Exploit | 4 | Brute Force, SQL Injection, RCE, Password Cracking |
| Lateral | 3 | Privilege Escalation, Pass-the-Hash, Kerberos |
| Persist | 3 | Backdoor, Rootkit, Scheduled Tasks |

## 🎓 Classes Principales

### `AttackTechnique`
```cpp
struct AttackTechnique {
    std::string id;           // unique identifier
    std::string name;         // descriptive name
    std::string phase;        // attack phase
    int success_rate;         // success percentage
    bool available;           // is available
    bool selected;            // is selected
};
```

### `AgentAttackConfig`
```cpp
class AgentAttackConfig {
    std::string agent_id;
    std::vector<AttackTechnique> available_techniques;
    std::vector<std::string> selected_techniques;
    
    void selectTechnique(const std::string& tech_id);
    void deselectTechnique(const std::string& tech_id);
    void toggleTechnique(const std::string& tech_id);
};
```

### `AttackConfigurationManager`
```cpp
class AttackConfigurationManager {
    std::vector<AgentAttackConfig> agent_configs;
    
    AgentAttackConfig* getAgentConfig(const std::string& agent_id);
    void saveConfiguration();
    void loadConfiguration();
};
```

## 🔄 Migration depuis l'Ancienne Structure

### Avant (Multiple Fichiers)

```
gui/
├── src/
│   ├── gui_button.cpp
│   ├── gui_agent_panel.cpp
│   ├── main_gui_example.cpp
│   └── simple_gui.cpp
└── include/
    ├── gui_button.h
    ├── gui_agent_panel.h
    └── gui_window.h
```

### Après (Fichier Unique)

```
gui/
├── src/
│   └── unified_gui_system.cpp  ⭐ TOUT EN UN SEUL FICHIER
├── build/
│   └── agent_manager_gui_unified (exécutable)
└── Makefile_unified
```

## 📈 Avantages

✅ **Monolithique** - Un seul fichier à maintenir
✅ **Sélection d'attaques** - Choix granulaire par agent
✅ **Persistance** - Sauvegarde/chargement des configurations
✅ **Évolutif** - Facile d'ajouter de nouvelles techniques
✅ **Intégration facile** - Format simple pour les agents C
✅ **Pas de dépendances externes** - Juste C++11 et StdLib

## 🐛 Dépannage

### Le programme ne compile pas
```bash
# Vérifier la version de g++
g++ --version

# Compiler avec plus de détails
g++ -std=c++11 -v -Wall -Wextra -O2 src/unified_gui_system.cpp -o build/agent_manager_gui_unified
```

### Les configurations ne se sauvegardent pas
```bash
# Vérifier les permissions
ls -la attack_config.txt

# Vérifier le répertoire de travail
pwd
```

## 🚀 Améliorations Futures

- [ ] Intégration graphique complète avec Qt/SFML
- [ ] Exportation en JSON/YAML
- [ ] Validation des techniques
- [ ] Statistiques d'exécution
- [ ] Historique des configurations
- [ ] Versioning des configurations

## 📞 Support

Pour des questions ou des bug reports, consultez le README principal du projet.

---

**Version:** 1.0  
**Date:** 2026-04-18  
**Status:** Production Ready ✅
