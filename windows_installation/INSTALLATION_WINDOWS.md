# 🪟 GUIDE D'INSTALLATION WINDOWS - COMPLET

**Système d'exploitation:** Windows 10 / Windows 11  
**Date:** Avril 2026

---

## ⚡ Installation Ultra-Rapide (5 minutes)

### 1️⃣ Python
```
Téléchargez: https://www.python.org/downloads/
Installez et COCHEZ "Add Python to PATH"
```

### 2️⃣ Dépendances
```bash
pip install opencv-python mediapipe
```

### 3️⃣ Lancer
```bash
# GUI
double-click start_gui.bat

# Détection
double-click start_hand_detection.bat

# Menu complet
double-click launcher.bat
```

---

## 📋 Installation Détaillée Étape par Étape

### **Étape 1: Installer Python**

#### Option A: Installer.exe (Recommandé pour débutants)

1. **Télécharger**
   - Allez sur https://www.python.org/downloads/
   - Cliquez sur "Download Python 3.11" (ou plus récent)
   - Téléchargez l'installateur Windows 64-bit

2. **Installer**
   - Double-cliquez sur `python-3.11.x-amd64.exe`
   - **IMPORTANT:** Cochez "Add Python to PATH" en bas
   - Cliquez sur "Install Now"
   - Attendez la fin

3. **Vérifier**
   - Ouvrez CMD (touche Windows + R, tapez `cmd`)
   - Tapez: `python --version`
   - Vous devez voir: `Python 3.11.x`

#### Option B: Windows Store (Alternative)

1. Ouvrez Microsoft Store
2. Cherchez "Python"
3. Installez "Python 3.11" (par Python Software Foundation)
4. C'est automatique!

---

### **Étape 2: Installer les Dépendances Python**

1. **Ouvrez CMD**
   - Touche Windows + R
   - Tapez: `cmd`
   - Appuyez sur Entrée

2. **Installez les packages**
   ```bash
   pip install opencv-python mediapipe
   ```

3. **Vérifier**
   ```bash
   pip list
   ```
   Vous devez voir `opencv-python` et `mediapipe`

---

### **Étape 3: Préparer les Fichiers**

1. **Téléchargez le repository**
   ```bash
   git clone https://github.com/darklionu/application-linux.git
   cd application-linux
   ```

2. **Ou copiez les fichiers**
   - Copiez le dossier entier sur votre PC
   - Ouvrez CMD dans ce dossier (Maj+Clic droit → "Ouvrir PowerShell ici")

---

### **Étape 4: Lancer l'Application**

#### Option A: Double-cliquez les .bat (Le Plus Simple)

```
launcher.bat              ← Menu principal
start_gui.bat             ← Interface graphique
start_hand_detection.bat  ← Détection de mains
```

#### Option B: Depuis CMD

```bash
# Interface
start_gui.bat

# Détection
start_hand_detection.bat

# Menu
launcher.bat
```

#### Option C: Python Direct

```bash
cd hand_detection_system
python main.py
```

---

## 🎯 Structure des Fichiers Windows

```
application-linux/
├── launcher.bat                    ← Menu principal ⭐
├── start_gui.bat                   ← Lancer interface
├── start_hand_detection.bat        ← Lancer détection
├── INSTALLATION_WINDOWS.md         ← Ce fichier
│
├── gui/
│   ├── agent_manager_unified.exe   ← Interface (si compilée)
│   ├── jarvis_interactive          ← Alternative
│   └── src/                        ← Code source
│
└── hand_detection_system/
    ├── main.py                     ← Détection standard
    ├── main_ultra_light.py         ← Version légère
    ├── hand_detector.py
    ├── hand_detector_optimized.py  ← ⭐ Nouveau
    ├── gesture_recognizer.py
    └── gesture_recognizer_advanced.py ← ⭐ Nouveau
```

---

## ✨ 3 Façons de Lancer

### **1. Menu Graphique (Recommandé pour débutants)**
```
Double-cliquez: launcher.bat
Choisissez votre option
```

### **2. Démarrage Rapide (Pour experts)**
```
Double-cliquez: start_gui.bat
OU
Double-cliquez: start_hand_detection.bat
```

### **3. Ligne de Commande**
```bash
python main.py
python -m hand_detector_optimized
```

---

## 🔧 Dépannage Windows

### **Problème: "python: commande non reconnue"**

**Cause:** Python n'est pas dans PATH

**Solution:**
```
1. Désinstallez Python
2. Réinstallez en cochant "Add Python to PATH"
3. Redémarrez Windows
4. Rouvrez CMD
```

---

### **Problème: "pip: commande non reconnue"**

**Cause:** pip n'est pas installé

**Solution:**
```bash
python -m pip install --upgrade pip
python -m pip install opencv-python mediapipe
```

---

### **Problème: "No module named cv2"**

**Cause:** opencv-python n'est pas installé

**Solution:**
```bash
pip install --upgrade opencv-python
```

---

### **Problème: Caméra non trouvée**

**Cause:** 
- Caméra non connectée
- Permissions manquantes
- Driver non mis à jour

**Solution:**
```
1. Vérifiez que la caméra est connectée
2. Allez à Paramètres → Confidentialité → Caméra
3. Assurez-vous que "Accès à la caméra" est activé
4. Mettez à jour les drivers de la caméra
```

---

### **Problème: "Access denied" ou "Permission denied"**

**Cause:** Fichiers en lecture seule

**Solution:**
```
1. Clic droit sur le dossier → Propriétés
2. Décochez "Lecture seule"
3. Appliquez à tous les fichiers
```

---

## 📊 Cas d'Usage Courants

### "Je veux juste lancer l'interface"
```
Double-cliquez: start_gui.bat
```

### "Je veux tester la détection de mains"
```
Double-cliquez: start_hand_detection.bat
```

### "Je veux choisir entre plusieurs options"
```
Double-cliquez: launcher.bat
```

### "Je veux une version légère (peu de RAM)"
```bash
python main_ultra_light.py
```

### "Je veux juste le code Python"
```bash
cd hand_detection_system
python -m pip install opencv-python mediapipe
python main.py
```

---

## 🚀 Commandes Utiles

```bash
# Voir la version Python
python --version

# Voir les packages installés
pip list

# Mettre à jour un package
pip install --upgrade opencv-python

# Désinstaller un package
pip uninstall opencv-python

# Installer depuis requirements.txt
pip install -r requirements.txt

# Créer un environnement virtuel
python -m venv env
env\Scripts\activate
```

---

## 🎨 Compilation de l'Interface GUI (Optionnel)

Si vous voulez compiler l'interface GUI C++ vous-même :

### **Option A: Avec Visual Studio Community (Recommandé)**

1. **Téléchargez Visual Studio Community** 
   - https://visualstudio.microsoft.com/downloads/
   - Installez "Desktop development with C++"

2. **Compilez**
   ```bash
   cd gui
   cl /O2 /EHsc /I include src\unified_gui_system.cpp /Fe:build\agent_manager_gui_unified.exe
   ```

### **Option B: Avec MinGW**

1. **Installez MinGW**
   - https://mingw-w64.org/
   - Ajoutez à PATH

2. **Compilez**
   ```bash
   cd gui
   g++ -std=c++11 -O2 -o build/agent_manager_gui_unified.exe src/unified_gui_system.cpp
   ```

---

## 💾 Télécharger Les Fichiers

### **Option 1: Git**
```bash
git clone https://github.com/darklionu/application-linux.git
cd application-linux
launcher.bat
```

### **Option 2: ZIP**
- Cliquez sur "Code" → "Download ZIP"
- Extrayez le fichier
- Ouvrez le dossier
- Double-cliquez `launcher.bat`

### **Option 3: Synchroniser**
```bash
# Dans le dossier existant
git pull
```

---

## 🌐 Ressources Utiles

| Ressource | Lien |
|-----------|------|
| **Python** | https://www.python.org/downloads/ |
| **Git** | https://git-scm.com/download/win |
| **Visual Studio** | https://visualstudio.microsoft.com/ |
| **MinGW** | https://mingw-w64.org/ |
| **OpenCV Docs** | https://docs.opencv.org/ |
| **MediaPipe** | https://mediapipe.dev/ |

---

## 📞 Support

Si vous avez un problème:

1. Vérifiez cette page de dépannage
2. Consultez les logs
3. Cherchez sur Google: votre erreur exacte
4. Ouvrez une issue sur GitHub

---

## ✅ Checklist d'Installation

- [ ] Python installé (`python --version` fonctionne)
- [ ] "Add Python to PATH" coché lors de l'installation
- [ ] opencv-python installé (`pip list` montre opencv-python)
- [ ] mediapipe installé (`pip list` montre mediapipe)
- [ ] Fichiers .bat présents dans le dossier
- [ ] Caméra connectée et fonctionnelle
- [ ] Permissions de fichiers OK

---

## 🎓 Prochaines Étapes

1. ✅ Lancez `launcher.bat`
2. ✅ Essayez l'interface graphique
3. ✅ Testez la détection de mains
4. ✅ Explorez les options du menu
5. ✅ Consultez les autres guides

---

**Prêt? Double-cliquez sur `launcher.bat` et amusez-vous! 🚀**
