# 🎨 Démarrage de l'Interface Graphique

## ✅ Structure créée

```
gui/
├── include/              # Classes et interfaces
│   ├── gui_window.h     # Fenêtre principale
│   ├── gui_agent_panel.h # Panneaux d'agents
│   └── gui_button.h     # Boutons
│
├── src/                 # Implémentation
│   ├── simple_gui.cpp
│   ├── gui_agent_panel.cpp ✅ Compilé
│   ├── gui_button.cpp ✅ Compilé
│   └── main_gui_example.cpp
│
├── assets/              # Images, icônes (vide)
├── build/               # Fichiers compilés
├── libgui.a             # Bibliothèque compilée ✅
│
├── Makefile             # Build system
├── config.ini           # Configuration
├── setup.sh             # Script d'installation
├── README.md            # Documentation complète
└── DÉMARRAGE.md         # Ce fichier
```

## 🚀 3 étapes pour commencer

### Étape 1️⃣ : Choisir le framework graphique

#### Option A : **SFML** ⭐ RECOMMANDÉ (Simple)
```bash
sudo apt-get install libsfml-dev
```
- ✅ Facile à utiliser
- ✅ Léger et rapide
- ✅ Bon pour du 2D

#### Option B : **Qt5** (Complet)
```bash
sudo apt-get install qt5-default
```
- ✅ Production-ready
- ✅ Cross-platform
- ✅ Beaucoup de features

#### Option C : **GTK3** (Standard Linux)
```bash
sudo apt-get install libgtk-3-dev
```
- ✅ Bien intégré Linux
- ✅ Classique
- ✅ Stable

### Étape 2️⃣ : Exécuter le setup

```bash
cd gui/
./setup.sh
```

Cela va :
- ✅ Vérifier les dépendances
- ✅ Créer la structure
- ✅ Compiler la bibliothèque

### Étape 3️⃣ : Tester l'exemple

```bash
cd gui/src/
g++ -std=c++17 -I../include -L.. -lsfml-graphics -lsfml-window -lsfml-system \
  main_gui_example.cpp gui_agent_panel.cpp gui_button.cpp -o example
./example
```

## 📋 À faire maintenant

### Créer la fenêtre principale (Option 1 : SFML)

Créer `gui/src/gui_window_sfml.cpp`:

```cpp
#include <SFML/Graphics.hpp>
#include "../include/gui_window.h"
#include "../include/gui_agent_panel.h"

void GUIWindow::create() {
    // Créer la fenêtre SFML
    sf::RenderWindow window(
        sf::VideoMode(window_width, window_height), 
        window_title
    );
    window.setFramerateLimit(60);
    
    is_running = true;
}

void GUIWindow::run() {
    // Boucle d'affichage
    while (is_running) {
        // Traiter les événements
        // Mettre à jour
        // Afficher
    }
}
```

### Créer la fenêtre principale (Option 2 : Qt5)

Créer `gui/src/gui_window_qt.cpp`:

```cpp
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include "../include/gui_window.h"

void GUIWindow::create() {
    // Créer la fenêtre Qt
}

void GUIWindow::run() {
    // Lancer la boucle d'événements Qt
}
```

## 🎯 Architecture de l'interface

```
┌─────────────────────────────────────────────┐
│ GUIWindow (Fenêtre principale)              │
├─────────────────────────────────────────────┤
│                                             │
│  Contient:                                  │
│  - AgentPanelManager (gestion des agents)   │
│  - Boutons (contrôles)                      │
│  - Zone de logs                             │
│  - Barre de menu                            │
│                                             │
└─────────────────────────────────────────────┘
```

## 📝 Fichiers d'exemple

### Utiliser AgentPanelManager

```cpp
#include "gui/include/gui_agent_panel.h"

int main() {
    AgentPanelManager manager;
    
    // Sélectionner un agent
    manager.selectPanel(0);
    
    // Mettre à jour le status
    manager.updatePanelStatus(0, "Running");
    
    // Déselectionner tous
    manager.unselectAllPanels();
    
    return 0;
}
```

### Utiliser les boutons

```cpp
#include "gui/include/gui_button.h"

int main() {
    Button btn("Lancer", 100, 100, 120, 40);
    
    btn.setCallback([]() {
        std::cout << "Agent lancé!\n";
    });
    
    // Lors du click souris
    btn.update(mouse_x, mouse_y, mouse_clicked);
    
    return 0;
}
```

## 🔗 Lier à l'application principale

### Dans `/workspaces/application-linux/Makefile_cpp`:

```makefile
# Ajouter la GUI
CXXFLAGS += -Igui/include
LDFLAGS += -Lgui -lgui -lsfml-graphics -lsfml-window -lsfml-system

# Lier les objets GUI
SOURCES += gui/src/gui_agent_panel.cpp gui/src/gui_button.cpp
```

## 📚 Ressources

| Framework | Documentation | Installation |
|-----------|--------------|--------------|
| SFML | https://sfml-dev.org/docs | `apt install libsfml-dev` |
| Qt | https://doc.qt.io | `apt install qt5-default` |
| GTK | https://www.gtk.org/docs | `apt install libgtk-3-dev` |
| ImGui | https://github.com/ocornut/imgui | Build manual |

## ✨ Prochaines étapes

1. **Choisir SFML, Qt ou GTK** → installer
2. **Créer gui_window_[sfml/qt].cpp**
3. **Implémenter create() et run()**
4. **Ajouter le rendu des panneaux**
5. **Ajouter l'interactivité (click)**
6. **Connecter aux agents**
7. **Ajouter les logs en temps réel**
8. **Polisher l'UI (couleurs, font)**

## 🐛 Débogage

```bash
# Compiler avec symboles de debug
g++ -std=c++17 -g -O0 -I./include -c src/*.cpp

# Lancer avec gdb
gdb ./gui_app

# Ou avec valgrind pour les fuites mémoire
valgrind --leak-check=full ./gui_app
```

## 📞 Commandes utiles

```bash
# Compiler la GUI
cd gui && make all

# Nettoyer
cd gui && make clean

# Voir l'aide du Makefile
cd gui && make help

# Exécuter le setup
./gui/setup.sh

# Compiler un exemple
g++ -std=c++17 -I./include src/main_gui_example.cpp -o example
```

## 🎨 Configuration

Éditer `gui/config.ini` pour personnaliser :
- Couleurs (thème)
- Dimensions de fenêtre
- Layout des panneaux
- Rafraîchissement

## ✅ Checklist

- [ ] Dossier `gui/` créé
- [ ] Fichiers d'en-têtes créés
- [ ] Bibliothèque compilée (`libgui.a`)
- [ ] SFML/Qt installé
- [ ] Fenêtre principale implémentée
- [ ] Affichage des agents
- [ ] Boutons interactifs
- [ ] Logs en temps réel
- [ ] Thème personnel
- [ ] Lié à l'app principale

---

**État**: 🚧 Prêt à développer  
**Dernière mise à jour**: 17 Avril 2026  
**Prochaine étape**: Créer `gui_window_sfml.cpp` ou `gui_window_qt.cpp`
