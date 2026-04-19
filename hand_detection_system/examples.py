#!/usr/bin/env python3
"""
Exemples Avancés - Utilisation du système de détection de mains
"""

import cv2
from hand_detector import HandDetector
from gesture_recognizer import GestureRecognizer


def example_1_basic_detection():
    """Exemple 1: Détection basique"""
    print("="*60)
    print("Exemple 1: Détection Basique")
    print("="*60)
    
    detector = HandDetector(max_hands=2, detection_confidence=0.5)
    cap = cv2.VideoCapture(0)
    
    frame_count = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        frame_count += 1
        
        # Détecter les mains
        hands, annotated = detector.detect(frame)
        
        # Afficher
        cv2.imshow("Détection de Mains", annotated)
        
        # Affichage tous les 30 frames
        if frame_count % 30 == 0:
            print(f"Frame {frame_count}: {len(hands)} main(s) détectée(s)")
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    detector.release()
    cv2.destroyAllWindows()


def example_2_gesture_recognition():
    """Exemple 2: Reconnaissance de gestes"""
    print("="*60)
    print("Exemple 2: Reconnaissance de Gestes")
    print("="*60)
    
    detector = HandDetector()
    cap = cv2.VideoCapture(0)
    
    gesture_log = []
    
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        hands, annotated = detector.detect(frame)
        
        for hand in hands:
            # Reconnaître le geste
            gesture_code, gesture_text = GestureRecognizer.recognize(hand)
            gesture_log.append(gesture_code)
            
            # Afficher
            detector.draw_hand_info(annotated, hand, (10, 50))
            
            cv2.putText(
                annotated,
                f"Geste: {gesture_text}",
                (10, 100),
                cv2.FONT_HERSHEY_SIMPLEX,
                1.0,
                (0, 255, 0),
                2
            )
        
        cv2.imshow("Reconnaissance de Gestes", annotated)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    detector.release()
    cv2.destroyAllWindows()
    
    # Afficher le résumé
    print(f"\nGestes détectés ({len(gesture_log)} total):")
    for gesture in set(gesture_log):
        count = gesture_log.count(gesture)
        print(f"  - {gesture}: {count}x")


def example_3_hand_features():
    """Exemple 3: Extraction des caractéristiques de la main"""
    print("="*60)
    print("Exemple 3: Caractéristiques de la Main")
    print("="*60)
    
    detector = HandDetector()
    cap = cv2.VideoCapture(0)
    
    frame_count = 0
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        frame_count += 1
        
        hands, annotated = detector.detect(frame)
        
        for hand in hands:
            h, w = annotated.shape[:2]
            
            # Obtenir les caractéristiques
            palm_center = hand.palm_center
            index_tip = hand.index_tip
            thumb_tip = hand.thumb_tip
            distance = GestureRecognizer.distance(palm_center, index_tip)
            
            # Convertir en pixels
            palm_px = (int(palm_center[0] * w), int(palm_center[1] * h))
            index_px = (int(index_tip[0] * w), int(index_tip[1] * h))
            thumb_px = (int(thumb_tip[0] * w), int(thumb_tip[1] * h))
            
            # Dessiner
            cv2.circle(annotated, palm_px, 5, (0, 255, 0), -1)
            cv2.circle(annotated, index_px, 5, (0, 0, 255), -1)
            cv2.circle(annotated, thumb_px, 5, (255, 0, 0), -1)
            cv2.line(annotated, palm_px, index_px, (255, 255, 0), 2)
            
            # Texte
            cv2.putText(
                annotated,
                f"Main: {hand.handedness}",
                (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.7,
                (0, 255, 0),
                2
            )
            
            cv2.putText(
                annotated,
                f"Distance: {distance:.2f}",
                (10, 70),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.7,
                (255, 255, 0),
                2
            )
        
        cv2.imshow("Caractéristiques de la Main", annotated)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    detector.release()
    cv2.destroyAllWindows()


def example_4_two_hands():
    """Exemple 4: Détection de deux mains"""
    print("="*60)
    print("Exemple 4: Détection de Deux Mains")
    print("="*60)
    
    detector = HandDetector(max_hands=2)
    cap = cv2.VideoCapture(0)
    
    left_gestures = []
    right_gestures = []
    
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        hands, annotated = detector.detect(frame)
        
        for i, hand in enumerate(hands):
            gesture_code, gesture_text = GestureRecognizer.recognize(hand)
            
            if hand.handedness == "Right":
                right_gestures.append(gesture_code)
            else:
                left_gestures.append(gesture_code)
            
            y_pos = 50 + (i * 50)
            
            cv2.putText(
                annotated,
                f"{hand.handedness}: {gesture_text}",
                (10, y_pos),
                cv2.FONT_HERSHEY_SIMPLEX,
                0.7,
                (0, 255, 0),
                2
            )
        
        cv2.putText(
            annotated,
            f"Mains détectées: {len(hands)}",
            (10, annotated.shape[0] - 20),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.7,
            (0, 255, 255),
            2
        )
        
        cv2.imshow("Deux Mains", annotated)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    detector.release()
    cv2.destroyAllWindows()
    
    # Résumé
    print(f"\nRésumé:")
    print(f"  Main gauche: {len(left_gestures)} gestes")
    print(f"  Main droite: {len(right_gestures)} gestes")


def example_5_gesture_counting():
    """Exemple 5: Compter les gestes"""
    print("="*60)
    print("Exemple 5: Comptage des Doigts Levés")
    print("="*60)
    
    detector = HandDetector()
    cap = cv2.VideoCapture(0)
    
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        
        hands, annotated = detector.detect(frame)
        
        for hand in hands:
            # Compter les doigts levés
            fingers_up = [
                GestureRecognizer.is_finger_up(hand, 4, 3),   # Pouce
                GestureRecognizer.is_finger_up(hand, 8, 6),   # Index
                GestureRecognizer.is_finger_up(hand, 12, 10), # Majeur
                GestureRecognizer.is_finger_up(hand, 16, 14), # Annulaire
                GestureRecognizer.is_finger_up(hand, 20, 18)  # Auriculaire
            ]
            
            count = sum(fingers_up)
            
            finger_names = ["Pouce", "Index", "Majeur", "Annulaire", "Auriculaire"]
            
            y_pos = 30
            for name, is_up in zip(finger_names, fingers_up):
                color = (0, 255, 0) if is_up else (0, 0, 255)
                status = "↑" if is_up else "↓"
                
                cv2.putText(
                    annotated,
                    f"{name}: {status}",
                    (10, y_pos),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.6,
                    color,
                    2
                )
                y_pos += 30
            
            # Nombre total
            cv2.putText(
                annotated,
                f"Total: {count}",
                (10, y_pos + 10),
                cv2.FONT_HERSHEY_SIMPLEX,
                1.0,
                (0, 255, 255),
                3
            )
        
        cv2.imshow("Comptage des Doigts", annotated)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    cap.release()
    detector.release()
    cv2.destroyAllWindows()


def main():
    """Menu principal"""
    print("\n" + "="*60)
    print("EXEMPLES AVANCÉS - DÉTECTION DE MAINS")
    print("="*60)
    print("\n1. Détection Basique")
    print("2. Reconnaissance de Gestes")
    print("3. Caractéristiques de la Main")
    print("4. Détection de Deux Mains")
    print("5. Comptage des Doigts Levés")
    print("0. Quitter")
    print("="*60)
    
    choice = input("\nChoisir un exemple (0-5): ").strip()
    
    if choice == "1":
        example_1_basic_detection()
    elif choice == "2":
        example_2_gesture_recognition()
    elif choice == "3":
        example_3_hand_features()
    elif choice == "4":
        example_4_two_hands()
    elif choice == "5":
        example_5_gesture_counting()
    elif choice == "0":
        print("Au revoir!")
    else:
        print("Choix invalide!")


if __name__ == "__main__":
    main()
