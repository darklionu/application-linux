"""
Reconnaissance des Gestes - Détecte les gestes avec la main
"""

import math
from typing import Tuple, List
from hand_detector import Hand


class GestureRecognizer:
    """Reconnaissance des gestes avec les mains"""
    
    # Gestes disponibles
    GESTURES = {
        "PALM": "Paume ouverte",
        "FIST": "Poing fermé",
        "PINCH": "Geste de pincer (pouce + index)",
        "PEACE": "Signe de paix (index + majeur)",
        "OK": "Signe OK (index + pouce)",
        "POINTING": "Pointage (index)",
        "THUMBS_UP": "Pouce vers le haut",
        "THUMBS_DOWN": "Pouce vers le bas"
    }
    
    @staticmethod
    def distance(p1: Tuple[float, float], p2: Tuple[float, float]) -> float:
        """Calculer la distance entre deux points"""
        return math.sqrt((p1[0] - p2[0]) ** 2 + (p1[1] - p2[1]) ** 2)
    
    @staticmethod
    def is_finger_up(hand: Hand, finger_tip_idx: int, finger_pip_idx: int) -> bool:
        """
        Vérifier si un doigt est levé
        
        Args:
            hand: Main détectée
            finger_tip_idx: Index du bout du doigt (landmark)
            finger_pip_idx: Index de la première articulation du doigt
            
        Returns:
            True si le doigt est levé
        """
        tip_y = hand.landmarks[finger_tip_idx][1]
        pip_y = hand.landmarks[finger_pip_idx][1]
        return tip_y < pip_y
    
    @classmethod
    def recognize(cls, hand: Hand) -> Tuple[str, str]:
        """
        Reconnaître le geste de la main
        
        Args:
            hand: Main détectée
            
        Returns:
            Tuple de (code_geste, description)
        """
        # Calculer les états des doigts
        # 4: Pouce, 8: Index, 12: Majeur, 16: Annulaire, 20: Auriculaire
        # TIP: bout du doigt, PIP: articulation (PIP est généralement -2 points)
        
        thumb_up = cls.is_finger_up(hand, 4, 3)
        index_up = cls.is_finger_up(hand, 8, 6)
        middle_up = cls.is_finger_up(hand, 12, 10)
        ring_up = cls.is_finger_up(hand, 16, 14)
        pinky_up = cls.is_finger_up(hand, 20, 18)
        
        # Calculer les distances
        thumb_index_dist = cls.distance(hand.thumb_tip, hand.index_tip)
        index_middle_dist = cls.distance(hand.index_tip, hand.middle_tip)
        
        # Paume ouverte: tous les doigts levés
        if thumb_up and index_up and middle_up and ring_up and pinky_up:
            return "PALM", cls.GESTURES["PALM"]
        
        # Poing fermé: aucun doigt levé
        if not thumb_up and not index_up and not middle_up and not ring_up and not pinky_up:
            return "FIST", cls.GESTURES["FIST"]
        
        # Signe de paix: index + majeur levés
        if index_up and middle_up and not ring_up and not pinky_up and thumb_up:
            return "PEACE", cls.GESTURES["PEACE"]
        
        # Geste OK: pouce + index rapprochés, autres doigts levés
        if thumb_up and index_up and thumb_index_dist < 0.05:
            if middle_up and ring_up and pinky_up:
                return "OK", cls.GESTURES["OK"]
        
        # Pointage: index levé, autres bas
        if index_up and not middle_up and not ring_up and not pinky_up:
            return "POINTING", cls.GESTURES["POINTING"]
        
        # Pincer: pouce + index rapprochés
        if thumb_up and index_up and thumb_index_dist < 0.05:
            return "PINCH", cls.GESTURES["PINCH"]
        
        # Pouce vers le haut: pouce levé vers le haut
        if thumb_up and not index_up and not middle_up and not ring_up and not pinky_up:
            # Vérifier la direction (vers le haut)
            palm = hand.palm_center
            thumb = hand.thumb_tip
            if thumb[1] < palm[1]:
                return "THUMBS_UP", cls.GESTURES["THUMBS_UP"]
            else:
                return "THUMBS_DOWN", cls.GESTURES["THUMBS_DOWN"]
        
        return "UNKNOWN", "Geste inconnu"
    
    @classmethod
    def get_hand_center(cls, hand: Hand) -> Tuple[float, float]:
        """
        Obtenir le centre de la main (moyenne des landmarks)
        
        Args:
            hand: Main détectée
            
        Returns:
            Tuple (x, y) du centre
        """
        x_sum = sum(lm[0] for lm in hand.landmarks)
        y_sum = sum(lm[1] for lm in hand.landmarks)
        
        x_center = x_sum / len(hand.landmarks)
        y_center = y_sum / len(hand.landmarks)
        
        return (x_center, y_center)
    
    @classmethod
    def get_finger_direction(cls, hand: Hand) -> Tuple[float, float]:
        """
        Obtenir la direction du pointage (si index levé)
        
        Args:
            hand: Main détectée
            
        Returns:
            Tuple (dx, dy) direction normalisée
        """
        # Direction depuis la paume vers le bout de l'index
        palm = hand.palm_center
        index = hand.index_tip
        
        dx = index[0] - palm[0]
        dy = index[1] - palm[1]
        
        # Normaliser
        dist = math.sqrt(dx**2 + dy**2)
        if dist == 0:
            return (0, 0)
        
        return (dx / dist, dy / dist)
