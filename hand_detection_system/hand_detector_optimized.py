"""
Détecteur de Mains Optimisé - Performances améliorées et meilleure précision
Ajoute: threading, cache, filtre de stabilité, meilleure calibration
"""

import cv2
import mediapipe as mp
import numpy as np
import threading
from dataclasses import dataclass
from typing import List, Tuple, Optional, Dict
from collections import deque
import time


@dataclass
class Hand:
    """Représentation d'une main détectée"""
    landmarks: List[Tuple[float, float, float]]  # Position des 21 points
    handedness: str  # "Right" ou "Left"
    confidence: float  # Confiance de la détection (0-1)
    frame_id: int  # Numéro de la frame
    
    @property
    def palm_center(self) -> Tuple[float, float]:
        """Centre de la paume (point 0)"""
        return (self.landmarks[0][0], self.landmarks[0][1])
    
    @property
    def thumb_tip(self) -> Tuple[float, float]:
        return (self.landmarks[4][0], self.landmarks[4][1])
    
    @property
    def index_tip(self) -> Tuple[float, float]:
        return (self.landmarks[8][0], self.landmarks[8][1])
    
    @property
    def middle_tip(self) -> Tuple[float, float]:
        return (self.landmarks[12][0], self.landmarks[12][1])
    
    @property
    def ring_tip(self) -> Tuple[float, float]:
        return (self.landmarks[16][0], self.landmarks[16][1])
    
    @property
    def pinky_tip(self) -> Tuple[float, float]:
        return (self.landmarks[20][0], self.landmarks[20][1])


class HandDetectorOptimized:
    """Détecteur de mains optimisé avec amélioration de performances"""
    
    def __init__(
        self, 
        max_hands: int = 2, 
        detection_confidence: float = 0.7,  # Augmenté pour moins de faux positifs
        enable_cache: bool = True,
        smoothing_frames: int = 5
    ):
        """
        Initialiser le détecteur optimisé
        
        Args:
            max_hands: Nombre maximal de mains
            detection_confidence: Seuil de confiance (0-1)
            enable_cache: Utiliser le cache pour optimiser
            smoothing_frames: Nombre de frames pour lissage
        """
        self.mp_hands = mp.solutions.hands
        self.hands = self.mp_hands.Hands(
            static_image_mode=False,
            max_num_hands=max_hands,
            min_detection_confidence=detection_confidence,
            min_tracking_confidence=0.7  # Augmenté pour meilleur suivi
        )
        self.mp_drawing = mp.solutions.drawing_utils
        
        # Cache et lissage
        self.enable_cache = enable_cache
        self.smoothing_frames = smoothing_frames
        self.landmark_history: Dict[str, deque] = {}
        
        # Performance
        self.frame_id = 0
        self.last_detection_time = 0
        self.detection_time_avg = 0
        self.fps = 0
        
        # Threading
        self._lock = threading.Lock()
        self._latest_hands = []
        self._latest_frame = None
        
    def _smooth_landmarks(self, landmarks: List[Tuple[float, float, float]], 
                         hand_id: str) -> List[Tuple[float, float, float]]:
        """
        Lisser les landmarks avec un filtre temporel
        
        Args:
            landmarks: Landmarks bruts
            hand_id: Identifiant unique pour cette main
            
        Returns:
            Landmarks lissés
        """
        if hand_id not in self.landmark_history:
            self.landmark_history[hand_id] = deque(maxlen=self.smoothing_frames)
        
        self.landmark_history[hand_id].append(landmarks)
        
        # Moyenne des frames
        smoothed = []
        num_landmarks = len(landmarks)
        for i in range(num_landmarks):
            xs = [frame[i][0] for frame in self.landmark_history[hand_id]]
            ys = [frame[i][1] for frame in self.landmark_history[hand_id]]
            zs = [frame[i][2] for frame in self.landmark_history[hand_id]]
            
            smoothed.append((
                np.mean(xs),
                np.mean(ys),
                np.mean(zs)
            ))
        
        return smoothed
    
    def _filter_outliers(self, hand: Hand, prev_hands: List[Hand]) -> bool:
        """
        Filtrer les détections aberrantes
        
        Args:
            hand: Main détectée
            prev_hands: Mains précédentes
            
        Returns:
            True si la main est valide
        """
        if not prev_hands:
            return True
        
        # Vérifier la distance avec les mains précédentes
        palm_center = hand.palm_center
        for prev_hand in prev_hands:
            prev_center = prev_hand.palm_center
            distance = np.sqrt(
                (palm_center[0] - prev_center[0]) ** 2 + 
                (palm_center[1] - prev_center[1]) ** 2
            )
            
            # Si la distance est trop grande, c'est probable une détection fausse
            if distance > 0.3:  # 30% du cadre
                return False
        
        return True
    
    def detect(self, frame: np.ndarray) -> Tuple[List[Hand], np.ndarray]:
        """
        Détecter les mains avec optimisations
        
        Args:
            frame: Image OpenCV (BGR)
            
        Returns:
            Tuple de (liste des mains, image annotée)
        """
        start_time = time.time()
        self.frame_id += 1
        
        # Convertir BGR en RGB
        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        
        # Redimensionner pour optimiser les performances (mais garder le ratio)
        height, width = rgb_frame.shape[:2]
        if width > 1280:
            scale = 1280 / width
            new_width = 1280
            new_height = int(height * scale)
            rgb_frame = cv2.resize(rgb_frame, (new_width, new_height))
            resize_needed = True
        else:
            resize_needed = False
        
        # Détecter les mains
        results = self.hands.process(rgb_frame)
        
        hands = []
        annotated_frame = frame.copy()
        
        if results.multi_hand_landmarks and results.multi_handedness:
            for idx, (landmarks, handedness_info) in enumerate(
                zip(results.multi_hand_landmarks, results.multi_handedness)
            ):
                # Extraire les coordonnées
                hand_landmarks = [
                    (lm.x, lm.y, lm.z) for lm in landmarks.landmark
                ]
                
                # Appliquer le lissage si activé
                if self.enable_cache:
                    hand_id = f"{handedness_info.classification[0].label}_{idx}"
                    hand_landmarks = self._smooth_landmarks(hand_landmarks, hand_id)
                
                # Créer objet Hand
                hand = Hand(
                    landmarks=hand_landmarks,
                    handedness=handedness_info.classification[0].label,
                    confidence=handedness_info.classification[0].score,
                    frame_id=self.frame_id
                )
                
                # Filtrer les aberrations
                if self._filter_outliers(hand, hands):
                    hands.append(hand)
                    
                    # Dessiner les points et connexions
                    self.mp_drawing.draw_landmarks(
                        annotated_frame,
                        landmarks,
                        self.mp_hands.HAND_CONNECTIONS,
                        landmark_drawing_spec=mp.solutions.drawing_utils.DrawingSpec(
                            color=(0, 255, 0),
                            thickness=2,
                            circle_radius=2
                        ),
                        connection_drawing_spec=mp.solutions.drawing_utils.DrawingSpec(
                            color=(255, 0, 0),
                            thickness=2
                        )
                    )
        
        # Calculer les performances
        detection_time = time.time() - start_time
        self.detection_time_avg = (self.detection_time_avg * 0.7) + (detection_time * 0.3)
        if detection_time > 0:
            self.fps = 1.0 / detection_time
        
        return hands, annotated_frame
    
    def draw_hand_info(self, frame: np.ndarray, hand: Hand, 
                      position: Tuple[int, int] = (10, 30)) -> np.ndarray:
        """Dessiner les informations de la main"""
        x, y = position
        
        cv2.putText(
            frame,
            f"{hand.handedness} Hand (Conf: {hand.confidence:.2f})",
            (x, y),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.7,
            (0, 255, 0),
            2
        )
        
        return frame
    
    def draw_performance_info(self, frame: np.ndarray, position: Tuple[int, int] = (10, 60)) -> np.ndarray:
        """Dessiner les infos de performance"""
        x, y = position
        
        cv2.putText(
            frame,
            f"FPS: {self.fps:.1f} | Detection: {self.detection_time_avg*1000:.1f}ms",
            (x, y),
            cv2.FONT_HERSHEY_SIMPLEX,
            0.6,
            (255, 255, 0),
            1
        )
        
        return frame
    
    def get_stats(self) -> Dict[str, float]:
        """Obtenir les statistiques"""
        return {
            'fps': self.fps,
            'detection_time_ms': self.detection_time_avg * 1000,
            'frame_id': self.frame_id
        }
