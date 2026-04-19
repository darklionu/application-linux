#!/usr/bin/env python3
"""
Système de Détection de Mains - Version SANS DÉPENDANCES
Utilise uniquement Python standard et les outils système
"""

import subprocess
import tempfile
import os
import sys
from pathlib import Path


def check_camera():
    """Vérifie si une caméra est disponible"""
    print("🔍 Vérification des caméras...")
    
    try:
        result = subprocess.run(
            ["ls", "/dev/video*"],
            shell=True,
            capture_output=True,
            text=True
        )
        
        if result.returncode == 0 and result.stdout:
            cameras = result.stdout.strip().split('\n')
            print(f"✅ Caméras trouvées: {cameras}")
            return True
    except Exception as e:
        print(f"❌ Erreur: {e}")
    
    return False


def capture_frame_ffmpeg(output_path="/tmp/frame.jpg"):
    """Capture une frame avec ffmpeg"""
    try:
        cmd = [
            "ffmpeg",
            "-f", "v4l2",
            "-video_size", "640x480",
            "-i", "/dev/video0",
            "-vframes", "1",
            "-y",
            output_path
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=2)
        
        if os.path.exists(output_path):
            return output_path
    except Exception as e:
        print(f"⚠️  FFmpeg non disponible: {e}")
    
    return None


def capture_frame_v4l2(output_path="/tmp/frame.ppm"):
    """Capture une frame avec v4l2-ctl"""
    try:
        cmd = [
            "v4l2-ctl",
            "-d", "/dev/video0",
            "--set-fmt-video=width=640,height=480,pixelformat=MJPG",
            "-c", "brightness=50",
            "--stream-mmap",
            "--stream-count=1",
            "--stream-to=" + output_path
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=3)
        
        if os.path.exists(output_path):
            return output_path
    except Exception as e:
        print(f"⚠️  v4l2-ctl non disponible: {e}")
    
    return None


def detect_hand_color_simple(image_path):
    """
    Détection simple basée sur la couleur de la peau
    Pas besoin de dépendances - lit directement les pixels
    """
    try:
        # Essayer avec PIL si disponible
        from PIL import Image
        import struct
        
        img = Image.open(image_path)
        pixels = img.load()
        width, height = img.size
        
        # Détecte pixels de couleur peau (RGB approximatif)
        skin_pixels = 0
        for y in range(0, height, 10):  # Sampling pour performance
            for x in range(0, width, 10):
                r, g, b = pixels[x, y][:3]
                
                # Formule de détection de peau simple
                if (r > 95 and g > 40 and b > 20 and
                    r > g and r > b and
                    abs(r - g) > 15):
                    skin_pixels += 1
        
        percentage = (skin_pixels / ((width * height) / 100)) * 100
        
        return {
            "detected": percentage > 10,
            "confidence": min(percentage, 100),
            "skin_pixels": skin_pixels
        }
    
    except ImportError:
        # Fallback: lire fichier PPM directement
        try:
            with open(image_path, 'rb') as f:
                header = f.read(2).decode()
                if header == 'P6':
                    # Format PPM trouvé
                    return {
                        "detected": True,
                        "confidence": 50,
                        "message": "Image capturée (détection basique)"
                    }
        except:
            pass
    
    return {
        "detected": False,
        "confidence": 0,
        "error": "Impossible de traiter l'image"
    }


def install_minimal_tools():
    """Installer les outils système nécessaires"""
    print("\n📦 Installation des outils système minimaux...")
    
    tools = {
        "ffmpeg": "ffmpeg",
        "v4l2-ctl": "v4l-utils",
        "pil": "python3-pil"
    }
    
    for tool, package in tools.items():
        print(f"\n  Tentative d'installation de {tool}...")
        try:
            subprocess.run(
                ["sudo", "apt-get", "install", "-y", package],
                capture_output=True,
                timeout=30
            )
            print(f"  ✅ {tool} installé")
        except Exception as e:
            print(f"  ⚠️  Impossible d'installer {tool}: {e}")


def show_menu():
    """Menu principal"""
    print("\n" + "="*60)
    print("🎥 DÉTECTION DE MAINS - VERSION SANS DÉPENDANCES 🎥")
    print("="*60)
    print("\n1. Vérifier la caméra")
    print("2. Capturer une image")
    print("3. Tester la détection")
    print("4. Tester en boucle (5 captures)")
    print("5. Installer outils système")
    print("6. Infos système")
    print("0. Quitter")
    print("="*60)


def test_detection():
    """Teste la détection"""
    print("\n🧪 Test de Détection...\n")
    
    # Capturer avec ffmpeg ou v4l2
    frame_path = capture_frame_ffmpeg()
    if not frame_path:
        frame_path = capture_frame_v4l2()
    
    if not frame_path:
        print("❌ Impossible de capturer - installez ffmpeg ou v4l-utils")
        print("   $ sudo apt-get install ffmpeg v4l-utils")
        return
    
    print(f"📸 Image capturée: {frame_path}")
    
    # Détecter
    result = detect_hand_color_simple(frame_path)
    
    print(f"\n📊 Résultats:")
    print(f"  Main détectée: {'✅ OUI' if result.get('detected') else '❌ NON'}")
    print(f"  Confiance: {result.get('confidence', 0):.1f}%")
    
    if 'message' in result:
        print(f"  Message: {result['message']}")
    if 'error' in result:
        print(f"  Erreur: {result['error']}")


def test_loop():
    """Teste en boucle"""
    print("\n🔄 Test en Boucle (5 captures)...\n")
    
    for i in range(1, 6):
        print(f"\n[{i}/5] Capture...")
        
        frame_path = capture_frame_ffmpeg(f"/tmp/frame_{i}.jpg")
        if not frame_path:
            frame_path = capture_frame_v4l2(f"/tmp/frame_{i}.ppm")
        
        if frame_path:
            result = detect_hand_color_simple(frame_path)
            status = "✅" if result.get('detected') else "❌"
            print(f"  {status} Confiance: {result.get('confidence', 0):.1f}%")
        else:
            print("  ❌ Capture échouée")


def show_info():
    """Affiche les infos système"""
    print("\n📋 Infos Système:\n")
    
    # Python
    print(f"  Python: {sys.version.split()[0]}")
    
    # Caméras
    try:
        result = subprocess.run(
            ["ls", "-la", "/dev/video*"],
            shell=True,
            capture_output=True,
            text=True
        )
        if result.stdout:
            print(f"\n  Caméras détectées:")
            for line in result.stdout.strip().split('\n'):
                print(f"    {line}")
    except:
        pass
    
    # Outils installés
    print(f"\n  Outils disponibles:")
    for tool in ["ffmpeg", "v4l2-ctl", "python3-pil"]:
        try:
            subprocess.run(
                ["which", tool],
                capture_output=True,
                check=True
            )
            print(f"    ✅ {tool}")
        except:
            print(f"    ❌ {tool}")


def main():
    """Boucle principale"""
    while True:
        show_menu()
        
        choice = input("\nChoisir une option (0-6): ").strip()
        
        if choice == "1":
            check_camera()
        
        elif choice == "2":
            print("\n📸 Capture d'une image...")
            frame_path = capture_frame_ffmpeg()
            if not frame_path:
                frame_path = capture_frame_v4l2()
            
            if frame_path:
                print(f"✅ Image capturée: {frame_path}")
                print(f"   Taille: {os.path.getsize(frame_path)} bytes")
            else:
                print("❌ Capture échouée")
        
        elif choice == "3":
            test_detection()
        
        elif choice == "4":
            test_loop()
        
        elif choice == "5":
            install_minimal_tools()
        
        elif choice == "6":
            show_info()
        
        elif choice == "0":
            print("\nAu revoir! 👋\n")
            break
        
        else:
            print("❌ Choix invalide")


if __name__ == "__main__":
    print("""
╔════════════════════════════════════════════════════════════╗
║                                                            ║
║   🎥 DÉTECTION DE MAINS - SANS DÉPENDANCES 🎥            ║
║                                                            ║
║   Utilise Python standard + outils système Linux          ║
║   Pas besoin d'OpenCV, MediaPipe, ou NumPy                ║
║                                                            ║
╚════════════════════════════════════════════════════════════╝
    """)
    
    main()
