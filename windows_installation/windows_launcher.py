"""
Wrapper Windows pour la détection de mains
Compatible avec Windows 10/11, Python 3.7+
"""

import sys
import os
import subprocess

def install_dependencies():
    """Installer les dépendances si manquantes"""
    packages = ['opencv-python', 'mediapipe']
    
    for package in packages:
        try:
            __import__(package.replace('-', '_'))
        except ImportError:
            print(f"Installation de {package}...")
            subprocess.check_call([
                sys.executable, '-m', 'pip', 'install', package, '-q'
            ])

def main_windows():
    """Lancer le système de détection sur Windows"""
    import cv2
    from hand_detector_optimized import HandDetectorOptimized
    from gesture_recognizer_advanced import GestureRecognizerAdvanced
    
    print("\n" + "="*60)
    print("🖐️  SYSTÈME DE DÉTECTION DE MAINS - WINDOWS")
    print("="*60)
    print("\nCommandes:")
    print("  - Pointez une main vers la caméra")
    print("  - Appuyez sur 'q' pour quitter")
    print("="*60 + "\n")
    
    # Initialiser
    try:
        detector = HandDetectorOptimized(detection_confidence=0.7)
        recognizer = GestureRecognizerAdvanced()
    except Exception as e:
        print(f"Erreur: {e}")
        print("Assurez-vous que mediapipe est installé:")
        print("  pip install mediapipe")
        return
    
    # Ouvrir caméra
    try:
        cap = cv2.VideoCapture(0)
        if not cap.isOpened():
            print("❌ Erreur: Caméra non trouvée")
            print("Vérifiez que votre caméra est connectée et activée")
            return
        
        cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1280)
        cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 720)
        cap.set(cv2.CAP_PROP_FPS, 30)
        
        frame_count = 0
        while True:
            ret, frame = cap.read()
            if not ret:
                print("Erreur: Impossible de lire la caméra")
                break
            
            frame_count += 1
            
            # Détecter
            hands, annotated = detector.detect(frame)
            annotated = detector.draw_performance_info(annotated)
            
            # Afficher les gestes
            for i, hand in enumerate(hands):
                gesture_code, gesture_text, confidence = GestureRecognizerAdvanced.recognize(hand)
                cv2.putText(
                    annotated,
                    f"Geste: {gesture_text} ({confidence:.0%})",
                    (10, 100 + i*30),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.7,
                    (0, 255, 0),
                    2
                )
            
            cv2.imshow('Detection de Mains', annotated)
            
            # Quitter avec 'q'
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q') or key == 27:  # 'q' ou ESC
                break
        
        cap.release()
        cv2.destroyAllWindows()
        print("\n✓ Fermeture du système\n")
    
    except Exception as e:
        print(f"❌ Erreur: {e}")
        import traceback
        traceback.print_exc()

if __name__ == '__main__':
    print("Vérification des dépendances...")
    install_dependencies()
    print("✓ Dépendances OK\n")
    main_windows()
