"""
Détecteur de Mains - Utilise MediaPipe pour détecter les mains
"""

import cv2
import mediapipe as mp
import numpy as np
from dataclasses import dataclass
from typing import List, Tuple, Optional

@dataclass
class Hand:
    """Représentation d'une main détectée"""
    landmarks: List[Tuple[float, float, float]]  # Position des 21 points
    handedness: str  # "Right" ou "Left"
    confidence: float  # Confiance de la détection (0-1)
    
    @property
    def palm_center(self) -> Tuple[float, float]:
        """Centre de la paume (point 0)"""
        return (self.landmarks[0][0], self.landmarks[0][1])
    
    @property
    def thumb_tip(self) -> Tuple[float, float]:
        """Bout du pouce (point 4)"""
        return (self.landmarks[4][0], self.landmarks[4][1])
    
    @property
    def index_tip(self) -> Tuple[float, float]:
        """Bout de l'index (point 8)"""
        return (self.landmarks[8][0], self.landmarks[8][1])
    
    @property
    def middle_tip(self) -> Tuple[float, float]:
        """Bout du majeur (point 12)"""
        return (self.landmarks[12][0], self.landmarks[12][1])
    
    @property
    def ring_tip(self) -> Tuple[float, float]:
        """Bout de l'annulaire (point 16)"""
        return (self.landmarks[16][0], self.landmarks[16][1])
    
    @property
    def pinky_tip(self) -> Tuple[float, float]:
        """Bout de l'auriculaire (point 20)"""
        return (self.landmarks[20][0], self.landmarks[20][1])


class HandDetector:
    """Détecteur de mains en temps réel"""
    
    def __init__(self, max_hands: int = 2, detection_confidence: float = 0.5):
        """
        Initialiser le détecteur
        
        Args:
            max_hands: Nombre maximal de mains à détecter
            detection_confidence: Seuil de confiance pour la détection (0-1)
        """
        self.mp_hands = mp.solutions.hands
        self.hands = self.mp_hands.Hands(
            static_image_mode=False,
            max_num_hands=max_hands,
            min_detection_confidence=detection_confidence,
            min_tracking_confidence=0.5
        )
        self.mp_drawing = mp.solutions.drawing_utils
        
    def detect(self, frame: np.ndarray) -> Tuple[List[Hand], np.ndarray]:
        """
        Détecter les mains dans une frame
        
        Args:
            frame: Image OpenCV (BGR)
            
        Returns:
            Tuple de (liste des mains détectées, image annotée)
        """
        # Convertir BGR en RGB
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        
        # Détecter les mains
        results = self.hands.process(rgb_frame)
        
        hands = []
        annotated_frame = frame.copy()
        
        if results.multi_hand_landmarks and results.multi_handedness:
            for landmarks, handedness_info in zip(
                results.multi_hand_landmarks,
                results.multi_handedness
            ):
                # Extraire les coordonnées
                hand_landmarks = [
                    (lm.x, lm.y, lm.z) for lm in landmarks.landmark
                ]
                
                # Créer objet Hand
                hand = Hand(
                    landmarks=hand_landmarks,
                    handedness=handedness_info.classification[0].label,
                    confidence=handedness_info.classification[0].score
                )
                hands.append(hand)
                
                # Dessiner les points et connexions
                self.mp_drawing.draw_landmarks(
                    annotated_frame,
                    landmarks,
                    self.mp_hands.HAND_CONNECTIONS,
                    landmark_drawing_spec=mp.solutions.drawing_utils.DrawingSpec(
                        color=(0, 255, 0),
                        thickness=1,
                        circle_radius=1
                    ),
                    connection_drawing_spec=mp.solutions.drawing_utils.DrawingSpec(
                        color=(255, 0, 0),
                        thickness=1
                    )
                )
        
        return hands, annotated_frame
    
    def draw_hand_info(self, frame: np.ndarray, hand: Hand, position: Tuple[int, int]) -> np.ndarray:
        """
        Dessiner les informations de la main
        
        Args:
            frame: Image OpenCV
            hand: Main détectée
            position: Position (x, y) pour le texte
            
        Returns:
            Image annotée
        """
        x, y = position
        
        # Texte
        cv2.putText(
            frame,
            f"{hand.handedness} Hand",
            (x, y),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.7,
            (0, 255, 0),
            2
        )
        
        cv2.putText(
            frame,
            f"Confidence: {hand.confidence:.2f}",
            (x, y + 30),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.5,
            (255, 255, 0),
            1
        )
        
        return frame
    
    def release(self):
        """Libérer les ressources"""
        self.hands.close()
