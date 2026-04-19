#!/usr/bin/env python3
"""
Système de Détection de Mains avec Interface Interactive
Utilise la caméra pour détecter les mains et interagir avec une interface
"""

import cv2
import sys
import argparse
from typing import Optional

from hand_detector import HandDetector
from gesture_recognizer import GestureRecognizer
from interactive_interface import InteractiveInterface


class HandInteractionSystem:
    """Système d'interaction avec les mains"""
    
    def __init__(self, camera_id: int = 0, width: int = 800, height: int = 600):
        """
        Initialiser le système
        
        Args:
            camera_id: ID de la caméra
            width, height: Dimensions de la fenêtre
        """
        self.width = width
        self.height = height
        
        # Initialiser les composants
        self.detector = HandDetector(max_hands=2, detection_confidence=0.5)
        self.interface = InteractiveInterface(width, height)
        
        # Ouvrir la caméra
        self.cap = cv2.VideoCapture(camera_id)
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, width)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, height)
        self.cap.set(cv2.CAP_PROP_FPS, 30)
        
        if not self.cap.isOpened():
            raise RuntimeError("Impossible d'ouvrir la caméra")
        
        # État
        self.running = True
        self.current_gesture = "Aucun"
        self.interaction_log = []
    
    def run(self):
        """Lancer le système"""
        print("\n" + "="*60)
        print("🎥 SYSTÈME DE DÉTECTION DE MAINS - EN DIRECT")
        print("="*60)
        print("\nCommandes:")
        print("  - Pointez sur un bouton pour l'activer")
        print("  - Utilisez des gestes pour interagir")
        print("  - 'q' pour quitter")
        print("  - 'c' pour effacer l'historique")
        print("="*60 + "\n")
        
        frame_count = 0
        
        while self.running:
            ret, frame = self.cap.read()
            
            if not ret:
                print("Erreur: Impossible de lire la frame")
                break
            
            frame_count += 1
            
            # Détecter les mains
            hands, annotated_frame = self.detector.detect(frame)
            
            # Traiter chaque main
            for hand in hands:
                # Reconnaître le geste
                gesture_code, gesture_text = GestureRecognizer.recognize(hand)
                self.current_gesture = gesture_text
                
                # Obtenir le centre de la main
                hand_center = GestureRecognizer.get_hand_center(hand)
                
                # Normaliser les coordonnées (0-1)
                hand_x_norm = hand_center[0]
                hand_y_norm = hand_center[1]
                
                # Vérifier l'interaction avec l'interface
                element, value = self.interface.check_hand_interaction(hand_x_norm, hand_y_norm)
                
                if element:
                    self._log_interaction(hand, gesture_code, element, value)
                
                # Dessiner le point de la main sur la vidéo
                hand_x_pixel = int(hand_center[0] * annotated_frame.shape[1])
                hand_y_pixel = int(hand_center[1] * annotated_frame.shape[0])
                cv2.circle(annotated_frame, (hand_x_pixel, hand_y_pixel), 10, (0, 255, 255), -1)
                cv2.circle(annotated_frame, (hand_x_pixel, hand_y_pixel), 12, (255, 255, 0), 2)
            
            # Redimensionner pour l'interface
            combined = annotated_frame.copy()
            
            # Afficher les informations
            self.interface.display_info(combined, self.current_gesture, len(hands))
            
            # Afficher
            cv2.imshow("Détection de Mains - Vue Caméra", combined)
            
            # Afficher l'interface (si le nombre de mains > 0)
            if len(hands) > 0:
                interface_frame = self.interface.draw(combined.copy())
                cv2.imshow("Interface Interactive", interface_frame)
            
            # Affichage FPS et infos
            if frame_count % 30 == 0:
                print(f"Frame: {frame_count} | Mains: {len(hands)} | Geste: {self.current_gesture}")
            
            # Gestion des touches
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                self.running = False
                print("\n✓ Arrêt du programme...")
            elif key == ord('c'):
                self.interaction_log = []
                print("✓ Historique effacé")
            elif key == ord('s'):
                self._save_screenshot(annotated_frame)
    
    def _log_interaction(self, hand, gesture_code: str, element: str, value: Optional[float]):
        """Enregistrer une interaction"""
        log_entry = {
            'hand': hand.handedness,
            'gesture': gesture_code,
            'element': element,
            'value': value
        }
        self.interaction_log.append(log_entry)
        
        # Afficher
        print(f"  [{hand.handedness}] {gesture_code} → {element}", end="")
        if value is not None:
            print(f" = {value:.0f}", end="")
        print()
    
    def _save_screenshot(self, frame):
        """Sauvegarder une capture d'écran"""
        import time
        filename = f"hand_detection_{int(time.time())}.jpg"
        cv2.imwrite(filename, frame)
        print(f"✓ Capture sauvegardée: {filename}")
    
    def cleanup(self):
        """Nettoyer les ressources"""
        self.cap.release()
        self.detector.release()
        cv2.destroyAllWindows()
        
        # Afficher le résumé
        self._print_summary()
    
    def _print_summary(self):
        """Afficher un résumé"""
        print("\n" + "="*60)
        print("📊 RÉSUMÉ DE LA SESSION")
        print("="*60)
        print(f"Interactions enregistrées: {len(self.interaction_log)}")
        
        if self.interaction_log:
            print("\nDernières interactions:")
            for entry in self.interaction_log[-5:]:
                print(f"  - {entry['hand']} | {entry['gesture']} → {entry['element']}")
        
        print("="*60 + "\n")


def main():
    """Programme principal"""
    parser = argparse.ArgumentParser(
        description="Système de détection de mains avec interface interactive"
    )
    parser.add_argument("--camera", type=int, default=0, help="ID de la caméra (défaut: 0)")
    parser.add_argument("--width", type=int, default=800, help="Largeur (défaut: 800)")
    parser.add_argument("--height", type=int, default=600, help="Hauteur (défaut: 600)")
    
    args = parser.parse_args()
    
    try:
        system = HandInteractionSystem(
            camera_id=args.camera,
            width=args.width,
            height=args.height
        )
        system.run()
        system.cleanup()
        
    except KeyboardInterrupt:
        print("\n\n✓ Arrêt par l'utilisateur")
    except Exception as e:
        print(f"\n❌ Erreur: {e}")
        sys.exit(1)


if __name__ == "__main__":
    main()
