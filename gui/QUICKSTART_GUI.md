# 🚀 Quick Start - Interface Graphique

## Commande unique pour démarrer (30 secondes)

```bash
cd /workspaces/application-linux/gui

# Installation du framework SFML (recommandé)
sudo apt-get install libsfml-dev

# Compilation de la bibliothèque
make all

# Exécuter l'exemple
cd src && g++ -std=c++17 -I../include main_gui_example.cpp \
  gui_agent_panel.cpp gui_button.cpp -o example && ./example
```

## Structure complète

```
gui/
├── 📖 DÉMARRAGE.md      ← Lisez ceci en premier!
├── 📖 README.md         ← Documentation complète
├── 📖 STRUCTURE.txt     ← Vue d'ensemble (ce que vous lisez)
├── 📖 QUICKSTART_GUI.md ← Guide rapide (ce fichier)
│
├── include/             ← Classes disponibles
│   ├── gui_window.h     (à implémenter)
│   ├── gui_agent_panel.h (prêt)
│   └── gui_button.h     (prêt)
│
├── src/                 ← Code source
│   ├── simple_gui.cpp
│   ├── gui_agent_panel.cpp
│   ├── gui_button.cpp
│   └── main_gui_example.cpp
│
├── Makefile             ← Compilation
├── config.ini           ← Paramètres
└── setup.sh             ← Installation
```

## 3 Options graphiques

### 1. SFML (Recommandé) ⭐
```bash
# Installation
sudo apt-get install libsfml-dev

# Compilation avec SFML
g++ -std=c++17 main.cpp -lsfml-graphics -lsfml-window -lsfml-system

# Avantages: Simple, rapide, 2D excellent
```

### 2. Qt5 (Complet)
```bash
# Installation
sudo apt-get install qt5-default

# Avantages: Production-ready, cross-platform
```

### 3. GTK3 (Standard Linux)
```bash
# Installation
sudo apt-get install libgtk-3-dev

# Avantages: Bien intégré Linux
```

## Code minimum pour tester

### Fichier: `gui/src/test_gui.cpp`

```cpp
#include <iostream>
#include "../include/gui_agent_panel.h"
#include "../include/gui_button.h"

int main() {
    // Créer le gestionnaire d'agents
    AgentPanelManager manager;
    
    // Afficher les agents
    std::cout << "Agents disponibles:\n";
    for (size_t i = 0; i < manager.getPanelCount(); ++i) {
        auto& panel = manager.getPanels()[i];
        std::cout << i+1 << ". " << panel.name << "\n";
    }
    
    // Créer un bouton
    Button btn("Lancer", 100, 100, 120, 40);
    btn.setCallback([]() {
        std::cout << "Bouton cliqué!\n";
    });
    
    // Sélectionner un agent
    manager.selectPanel(0);
    manager.updatePanelStatus(0, "Running");
    
    std::cout << "\n✅ GUI initialisée!\n";
    return 0;
}
```

Compiler et exécuter:
```bash
cd gui/src
g++ -std=c++17 -I../include test_gui.cpp gui_agent_panel.cpp gui_button.cpp -o test
./test
```

## Intégration avec l'app principale

### Dans `/workspaces/application-linux/Makefile_cpp`:

```makefile
# Ajouter les chemins
CXXFLAGS += -Igui/include
LDFLAGS += -Lgui -lgui

# Compiler la GUI d'abord
gui/libgui.a:
$(MAKE) -C gui all

# Dépendance
agent_manager_app: gui/libgui.a $(OBJECTS)
$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
```

Puis compiler:
```bash
make -f Makefile_cpp all
```

## Fichiers clés à connaître

| Fichier | Contenu | Statut |
|---------|---------|--------|
| `DÉMARRAGE.md` | 3 étapes pour commencer | 📖 Lisez ceci |
| `README.md` | Architecture détaillée | 📖 Références |
| `config.ini` | Paramètres de l'UI | ⚙️ Personnalisez |
| `gui_agent_panel.h` | Panneaux d'agents | ✅ Prêt |
| `gui_button.h` | Boutons | ✅ Prêt |
| `gui_window.h` | Fenêtre principale | 🚧 À faire |

## Commandes essentielles

```bash
# Aller dans le dossier GUI
cd gui/

# Compiler
make all

# Nettoyer
make clean

# Voir l'aide
make help

# Exécuter le setup
./setup.sh

# Compiler un exemple
cd src && g++ -std=c++17 -I../include main_gui_example.cpp \
  gui_agent_panel.cpp gui_button.cpp -o example && ./example
```

## Prochaines étapes

### Phase 1: Base (cette semaine)
- [ ] Choisir SFML/Qt/GTK
- [ ] Installer le framework
- [ ] Compiler la bibliothèque
- [ ] Exécuter l'exemple

### Phase 2: Fenêtre (semaine prochaine)
- [ ] Créer `gui_window_sfml.cpp`
- [ ] Implémenter la fenêtre
- [ ] Ajouter le rendu des agents
- [ ] Tester l'affichage

### Phase 3: Interactivité (semaine d'après)
- [ ] Ajouter les clics souris
- [ ] Gérer la sélection d'agents
- [ ] Ajouter les boutons interactifs
- [ ] Afficher les logs

### Phase 4: Polish (finition)
- [ ] Thème personnalisé
- [ ] Animations
- [ ] Notifications
- [ ] Graphiques d'état

## ✨ Points clés

✅ **Structure prêt** - Dossiers et fichiers créés  
✅ **Bibliothèque compilée** - `libgui.a` utilisable  
✅ **Classes d'exemple** - `AgentPanelManager` et `Button`  
⚠️ **GUI window à implémenter** - Nécessite choix du framework  
⚠️ **Rendu graphique à ajouter** - Créer `gui_window_sfml.cpp`  

## 🎯 Recommandation

**Commencez avec SFML** - c'est le plus simple pour débuter une GUI 2D!

```bash
# Installation (5 sec)
sudo apt-get install libsfml-dev

# Test (30 sec)
cd gui && make all

# Développez la fenêtre SFML (1-2 heures)
```

---

**État**: 🚧 Prêt à développer  
**Durée estimée**: 4-6 heures pour une GUI fonctionnelle basique  
**Complexité**: Moyenne (intermédiaire en C++)

**Suivant**: Lisez `DÉMARRAGE.md` pour le guide détaillé!
