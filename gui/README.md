# 🎨 Interface Graphique du Gestionnaire d'Agents

Guide pour commencer le développement de l'interface graphique.

## 📁 Structure

```
gui/
├── include/           # En-têtes
│   ├── gui_window.h
│   ├── gui_agent_panel.h
│   └── gui_button.h
├── src/              # Implémentation
│   ├── simple_gui.cpp
│   ├── gui_agent_panel.cpp
│   └── gui_button.cpp
├── assets/           # Images, icônes
├── build/            # Fichiers compilés
├── Makefile          # Build
├── config.ini        # Configuration
└── README.md         # Ce fichier
```

## 🚀 Démarrage rapide

### Étape 1 : Installer le framework graphique

**Option A : SFML (recommandé)**
```bash
sudo apt-get install libsfml-dev
```

**Option B : Qt5**
```bash
sudo apt-get install qt5-default
```

### Étape 2 : Compiler la bibliothèque GUI

```bash
cd gui/
make all
```

### Étape 3 : Intégrer avec l'application principale

Les fichiers d'en-tête dans `include/` sont prêts à être utilisés dans l'application principale.

## 🎯 Architecture de l'interface

```
┌─────────────────────────────────────────────────────────┐
│ BARRE DE MENU (Fichier, Édition, Vue, Aide)            │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  ┌─────────────────────────────────────────────────┐  │
│  │                PANNEAUX D'AGENTS               │  │
│  │                                                 │  │
│  │  ┌─────────────┐ ┌─────────────┐ ┌──────────┐  │  │
│  │  │ RED TEAM    │ │ BLUE TEAM   │ │ MANAGER  │  │  │
│  │  │             │ │             │ │          │  │  │
│  │  │ [Recon]     │ │ [Monitor]   │ │ [Tools]  │  │  │
│  │  │ [Exploit]   │ │ [Response]  │ │          │  │  │
│  │  │ [BlackHat]  │ │             │ │          │  │  │
│  │  └─────────────┘ └─────────────┘ └──────────┘  │  │
│  │                                                 │  │
│  └─────────────────────────────────────────────────┘  │
│                                                         │
│  ┌──────────────────────────────────────────────────┐  │
│  │           ZONE DE CONTRÔLE                      │  │
│  │  [▶ Lancer] [⏸ Pause] [⏹ Stop] [⚙ Config]     │  │
│  │                                                  │  │
│  │  Agent sélectionné: [Nom de l'agent]           │  │
│  │  État: [Running/Ready/Done/Error]              │  │
│  └──────────────────────────────────────────────────┘  │
│                                                         │
│  ┌──────────────────────────────────────────────────┐  │
│  │         LOGS EN TEMPS RÉEL                      │  │
│  │                                                  │  │
│  │ [12:34:56] Agent lancé: Red Recon Agent        │  │
│  │ [12:34:57] Phase 1 complète                    │  │
│  │ [12:34:58] Résultats: XXX trouvés              │  │
│  │                                                  │  │
│  └──────────────────────────────────────────────────┘  │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

## 🔧 Composants disponibles

### 1. **GUIWindow**
Classe principale pour la fenêtre graphique.

```cpp
#include "gui_window.h"

GUIWindow window(1200, 800, "Agent Manager");
window.create();
window.run();
```

### 2. **AgentPanelManager**
Gestion des panneaux d'agents.

```cpp
#include "gui_agent_panel.h"

AgentPanelManager manager;
manager.selectPanel(0);  // Sélectionner le 1er agent
manager.updatePanelStatus(0, "Running");
```

### 3. **Button**
Classe pour les boutons.

```cpp
#include "gui_button.h"

Button btn("Lancer", 100, 100);
btn.setCallback([]() { 
    std::cout << "Bouton cliqué!" << std::endl; 
});
```

## 📝 Fichiers d'en-tête

- **`gui_window.h`** - Fenêtre principale
- **`gui_agent_panel.h`** - Panneaux des agents
- **`gui_button.h`** - Boutons interactifs

## 🎨 Thème et personnalisation

Éditer `config.ini` pour:
- Couleurs
- Dimensions de fenêtre
- Taille des panneaux
- Fréquence de rafraîchissement

## 📚 Ressources pour continuer

### SFML
- [Documentation SFML](https://www.sfml-dev.org/documentation/2.5.1/)
- Tutoriels graphiques
- Gestion d'événements

### Qt
- [Documentation Qt](https://doc.qt.io/)
- Designer pour l'interface
- Très complet

### Dear ImGui
- [GitHub ImGui](https://github.com/ocornut/imgui)
- UI immédiate
- Plus léger

## 🎯 Prochaines étapes

1. **Choisir le framework** (SFML recommandé)
2. **Installer les dépendances**
3. **Créer main_gui.cpp** avec la fenêtre
4. **Implémenter le rendu** des agents
5. **Ajouter l'interactivité** (clics, sélection)
6. **Intégrer avec l'agent_manager**
7. **Ajouter les logs en temps réel**
8. **Polisher l'interface** (couleurs, animations)

## ✨ Fonctionnalités à implémenter

- [ ] Fenêtre graphique avec SFML/Qt
- [ ] Affichage des panneaux d'agents
- [ ] Sélection d'agents (click)
- [ ] Boutons de contrôle (play, pause, stop)
- [ ] Affichage du status en temps réel
- [ ] Logs avec scroll
- [ ] Thème clair/sombre
- [ ] Notifications
- [ ] Graphiques d'état
- [ ] Historique d'exécution

## 🐛 Débogage

```bash
# Compiler avec symboles de debug
g++ -std=c++17 -g -O0 -I./include src/*.cpp -o debug_gui

# Lancer avec gdb
gdb ./debug_gui
```

## 📞 Configuration recommandée

- **Langage**: C++17
- **Framework**: SFML 2.5.1
- **Compilateur**: g++ 7+
- **Écran minimum**: 1200x800

---

**État**: 🚧 En construction  
**Dernière mise à jour**: 17 Avril 2026
