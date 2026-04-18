# Démarrage Rapide - Système GUI Unifié

## 🚀 En 3 Étapes

### 1️⃣ Compiler

```bash
cd /workspaces/application-linux/gui
chmod +x build_and_run_unified.sh
./build_and_run_unified.sh
```

**Ou manuellement:**
```bash
g++ -std=c++11 -Wall -Wextra -O2 -o build/agent_manager_gui_unified src/unified_gui_system.cpp
```

### 2️⃣ Lancer

```bash
./build/agent_manager_gui_unified
```

### 3️⃣ Configurer les Attaques

```
MENU PRINCIPAL
==============
1. Afficher la liste des agents
2. Configurer les attaques d'un agent        ← CLIQUEZ ICI
3. Afficher toutes les configurations
4. Sauvegarder les configurations            ← ET ICI POUR SAUVER
5. Charger les configurations
6. Afficher l'architecture
7. Quitter
```

---

## 📋 Exemple Complet

### Configuration Step-by-Step

```
Choisir une option (1-7): 2

[Configuration d'attaques par agent]
Agents disponibles:
  1. Red Recon Agent
  2. Red Exploit Agent
  3. Red BlackHat Agent

Choisir un agent (1-3): 1

[Configuration actuelle]
╔════════════════════════════════════════════════════════════╗
║     Configuration d'attaques - Red Recon Agent
╠════════════════════════════════════════════════════════════╣
║
║  [reconnaissance]
║    [ ] Port Scanning (nmap) (90%)
║    [ ] DNS Enumeration (85%)
║    [ ] OSINT Gathering (80%)
║  ...

[Menu de configuration]
1. Ajouter une technique
2. Supprimer une technique
3. Afficher toutes les techniques
4. Afficher la configuration actuelle
5. Retour au menu principal

Choisir une option (1-5): 1

Techniques disponibles:
  1. [ ] Port Scanning (reconnaissance)
  2. [ ] DNS Enumeration (reconnaissance)
  ...

Choisir une technique (1-20): 1
[✓] Technique ajoutée
```

---

## 🎯 Cas d'Usage

### Scénario 1: Red Team Reconnaissance

1. **Agent:** Red Recon Agent
2. **Attaques à sélectionner:**
   - Port Scanning (nmap)
   - DNS Enumeration
   - OSINT Gathering

### Scénario 2: Red Team Exploitation

1. **Agent:** Red Exploit Agent
2. **Attaques à sélectionner:**
   - Vulnerability Scan
   - Credential Brute Force
   - SQL Injection
   - Remote Code Execution

### Scénario 3: Advanced Persistence

1. **Agent:** Red BlackHat Agent
2. **Attaques à sélectionner:**
   - Privilege Escalation
   - Backdoor Installation
   - Rootkit Deployment
   - Scheduled Task Creation

---

## 💾 Fichier de Configuration

Après la sauvegarde, vous verrez un fichier `attack_config.txt`:

```
[red_recon]
recon_001
recon_002
scan_001

[red_exploit]
exploit_001
exploit_002
exploit_003

[red_blackhat]
lateral_001
persist_001
```

---

## 🔄 Workflow Typique

```
1. Lancer le GUI
        ↓
2. Afficher les agents
        ↓
3. Configurer Red Recon Agent
   ├─ Ajouter: Port Scanning
   ├─ Ajouter: Vulnerability Scan
   └─ Ajouter: Credential Brute Force
        ↓
4. Configurer Red Exploit Agent
   ├─ Ajouter: SQL Injection
   ├─ Ajouter: RCE
   └─ Ajouter: Privilege Escalation
        ↓
5. Configurer Red BlackHat Agent
   ├─ Ajouter: Backdoor
   ├─ Ajouter: Rootkit
   └─ Ajouter: Persistence
        ↓
6. Sauvegarder la configuration
        ↓
7. Utiliser les agents avec les configurations
```

---

## 🧪 Tester l'Intégration

Après configuration, testez avec le programme d'intégration:

```bash
# Compiler
gcc -o build/agent_executor_integration agent_executor_integration.c

# Exécuter
./build/agent_executor_integration

# Vous devriez voir:
# - Les techniques chargées depuis attack_config.txt
# - Simulation d'exécution
# - Statistiques de succès/échec
```

---

## 🆘 FAQ Rapide

**Q: Le fichier unified_gui_system.cpp est énorme?**
A: Oui! C'est normal - tout en un seul fichier pour la consolidation.

**Q: Peut-on éditer attack_config.txt manuellement?**
A: Oui! Format simple, facile à éditer.

**Q: Les configurations se perdent au redémarrage?**
A: Elles sont sauvegardées dans attack_config.txt.

**Q: Comment ajouter une nouvelle technique?**
A: Éditez la fonction `initDefaultTechniques()` dans le fichier.

**Q: L'interface est en console?**
A: Oui pour l'instant. GUI graphique possible avec Qt/SFML.

---

## 📊 Statistiques

- **1 fichier** principal (unified_gui_system.cpp)
- **17 techniques** d'attaque disponibles
- **5 phases** de la kill chain
- **3 agents** Red Team configurables
- **100% C++11** compatible

---

## 🎓 Concepts Clés

```
Agent              → Entité qui exécute des attaques
Technique          → Une attaque spécifique
Phase              → Étape du kill chain
Configuration      → Ensemble de techniques sélectionnées pour un agent
AttackConfigManager → Gestionnaire central des configurations
```

---

**Bon hacking! 🚀**
