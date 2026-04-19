# 🎥 GUIDE COMPLET WINDOWS

## 📋 Vue d'ensemble

Trois versions du système de détection de mains **totalement compatibles Windows 10/11**:

---

## 🚀 DÉMARRAGE RAPIDE (WINDOWS)

### Option 1: Ultra-Légère (< 1 minute)

```cmd
python windows_ultra_light.py
```

**Avantages:**
- ✅ Python standard uniquement
- ✅ Zéro dépendances initiales
- ✅ Menu interactif
- ✅ Fonctionne immédiatement

### Option 2: Avec OpenCV (2-3 minutes)

**Étape 1: Exécuter l'installateur**

Double-cliquez sur l'un de ces fichiers:

```
install_windows.bat           (Batch - CMD)
```

Ou en PowerShell:

```powershell
powershell -ExecutionPolicy Bypass -File install_windows.ps1
```

**Étape 2: Lancer l'application**

```cmd
python main_windows.py
```

**Avantages:**
- ✅ Installation automatisée
- ✅ Détection en temps réel
- ✅ Interface complète
- ✅ Performance optimale

---

## 💻 INSTALLATION DÉTAILLÉE (WINDOWS)

### Prérequis

1. **Python 3.8+**
   - Télécharger depuis: https://www.python.org/downloads/
   - **IMPORTANT**: Cocher "Add Python to PATH"
   - Tester: Ouvrir CMD et taper `python --version`

2. **Caméra**
   - Webcam USB ou intégrée
   - Vérifier dans Gestionnaire de périphériques

### Installation Méthode 1: BAT (Recommandée)

**Pour CMD/Invite de commandes:**

```batch
cd /d C:\chemin\vers\hand_detection_system
install_windows.bat
```

✅ Cela va:
1. Vérifier Python
2. Mettre à jour pip
3. Installer OpenCV
4. Installer MediaPipe

### Installation Méthode 2: PowerShell

**Pour PowerShell:**

```powershell
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope CurrentUser
cd C:\chemin\vers\hand_detection_system
.\install_windows.ps1
```

### Installation Méthode 3: Manuel

Si les scripts ne fonctionnent pas:

```cmd
# Mettre à jour pip
python -m pip install --upgrade pip

# Installer OpenCV
pip install opencv-python

# Installer MediaPipe
pip install mediapipe

# Installer NumPy
pip install numpy
```

---

## 🎮 UTILISATION (WINDOWS)

### Version Ultra-Légère

```cmd
python windows_ultra_light.py
```

**Menu options:**
- 1 - Vérifier la caméra
- 2 - Tester PIL (images)
- 3 - Test simple
- 4 - Installer dépendances
- 5 - Lancer main_windows.py
- 0 - Quitter

### Version Standard

```cmd
python main_windows.py
```

**Menu options:**
- 1 - Vérifier OpenCV
- 2 - Tester la caméra
- 3 - Capture d'une image
- 4 - Test de détection
- 5 - Démo interactive
- 6 - Installer dépendances
- 0 - Quitter

### Version Complète (avec ML)

```cmd
python main.py
```

---

## 🎯 FICHIERS WINDOWS

| Fichier | Fonction | Démarrage |
|---------|----------|-----------|
| **windows_ultra_light.py** | Menu interactif simple | `python windows_ultra_light.py` |
| **main_windows.py** | Application complète | `python main_windows.py` |
| **main.py** | Avec MediaPipe ML | `python main.py` |
| **install_windows.bat** | Installeur CMD | Double-clic |
| **install_windows.ps1** | Installeur PowerShell | PowerShell script |

---

## 🔧 RÉSOLUTION DES PROBLÈMES (WINDOWS)

### ❌ "Python not recognized"

**Cause:** Python n'est pas dans le PATH

**Solution:**
```cmd
# Réinstaller Python
# - Télécharger depuis python.org
# - Cocher "Add Python to PATH"
# - Relancer l'installation
```

Ou vérifier manuellement:
```cmd
C:\Users\YourName\AppData\Local\Programs\Python\Python312\python.exe --version
```

### ❌ "OpenCV import failed"

**Cause:** OpenCV non installé

**Solution:**
```cmd
pip install opencv-python
```

Ou utiliser le script d'installation:
```cmd
install_windows.bat
```

### ❌ "Camera not found"

**Cause:** La caméra n'est pas accessible

**Solutions:**
1. Vérifier dans Gestionnaire de périphériques
2. Vérifier les permissions d'accès (Paramètres → Confidentialité)
3. Redémarrer l'application
4. Essayer un autre port USB (si caméra USB)

### ❌ "ModuleNotFoundError: No module named 'cv2'"

**Cause:** OpenCV n'est pas installé

**Solution:**
```cmd
pip install opencv-python
```

### ❌ "Permission denied"

**Cause:** Problèmes de permissions

**Solutions:**
1. Exécuter en tant qu'administrateur
2. Vérifier les permissions de la caméra (Paramètres Windows)
3. Désactiver l'antivirus temporairement

---

## 📊 COMPARAISON DES VERSIONS (WINDOWS)

| Aspect | Ultra-Light | Standard | Complète |
|--------|-------------|----------|----------|
| **Fichier** | windows_ultra_light.py | main_windows.py | main.py |
| **Dépendances** | Python standard | OpenCV | OpenCV + MediaPipe |
| **Installation** | Aucune | install_windows.bat | Manuelle |
| **Temps démarrage** | <1s | 1-2s | 2-3s |
| **Précision** | Moyenne | Bonne | Très bonne |
| **FPS** | 1-2 | 5-10 | 25-30 |
| **Gestes** | Non | Basique | 8 types |
| **Landmarks** | Non | Non | 21/main |
| **RAM** | 50 MB | 100 MB | 200 MB |

---

## 🔍 TEST DE FONCTIONNEMENT

### Test Rapide (30 secondes)

```cmd
python windows_ultra_light.py
# Choisir option 3 (Test simple)
```

### Test Caméra (1 minute)

```cmd
python windows_ultra_light.py
# Choisir option 1 (Vérifier la caméra)
```

### Test Complet (2 minutes)

```cmd
install_windows.bat
python main_windows.py
# Choisir option 5 (Démo interactive)
```

---

## 📦 CRÉER UN EXÉCUTABLE WINDOWS

Si vous voulez distribuer l'application sans Python:

### Avec PyInstaller

```cmd
# Installer PyInstaller
pip install pyinstaller

# Créer l'exécutable
pyinstaller --onefile --windowed main_windows.py

# L'exécutable sera dans: dist/main_windows.exe
```

### Créer un Installer Windows

Utiliser Inno Setup:

1. Télécharger Inno Setup: http://www.jrsoftware.org/isdl.php
2. Créer un script .iss
3. Compiler avec Inno Setup

---

## 🌐 DISTRIBUTION (WINDOWS)

### Empaqueter pour Distribution

1. **Créer un dossier:**
   ```
   hand_detection_windows/
   ├── main_windows.py
   ├── windows_ultra_light.py
   ├── install_windows.bat
   ├── install_windows.ps1
   ├── README_WINDOWS.md
   └── requirements.txt
   ```

2. **Créer requirements.txt:**
   ```
   opencv-python==4.8.1.78
   mediapipe==0.10.5
   numpy==1.24.3
   ```

3. **Zipper le dossier:**
   ```
   hand_detection_windows.zip
   ```

4. **Distribuer:**
   - Les utilisateurs téléchargent le ZIP
   - Double-cliquent sur `install_windows.bat`
   - Lancent `python main_windows.py`

---

## 🎯 CHEMINS WINDOWS

| Type | Exemple |
|------|---------|
| **Home** | C:\Users\YourName |
| **Documents** | C:\Users\YourName\Documents |
| **Téléchargements** | C:\Users\YourName\Downloads |
| **Python** | C:\Users\YourName\AppData\Local\Programs\Python\Python312 |
| **PIP packages** | C:\Users\YourName\AppData\Local\Programs\Python\Python312\Lib\site-packages |

---

## 💾 ENREGISTREMENT DE FICHIERS

L'application sauvegarde les fichiers dans le **répertoire courant**:

```
C:\Users\YourName\Documents\hand_detection_windows\
├── capture.jpg           (Image capturée)
├── capture_windows.jpg   (Démo screenshot)
├── interactions.log      (Log des interactions)
└── screenshots/          (Dossier des captures)
```

---

## 📝 EXEMPLE DE SCRIPT BATCH PERSONNALISÉ

Créer un fichier `mon_app.bat`:

```batch
@echo off
cls
echo 🎥 Détection de Mains
python main_windows.py
pause
```

Puis double-cliquez pour lancer.

---

## ⚡ RACCOURCIS UTILES

| Touche | Action |
|--------|--------|
| **Q** ou **ESC** | Quitter |
| **SPACE** | Capture screenshot |
| **S** | Sauvegarder |
| **R** | Réinitialiser |
| **H** | Aide |

---

## 🎓 DÉPANNAGE AVANCÉ

### Logs détaillés

```cmd
python main_windows.py > log.txt 2>&1
```

### Avec débogage

```cmd
python -u main_windows.py
```

### Test de performance

```cmd
python -m cProfile main_windows.py
```

---

## 🌟 CONSEILS WINDOWS

1. **Toujours utiliser le chemin complet** si problèmes de PATH
2. **Exécuter en tant qu'administrateur** si permissions
3. **Désactiver antivirus** si problèmes de performance
4. **Utiliser CMD plutôt que PowerShell** si problèmes
5. **Redémarrer après installation** de Python

---

## 📞 SUPPORT

| Problème | Solution |
|----------|----------|
| Python ne fonctionne pas | Réinstaller Python avec "Add to PATH" |
| Caméra non trouvée | Vérifier dans Paramètres → Confidentialité |
| OpenCV erreur | Faire `pip install opencv-python` |
| Application lente | Fermer autres applications |
| Erreur d'accès | Exécuter en tant qu'administrateur |

---

## ✅ CHECKLIST WINDOWS

- [ ] Python 3.8+ installé et dans PATH
- [ ] Python vérifié: `python --version`
- [ ] Caméra testée dans Paramètres
- [ ] Script d'installation exécuté
- [ ] Application lancée: `python main_windows.py`
- [ ] Caméra s'affiche correctement
- [ ] Les gestes sont détectés
- [ ] Les captures sauvegardent

---

## 🎉 CONCLUSION

L'application est **100% compatible Windows**!

Choix recommandé:
- **Débutant:** `windows_ultra_light.py`
- **Utilisateur:** `main_windows.py` (après install_windows.bat)
- **Développeur:** `main.py` (installation manuelle)

**Bon amusement avec Windows! 🎥**
