# 🚀 GUIDE DE DÉMARRAGE RAPIDE

## ⚡ En 5 Minutes

### 1. Installation (2 min)
```bash
cd /workspaces/application-linux/hand_detection_system
bash install.sh
source venv/bin/activate
```

### 2. Vérification (1 min)
```bash
python3 test_quick.py
```

### 3. Lancement (2 min)
```bash
python3 main.py
```

---

## 🎮 Première Utilisation

### Ce que vous verrez:
1. **Fenêtre principale**: Flux vidéo de la caméra avec la détection des mains
2. **Points verts**: Les landmarks (points clés) de la main
3. **Cercle jaune**: Le centre de la main (point d'interaction)

### Ce que vous pouvez faire:
- **Pointer sur les boutons**: Avec votre index
- **Lever la paume**: "Oui" ou accepter
- **Fermer le poing**: "Non" ou annuler
- **Appuyer sur 'q'**: Quitter

---

## 📊 Vérification Rapide

```bash
# Vérifier les dépendances
python3 -c "import cv2; import mediapipe; print('✓ OK')"

# Tester la détection
python3 test_quick.py

# Voir les exemples
python3 examples.py
```

---

## 🐛 Troubleshooting

### Caméra non trouvée
```bash
# Vérifier les caméras
ls /dev/video*

# Utiliser une autre caméra
python3 main.py --camera 1
```

### Détection faible
- Améliorez l'éclairage
- Rapprochez la main
- Réessayez

### Module non trouvé
```bash
source venv/bin/activate
pip install -r requirements.txt
```

---

## 📚 Prochaines Étapes

1. **Lire le README complet**: `README.md`
2. **Explorer les exemples**: `python3 examples.py`
3. **Personnaliser l'interface**: Modifier `interactive_interface.py`
4. **Ajouter de nouveaux gestes**: Modifier `gesture_recognizer.py`

---

**Bon amusement!** 🎉
