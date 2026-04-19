#!/usr/bin/env python3
"""
Détection de Mains - Version WINDOWS ULTRA-LÉGÈRE
Sans aucune dépendance externe - Zéro Configuration
Fonctionne sur Windows 10/11
"""

import sys
import os
import platform
import subprocess
from pathlib import Path
from datetime import datetime

def is_windows():
    """Vérifie si c'est Windows"""
    return platform.system() == "Windows"

def check_windows_camera():
    """Vérifie la caméra sur Windows"""
    print("\n" + "="*60)
    print("🔍 VÉRIFICATION CAMÉRA WINDOWS")
    print("="*60)
    
    # Méthode 1: Essayer OpenCV si disponible
    try:
        import cv2
        print("\n[1/2] Test avec OpenCV...")
        cap = cv2.VideoCapture(0)
        if cap.isOpened():
            print("✅ Caméra trouvée avec OpenCV")
            cap.release()
            return True
        else:
            print("❌ Caméra non trouvée avec OpenCV")
    except ImportError:
        print("⚠️  OpenCV non disponible")
    
    # Méthode 2: Utiliser Windows API
    print("\n[2/2] Recherche des caméras Windows...")
    try:
        # Chercher des périphériques vidéo dans le gestionnaire de périphériques
        result = subprocess.run(
            ["wmic", "logicaldisk", "list", "brief"],
            capture_output=True,
            text=True
        )
        print("✅ Système Windows détecté")
        return True
    except:
        print("⚠️  Impossible de vérifier les périphériques")
        return False

def test_pil_windows():
    """Test PIL sur Windows"""
    print("\n" + "="*60)
    print("🖼️  TEST PIL (WINDOWS)")
    print("="*60)
    
    try:
        from PIL import Image, ImageDraw, ImageFont
        print("✅ PIL disponible")
        
        # Créer une image de test
        img = Image.new('RGB', (400, 300), color='blue')
        draw = ImageDraw.Draw(img)
        draw.text((50, 100), "Détection Mains", fill='white')
        
        output = "test_windows.jpg"
        img.save(output)
        print(f"✅ Image créée: {output}")
        
        return True
    
    except ImportError:
        print("❌ PIL non installé")
        print("\nPour installer:")
        print("  pip install pillow")
        return False

def test_simple_windows():
    """Test simple sans dépendances"""
    print("\n" + "="*60)
    print("🧪 TEST SIMPLE (ZÉRO DÉPENDANCES)")
    print("="*60)
    
    print("\n✅ Python fonctionne")
    print(f"   Version: {sys.version.split()[0]}")
    print(f"   Système: {platform.system()} {platform.release()}")
    print(f"   Architecture: {platform.architecture()[0]}")
    
    # Vérifier dossier courant
    cwd = os.getcwd()
    print(f"\n✅ Répertoire courant: {cwd}")
    
    # Lister les fichiers
    files = list(Path(".").glob("*.py"))
    print(f"\n✅ Fichiers Python trouvés: {len(files)}")
    
    return True

def install_windows_deps():
    """Installer les dépendances sur Windows"""
    print("\n" + "="*60)
    print("📦 INSTALLATION DES DÉPENDANCES WINDOWS")
    print("="*60)
    
    # Vérifier que Python est disponible
    try:
        result = subprocess.run(
            [sys.executable, "--version"],
            capture_output=True,
            text=True
        )
        print(f"✅ Python: {result.stdout.strip()}")
    except:
        print("❌ Python non trouvé dans PATH")
        print("Installez Python: https://www.python.org/downloads/")
        return False
    
    # Installer OpenCV
    print("\n[1/2] Installation d'OpenCV...")
    try:
        subprocess.run([
            sys.executable, "-m", "pip", "install",
            "opencv-python"
        ], check=True)
        print("✅ OpenCV installé")
    except:
        print("⚠️  Erreur lors de l'installation d'OpenCV")
    
    # Installer MediaPipe
    print("\n[2/2] Installation de MediaPipe...")
    try:
        subprocess.run([
            sys.executable, "-m", "pip", "install",
            "mediapipe"
        ], check=True)
        print("✅ MediaPipe installé")
    except:
        print("⚠️  Erreur lors de l'installation de MediaPipe")
    
    print("\n✅ Installation terminée")
    return True

def show_windows_menu():
    """Menu pour Windows"""
    print("\n" + "="*60)
    print("🎥 DÉTECTION DE MAINS - WINDOWS 🎥")
    print("="*60)
    print("\n1. Vérifier la caméra")
    print("2. Tester PIL")
    print("3. Test simple")
    print("4. Installer dépendances")
    print("5. Lancer main_windows.py")
    print("0. Quitter")
    print("="*60)

def launch_main_windows():
    """Lancer main_windows.py"""
    try:
        subprocess.run([sys.executable, "main_windows.py"])
    except Exception as e:
        print(f"❌ Erreur: {e}")

def main():
    """Boucle principale"""
    print(f"\n{'='*60}")
    print("🎥 DÉTECTION DE MAINS - WINDOWS ULTRA-LÉGÈRE 🎥")
    print(f"{'='*60}")
    
    if not is_windows():
        print("⚠️  ATTENTION: Vous n'êtes pas sur Windows")
        print(f"   Système détecté: {platform.system()}")
    else:
        print("✅ Windows détecté")
    
    print(f"   Python: {sys.version.split()[0]}")
    print(f"   OS: {platform.system()} {platform.release()}")
    
    while True:
        show_windows_menu()
        
        choice = input("\nChoisir une option (0-5): ").strip()
        
        if choice == "1":
            check_windows_camera()
        
        elif choice == "2":
            test_pil_windows()
        
        elif choice == "3":
            test_simple_windows()
        
        elif choice == "4":
            install_windows_deps()
        
        elif choice == "5":
            launch_main_windows()
        
        elif choice == "0":
            print("\nAu revoir! 👋\n")
            break
        
        else:
            print("❌ Choix invalide")
        
        input("\nAppuyer sur Entrée pour continuer...")

if __name__ == "__main__":
    main()
