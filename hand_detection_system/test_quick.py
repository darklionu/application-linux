#!/usr/bin/env python3
"""
Test Rapide - Vérifier que tout fonctionne
"""

import sys
import cv2

print("\n" + "="*60)
print("TEST RAPIDE - SYSTÈME DE DÉTECTION DE MAINS")
print("="*60)

# Test 1: OpenCV
print("\n[1/3] Test OpenCV...", end=" ")
try:
    import cv2
    print("✓")
except ImportError:
    print("✗")
    print("  ❌ OpenCV non installé")
    sys.exit(1)

# Test 2: MediaPipe
print("[2/3] Test MediaPipe...", end=" ")
try:
    import mediapipe as mp
    print("✓")
except ImportError:
    print("✗")
    print("  ❌ MediaPipe non installé")
    sys.exit(1)

# Test 3: Caméra
print("[3/3] Test Caméra...", end=" ")
try:
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("✗")
        print("  ❌ Impossible d'accéder à la caméra")
        sys.exit(1)
    cap.release()
    print("✓")
except Exception as e:
    print("✗")
    print(f"  ❌ Erreur: {e}")
    sys.exit(1)

# Test 4: Modules personnalisés
print("\n[4/3] Test modules personnalisés...", end=" ")
try:
    from hand_detector import HandDetector
    from gesture_recognizer import GestureRecognizer
    from interactive_interface import InteractiveInterface
    print("✓")
except ImportError as e:
    print("✗")
    print(f"  ❌ Erreur: {e}")
    sys.exit(1)

print("\n" + "="*60)
print("✅ TOUS LES TESTS RÉUSSIS!")
print("="*60)
print("\nLe système est prêt à être utilisé.")
print("\nPour lancer:")
print("  python3 main.py")
print("\nPour les exemples:")
print("  python3 examples.py")
print("="*60 + "\n")
