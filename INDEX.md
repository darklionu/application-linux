# 📑 Index - Gestionnaire d'Agents C++

## 🎯 Démarrage rapide

### Pour les impatients (30 secondes)
```bash
cd /workspaces/application-linux
./quick_start_manager.sh
```

### Première fois ?
```bash
./agent_manager.sh build    # Compiler
./agent_manager_app         # Exécuter
```

---

## 📂 Fichiers du projet

### 🔧 Code source (À développer)
- **`agent_manager_cpp.h`** - Classes `Agent` et `AgentManager`
- **`agent_manager_cpp.cpp`** - Implémentation complète
- **`main_agent_manager.cpp`** - Point d'entrée
- **`agent_config.h`** - Configuration des agents

### 🏗️ Build & Compilation
- **`Makefile_cpp`** - Script de compilation g++
- **`agent_manager.sh`** - Gestion complète (build, run, clean, debug)
- **`quick_start_manager.sh`** - Démarrage rapide

### 📝 Documentation (À lire)
- **`README_AGENT_MANAGER.md`** - **📍 COMMENCER ICI** - Vue d'ensemble complète
- **`INSTALLATION.md`** - Guide d'installation détaillé
- **`CHANGELOG.md`** - Historique et statistiques
- **`INDEX.md`** - Ce fichier

### 📚 Ressources
- **`examples_usage.sh`** - Exemples d'utilisation

### 🎯 Exécutable
- **`agent_manager_app`** - Application compilée et prête

---

## 📖 Guide de lecture

### Pour les développeurs
1. **`README_AGENT_MANAGER.md`** - Comprendre l'architecture
2. **`agent_manager_cpp.h`** - Voir les interfaces
3. **`agent_manager_cpp.cpp`** - Étudier l'implémentation
4. **`CHANGELOG.md`** - Voir ce qui a été fait

### Pour les utilisateurs
1. **`INSTALLATION.md`** - Installer l'application
2. **`README_AGENT_MANAGER.md`** - Apprendre à l'utiliser
3. **`examples_usage.sh`** - Voir des exemples pratiques
4. **Lancer**: `./agent_manager_app`

---

## 🚀 Commandes essentielles

| Commande | Description |
|----------|-------------|
| `make -f Makefile_cpp all` | Compiler |
| `./agent_manager_app` | Exécuter |
| `./agent_manager.sh run` | Compiler + Exécuter |
| `./agent_manager.sh debug` | Compiler avec debug (gdb) |
| `./agent_manager.sh clean` | Nettoyer les fichiers compilés |
| `./agent_manager.sh help` | Afficher l'aide |
| `./examples_usage.sh` | Voir les exemples |

---

## 🎮 Agents disponibles

### Red Team (Attaque)
- 🔴 **Red Recon Agent** - Reconnaissance
- 🔴 **Red Exploit Agent** - Exploitation
- 🔴 **Red BlackHat Agent** - Black Hat Ops

### Blue Team (Défense)
- 🔵 **Blue Monitor Agent** - Monitoring
- 🔵 **Blue Response Agent** - Réponse aux incidents

### Gestion
- 🟡 **Red Team Manager** - Gestionnaire

---

## ✨ Fonctionnalités principales

- ✅ Menu interactif centralisé
- ✅ Exécution individuelle d'agents
- ✅ Exécution par équipe
- ✅ Simulation complète (Red Team → Blue Team)
- ✅ Séquences personnalisées d'agents
- ✅ Mode verbeux pour le débogage
- ✅ Interface colorée et intuitive

---

## 🔍 Dépannage rapide

### L'application ne compile pas ?
```bash
# Installer les outils nécessaires
sudo apt-get install build-essential

# Recompiler
make -f Makefile_cpp rebuild
```

### Les chemins des agents sont incorrects ?
Éditer `agent_manager_cpp.cpp` et modifier la fonction `initializeAgents()`.

### Comment ajouter un agent ?
Voir `INSTALLATION.md` section "Customisation" → "Ajouter un nouvel agent"

---

## 📊 Statistiques

- **Total fichiers**: 12
- **Lignes de code**: ~400
- **Taille exécutable**: 49 KB
- **Standard C++**: C++17
- **Compilateur**: g++
- **Agents**: 6

---

## 🎯 Prochaines étapes

1. **Lire** `README_AGENT_MANAGER.md` pour comprendre l'application
2. **Exécuter** `./agent_manager_app` pour voir le menu
3. **Tester** les différentes options
4. **Lire** `INSTALLATION.md` pour les détails techniques
5. **Customiser** en fonction de vos besoins

---

## 📞 Ressources

| Document | Contenu |
|----------|---------|
| `README_AGENT_MANAGER.md` | Fonctionnalités et utilisation |
| `INSTALLATION.md` | Installation et troubleshooting |
| `CHANGELOG.md` | Changements et statistiques |
| `agent_manager_cpp.h` | Interfaces (API) |
| `agent_manager_cpp.cpp` | Implémentation |

---

## ✅ Checklist de démarrage

- [ ] Lire ce fichier (INDEX.md)
- [ ] Lire `README_AGENT_MANAGER.md`
- [ ] Exécuter `./quick_start_manager.sh`
- [ ] Explorer le menu de l'application
- [ ] Tester les différentes options
- [ ] Lire `INSTALLATION.md` pour les détails
- [ ] Customiser selon vos besoins

---

**Dernière mise à jour**: 17 Avril 2026  
**État**: ✅ Production-ready
