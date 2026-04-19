# 🚀 GUIDE DE DÉMARRAGE RAPIDE - VERSION SIMPLIFIÉE

## ⚡ En 30 secondes

### Option 1: Interface Graphique (Recommandé)
```bash
cd /workspaces/application-linux
bash start_gui.sh
```

### Option 2: Détection de Mains
```bash
cd /workspaces/application-linux
bash start_hand_detection.sh
```

### Option 3: Menu interactif complet
```bash
cd /workspaces/application-linux
bash launcher.sh
```

---

## 📋 Qu'est-ce que vous pouvez faire ?

### 🖥️ Interface Graphique
- Gérer les agents (Red Team, Blue Team)
- Voir l'état de chaque agent en temps réel
- Contrôler les opérations
- **Lancer avec:** `bash start_gui.sh`

### 🖐️ Détection de Mains
- Détecteur de mains en temps réel (caméra requise)
- Reconnaissance de gestes automatique
- Performances optimisées
- **Lancer avec:** `bash start_hand_detection.sh`

### 🔗 Système Combiné
- Détection de mains + interface GUI
- Interaction manuelle avec l'interface
- **Lancer via:** `bash launcher.sh` → Option 3

---

## 🔧 Prérequis

- **C++**: `g++` et `gcc` (pour compiler l'interface)
- **Python**: Python 3.7+ (pour la détection)
- **Dépendances Python**: Installées automatiquement
- **Caméra**: Requise pour la détection de mains

### Vérifier votre système

```bash
# Vérifier C++
g++ --version

# Vérifier Python
python3 --version

# Vérifier la caméra
ls -l /dev/video0
```

---

## 📱 Menu Interactif Principal

```
1) Interface Graphique GUI (C++)
2) Détection de Mains (Python)
3) Système Combiné (Détection + Interface)
4) Lancer tous les tests
5) Configuration et réparation
6) Afficher l'aide
0) Quitter
```

Lancez le menu avec:
```bash
bash launcher.sh
```

---

## ✨ Améliorations Récentes

### Détection de Mains 🖐️
- ✅ Performances optimisées (threading)
- ✅ Lissage temporel des landmarks
- ✅ Filtrage des détections aberrantes
- ✅ Reconnaissance avancée des gestes
- ✅ Historique et stabilité des gestes
- ✅ Calibration automatique

### Interface Graphique 🖥️
- ✅ Lancement simplifié avec `start_gui.sh`
- ✅ Compilation automatique si nécessaire
- ✅ Gestion d'erreurs améliorée
- ✅ Menu interactif complet

---

## 🐛 Dépannage

### Problème: "Commande non trouvée" 
```bash
# Vérifiez que vous êtes au bon endroit
cd /workspaces/application-linux
ls -la start_gui.sh launcher.sh
```

### Problème: Erreur de compilation GUI
```bash
# Réinstallez les outils de compilation
sudo apt-get update
sudo apt-get install build-essential

# Puis réessayez
bash start_gui.sh
```

### Problème: Caméra non trouvée
```bash
# Vérifiez la caméra
ls -l /dev/video0
v4l2-ctl --list-devices

# Démarrez sans caméra:
bash launcher.sh
# Choisir option 1 (Interface seule)
```

### Problème: Dépendances Python manquantes
```bash
# Les dépendances s'installent automatiquement
# Mais vous pouvez les installer manuellement:
pip3 install opencv-python mediapipe -q
```

---

## 🎯 Cas d'Usage Courants

### "Je veux juste lancer l'interface"
```bash
bash start_gui.sh
```

### "Je veux tester la détection de mains"
```bash
bash start_hand_detection.sh
```

### "Je veux explorer toutes les options"
```bash
bash launcher.sh
```

### "Je veux vérifier que tout fonctionne"
```bash
bash launcher.sh
# Puis sélectionner option 4 (Tests)
```

### "Je pense avoir un problème"
```bash
bash launcher.sh
# Puis sélectionner option 5 (Configuration)
```

---

## 📊 Architecture du Système

```
/workspaces/application-linux/
├── launcher.sh                      # Menu principal interactif
├── start_gui.sh                     # Lancement rapide GUI
├── start_hand_detection.sh          # Lancement rapide détection
│
├── gui/                             # Interface Graphique
│   ├── src/unified_gui_system.cpp  # Code source principal
│   ├── build/                      # Fichiers compilés
│   └── Makefile                    # Configuration de compilation
│
├── hand_detection_system/           # Détection de Mains
│   ├── hand_detector.py            # Version standard
│   ├── hand_detector_optimized.py  # Version optimisée ⭐
│   ├── gesture_recognizer.py       # Version standard
│   ├── gesture_recognizer_advanced.py  # Version avancée ⭐
│   ├── main.py                     # Script principal
│   ├── test_optimized.py           # Test optimisé
│   └── requirements.txt            # Dépendances
│
└── agent/                           # Systèmes d'agents
    ├── agent_orchestrator/          # Orchestration
    ├── agent_red_team/              # Agents rouges
    └── agent_blue_team/             # Agents bleus
```

---

## 🔗 Fichiers Importants

| Fichier | Description |
|---------|-------------|
| `launcher.sh` | Menu interactif principal |
| `start_gui.sh` | Démarrage rapide interface |
| `start_hand_detection.sh` | Démarrage rapide détection |
| `gui/src/unified_gui_system.cpp` | Interface graphique |
| `hand_detection_system/hand_detector_optimized.py` | Détection optimisée |
| `hand_detection_system/gesture_recognizer_advanced.py` | Gestes avancés |

---

## 📞 Support

Si vous rencontrez un problème:

1. Lancez le menu: `bash launcher.sh`
2. Sélectionnez option 4: "Lancer tous les tests"
3. Les tests vous montreront ce qui fonctionne/ne fonctionne pas

---

## 🎓 Prochaines Étapes

- [ ] Tester l'interface avec `bash start_gui.sh`
- [ ] Tester la détection avec `bash start_hand_detection.sh`
- [ ] Essayer le système combiné via `launcher.sh`
- [ ] Parcourir les autres guides dans le projet

**Bon amusement! 🚀**
