#!/usr/bin/env python3
"""
Système de Détection de Mains - Version WINDOWS
Compatible Windows 10/11 - Zéro Configuration
"""

import sys
import os
import platform
import subprocess
from pathlib import Path

def check_windows():
    """Vérifie que c'est Windows"""
    if platform.system() != "Windows":
        print("⚠️  Cette version est optimisée pour Windows")
        print(f"   Système détecté: {platform.system()}")
    return platform.system() == "Windows"

def check_camera():
    """Vérifie la caméra sur Windows"""
    try:
        import cv2
        cap = cv2.VideoCapture(0)
        if cap.isOpened():
            print("✅ Caméra trouvée")
            cap.release()
            return True
        else:
            print("❌ Impossible d'accéder à la caméra")
            return False
    except ImportError:
        print("⚠️  OpenCV non disponible")
        return False
    except Exception as e:
        print(f"❌ Erreur caméra: {e}")
        return False

def test_opencv():
    """Test OpenCV sur Windows"""
    print("\n" + "="*60)
    print("🔍 TEST OPENCV (WINDOWS)")
    print("="*60)
    
    try:
        import cv2
        print(f"✅ OpenCV installé: {cv2.__version__}")
        
        # Test caméra
        cap = cv2.VideoCapture(0)
        if cap.isOpened():
            print("✅ Caméra accessible")
            ret, frame = cap.read()
            if ret:
                print(f"✅ Capture réussie: {frame.shape}")
            cap.release()
            return True
        else:
            print("❌ Caméra non accessible")
            return False
    
    except ImportError:
        print("❌ OpenCV non installé")
        print("\nPour installer:")
        print("  pip install opencv-python")
        return False

def test_basic_capture():
    """Capture basique avec OpenCV"""
    print("\n" + "="*60)
    print("📸 CAPTURE BASIQUE (WINDOWS)")
    print("="*60)
    
    try:
        import cv2
        
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("❌ Caméra non trouvée")
            return False
        
        print("🔄 Capture en cours...")
        ret, frame = cap.read()
        
        if ret:
            # Sauvegarder
            output_path = "capture.jpg"
            cv2.imwrite(output_path, frame)
            print(f"✅ Image capturée: {output_path}")
            print(f"   Résolution: {frame.shape[1]}x{frame.shape[0]}")
            
            # Montrer l'image
            cv2.imshow("Capture", frame)
            print("Appuyez sur une touche pour continuer...")
            cv2.waitKey(3000)
            cv2.destroyAllWindows()
            
            cap.release()
            return True
        else:
            print("❌ Impossible de capturer")
            cap.release()
            return False
    
    except Exception as e:
        print(f"❌ Erreur: {e}")
        return False

def test_detection():
    """Détection simple"""
    print("\n" + "="*60)
    print("🖐️ DÉTECTION SIMPLE (WINDOWS)")
    print("="*60)
    
    try:
        import cv2
        import numpy as np
        
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("❌ Caméra non trouvée")
            return False
        
        print("🔄 Test de détection (5 frames)...")
        
        for i in range(5):
            ret, frame = cap.read()
            if ret:
                # Convertir en HSV pour détection peau
                hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
                
                # Limites pour couleur peau
                lower_skin = np.array([0, 20, 70], dtype=np.uint8)
                upper_skin = np.array([20, 255, 255], dtype=np.uint8)
                
                # Mask
                mask = cv2.inRange(hsv, lower_skin, upper_skin)
                
                # Contours
                contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
                
                if contours:
                    largest = max(contours, key=cv2.contourArea)
                    area = cv2.contourArea(largest)
                    
                    if area > 500:
                        print(f"  Frame {i+1}: ✅ Main détectée (aire: {area})")
                    else:
                        print(f"  Frame {i+1}: ❌ Pas de main")
                else:
                    print(f"  Frame {i+1}: ❌ Aucun contour")
        
        cap.release()
        return True
    
    except Exception as e:
        print(f"❌ Erreur: {e}")
        return False

def interactive_demo():
    """Démo interactive"""
    print("\n" + "="*60)
    print("🎮 DÉMO INTERACTIVE (WINDOWS)")
    print("="*60)
    
    try:
        import cv2
        import numpy as np
        
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("❌ Caméra non trouvée")
            return False
        
        print("▶️  Démarrage de la caméra...")
        print("    - Appuyer sur 'SPACE' pour capture")
        print("    - Appuyer sur 'Q' pour quitter")
        
        while True:
            ret, frame = cap.read()
            if not ret:
                print("Erreur de capture")
                break
            
            # Détection basique peau
            hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
            lower_skin = np.array([0, 20, 70], dtype=np.uint8)
            upper_skin = np.array([20, 255, 255], dtype=np.uint8)
            mask = cv2.inRange(hsv, lower_skin, upper_skin)
            
            # Afficher
            cv2.imshow("Détection Main (Windows)", frame)
            cv2.imshow("Mask Détection", mask)
            
            key = cv2.waitKey(1) & 0xFF
            
            if key == ord('q') or key == ord('Q'):
                break
            elif key == ord(' '):
                cv2.imwrite("capture_windows.jpg", frame)
                print("✅ Capture sauvegardée: capture_windows.jpg")
        
        cap.release()
        cv2.destroyAllWindows()
        return True
    
    except Exception as e:
        print(f"❌ Erreur: {e}")
        return False

def show_menu():
    """Menu principal"""
    print("\n" + "="*60)
    print("🎥 DÉTECTION DE MAINS - WINDOWS 🎥")
    print("="*60)
    print("\n1. Vérifier OpenCV")
    print("2. Tester la caméra")
    print("3. Capture d'une image")
    print("4. Test de détection")
    print("5. Démo interactive")
    print("6. Installer dépendances")
    print("0. Quitter")
    print("="*60)

def install_dependencies():
    """Installer les dépendances"""
    print("\n" + "="*60)
    print("📦 INSTALLATION DES DÉPENDANCES")
    print("="*60)
    
    print("\nInstallation de OpenCV...")
    subprocess.run([sys.executable, "-m", "pip", "install", "opencv-python"])
    
    print("\nInstallation de MediaPipe...")
    subprocess.run([sys.executable, "-m", "pip", "install", "mediapipe"])
    
    print("\n✅ Installation terminée")

def main():
    """Boucle principale"""
    print(f"\n{'='*60}")
    print("🎥 SYSTÈME DE DÉTECTION DE MAINS - WINDOWS 🎥")
    print(f"{'='*60}")
    print(f"Système: {platform.system()} {platform.release()}")
    print(f"Python: {sys.version.split()[0]}")
    
    if not check_windows():
        print("⚠️  Attention: Vous n'êtes pas sur Windows")
    
    while True:
        show_menu()
        
        choice = input("\nChoisir une option (0-6): ").strip()
        
        if choice == "1":
            test_opencv()
        
        elif choice == "2":
            check_camera()
        
        elif choice == "3":
            test_basic_capture()
        
        elif choice == "4":
            test_detection()
        
        elif choice == "5":
            interactive_demo()
        
        elif choice == "6":
            install_dependencies()
        
        elif choice == "0":
            print("\nAu revoir! 👋\n")
            break
        
        else:
            print("❌ Choix invalide")
        
        input("\nAppuyer sur Entrée pour continuer...")

if __name__ == "__main__":
    main()
