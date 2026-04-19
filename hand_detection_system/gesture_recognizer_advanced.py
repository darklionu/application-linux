"""
Reconnaissance des Gestes Améliorée
Ajoute: calibration, historique des gestes, meilleure précision
"""

import math
from typing import Tuple, List, Dict
from dataclasses import dataclass
from collections import deque
from hand_detector_optimized import Hand


@dataclass
class GestureEvent:
    """Événement de geste détecté"""
    gesture_code: str
    gesture_text: str
    confidence: float
    hand_center: Tuple[float, float]
    timestamp: float


class GestureRecognizerAdvanced:
    """Reconnaissance avancée des gestes"""
    
    GESTURES = {
        "PALM": "Paume ouverte",
        "FIST": "Poing fermé",
        "PINCH": "Geste de pincer",
        "PEACE": "Signe de paix",
        "OK": "Signe OK",
        "POINTING": "Pointage",
        "THUMBS_UP": "Pouce vers le haut",
        "THUMBS_DOWN": "Pouce vers le bas",
        "CALL": "Geste d'appel",
        "NONE": "Aucun geste"
    }
    
    def __init__(self, history_size: int = 10):
        """
        Initialiser le reconnaisseur
        
        Args:
            history_size: Nombre de gestes à mémoriser
        """
        self.gesture_history = deque(maxlen=history_size)
        self.last_gesture = "NONE"
        self.gesture_stability_counter = 0
        self.stability_threshold = 3  # Nombre de frames pour confirmer
        
        # Calibration par défaut
        self.calibration = {
            'pinch_distance': 0.05,
            'pointing_threshold': 0.15,
            'hand_size_scale': 1.0
        }
    
    @staticmethod
    def distance(p1: Tuple[float, float], p2: Tuple[float, float]) -> float:
        """Calculer la distance euclidienne"""
        return math.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)
    
    @staticmethod
    def is_finger_up(hand: Hand, finger_tip_idx: int, finger_pip_idx: int) -> bool:
        """Vérifier si un doigt est levé"""
        tip_y = hand.landmarks[finger_tip_idx][1]
        pip_y = hand.landmarks[finger_pip_idx][1]
        return tip_y < pip_y - 0.02  # Seuil de 2%
    
    @classmethod
    def recognize(cls, hand: Hand, confidence_threshold: float = 0.6) -> Tuple[str, str, float]:
        """
        Reconnaître le geste avec confiance
        
        Args:
            hand: Main détectée
            confidence_threshold: Seuil de confiance minimum
            
        Returns:
            Tuple (code, texte, confiance)
        """
        # États des doigts
        thumb_up = cls.is_finger_up(hand, 4, 3)
        index_up = cls.is_finger_up(hand, 8, 6)
        middle_up = cls.is_finger_up(hand, 12, 10)
        ring_up = cls.is_finger_up(hand, 16, 14)
        pinky_up = cls.is_finger_up(hand, 20, 18)
        
        # Distances clés
        thumb_index_dist = cls.distance(hand.thumb_tip, hand.index_tip)
        index_middle_dist = cls.distance(hand.index_tip, hand.middle_tip)
        middle_ring_dist = cls.distance(hand.middle_tip, hand.ring_tip)
        
        # Paume ouverte: tous les doigts levés
        if thumb_up and index_up and middle_up and ring_up and pinky_up:
            return "PALM", cls.GESTURES["PALM"], 0.95
        
        # Poing fermé: aucun doigt levé
        if not thumb_up and not index_up and not middle_up and not ring_up and not pinky_up:
            return "FIST", cls.GESTURES["FIST"], 0.95
        
        # Pincer: pouce + index rapprochés
        if thumb_up and index_up and thumb_index_dist < 0.06:
            if not middle_up and not ring_up and not pinky_up:
                return "PINCH", cls.GESTURES["PINCH"], 0.85
        
        # Signe de paix: index + majeur levés, écartés
        if index_up and middle_up and not ring_up and not pinky_up:
            if index_middle_dist > 0.05:
                return "PEACE", cls.GESTURES["PEACE"], 0.90
        
        # Pointage: index levé seul
        if index_up and not middle_up and not ring_up and not pinky_up and not thumb_up:
            return "POINTING", cls.GESTURES["POINTING"], 0.85
        
        # OK: pouce + index rapprochés, autres levés
        if thumb_up and index_up and thumb_index_dist < 0.06:
            if middle_up and ring_up and pinky_up:
                return "OK", cls.GESTURES["OK"], 0.80
        
        # Geste d'appel: auriculaire + index levés
        if index_up and pinky_up and not middle_up and not ring_up:
            return "CALL", cls.GESTURES["CALL"], 0.75
        
        # Pouce vers le haut
        if thumb_up and not index_up and not middle_up and not ring_up and not pinky_up:
            return "THUMBS_UP", cls.GESTURES["THUMBS_UP"], 0.80
        
        # Aucun geste reconnu
        return "NONE", cls.GESTURES["NONE"], 0.0
    
    @staticmethod
    def get_hand_center(hand: Hand) -> Tuple[float, float]:
        """Obtenir le centre de la main"""
        all_landmarks = hand.landmarks
        center_x = sum(lm[0] for lm in all_landmarks) / len(all_landmarks)
        center_y = sum(lm[1] for lm in all_landmarks) / len(all_landmarks)
        return (center_x, center_y)
    
    def update_history(self, gesture_code: str, hand_center: Tuple[float, float]):
        """Mettre à jour l'historique des gestes"""
        self.gesture_history.append((gesture_code, hand_center))
        
        # Vérifier la stabilité
        if gesture_code == self.last_gesture:
            self.gesture_stability_counter += 1
        else:
            self.gesture_stability_counter = 0
            self.last_gesture = gesture_code
    
    def is_gesture_stable(self) -> bool:
        """Vérifier si le geste actuel est stable"""
        return self.gesture_stability_counter >= self.stability_threshold
    
    def get_last_gestures(self, count: int = 5) -> List[str]:
        """Obtenir les derniers gestes"""
        return [g[0] for g in list(self.gesture_history)[-count:]]
    
    def calibrate_from_hand(self, hand: Hand):
        """
        Calibrer à partir d'une main détectée
        
        Args:
            hand: Main de référence pour calibration
        """
        # Calculer la taille de la main
        wrist = hand.landmarks[0]
        middle_mcp = hand.landmarks[9]
        hand_size = self.distance(
            (wrist[0], wrist[1]),
            (middle_mcp[0], middle_mcp[1])
        )
        
        if hand_size > 0:
            self.calibration['hand_size_scale'] = hand_size
