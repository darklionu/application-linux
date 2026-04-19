"""
Interface Interactive - Système de boutons et d'éléments interactifs
"""

import cv2
import numpy as np
from typing import Callable, Tuple, List, Optional
from dataclasses import dataclass


@dataclass
class Button:
    """Bouton interactif"""
    x: int
    y: int
    width: int
    height: int
    label: str
    color: Tuple[int, int, int]
    active_color: Tuple[int, int, int]
    callback: Optional[Callable] = None
    is_active: bool = False
    
    def contains_point(self, px: float, py: float) -> bool:
        """Vérifier si un point est dans le bouton"""
        return (self.x <= px <= self.x + self.width and
                self.y <= py <= self.y + self.height)
    
    def draw(self, frame: np.ndarray) -> np.ndarray:
        """Dessiner le bouton"""
        color = self.active_color if self.is_active else self.color
        
        cv2.rectangle(frame, (self.x, self.y),
                     (self.x + self.width, self.y + self.height),
                     color, -1)
        
        cv2.rectangle(frame, (self.x, self.y),
                     (self.x + self.width, self.y + self.height),
                     (255, 255, 255), 2)
        
        # Texte
        text_size = cv2.getTextSize(self.label, cv2.FONT_HERSHEY_SIMPLEX, 0.6, 1)[0]
        text_x = self.x + (self.width - text_size[0]) // 2
        text_y = self.y + (self.height + text_size[1]) // 2
        
        cv2.putText(frame, self.label,
                   (text_x, text_y),
                   cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                   (0, 0, 0), 2)
        
        return frame


@dataclass
class Slider:
    """Curseur interactif"""
    x: int
    y: int
    width: int
    height: int
    label: str
    min_val: float = 0.0
    max_val: float = 100.0
    current_val: float = 50.0
    color: Tuple[int, int, int] = (100, 100, 100)
    active_color: Tuple[int, int, int] = (0, 255, 0)
    is_active: bool = False
    
    def update(self, px: float) -> float:
        """Mettre à jour la valeur avec une position x"""
        if px < self.x:
            self.current_val = self.min_val
        elif px > self.x + self.width:
            self.current_val = self.max_val
        else:
            ratio = (px - self.x) / self.width
            self.current_val = self.min_val + ratio * (self.max_val - self.min_val)
        
        return self.current_val
    
    def draw(self, frame: np.ndarray) -> np.ndarray:
        """Dessiner le curseur"""
        # Barre de fond
        cv2.rectangle(frame, (self.x, self.y),
                     (self.x + self.width, self.y + self.height),
                     self.color, -1)
        
        # Curseur
        cursor_x = int(self.x + (self.current_val - self.min_val) / (self.max_val - self.min_val) * self.width)
        color = self.active_color if self.is_active else self.color
        cv2.circle(frame, (cursor_x, self.y + self.height // 2), 10, color, -1)
        
        # Texte
        cv2.putText(frame, f"{self.label}: {self.current_val:.0f}",
                   (self.x, self.y - 10),
                   cv2.FONT_HERSHEY_SIMPLEX, 0.5,
                   (255, 255, 255), 1)
        
        return frame


class InteractiveInterface:
    """Interface interactive avec des éléments contrôlables"""
    
    def __init__(self, width: int = 640, height: int = 480):
        """
        Initialiser l'interface
        
        Args:
            width: Largeur de l'interface
            height: Hauteur de l'interface
        """
        self.width = width
        self.height = height
        self.buttons: List[Button] = []
        self.sliders: List[Slider] = []
        self.selected_button: Optional[Button] = None
        
        self._create_default_interface()
    
    def _create_default_interface(self):
        """Créer l'interface par défaut"""
        # Boutons
        self.add_button(50, 50, 120, 50, "Clic", (50, 150, 50), (0, 255, 0))
        self.add_button(200, 50, 120, 50, "Effacer", (150, 50, 50), (255, 0, 0))
        self.add_button(350, 50, 120, 50, "Vu", (50, 50, 150), (0, 0, 255))
        
        # Curseurs
        self.add_slider(50, 150, 400, 20, "Luminosité", 0, 100, 50)
        self.add_slider(50, 200, 400, 20, "Saturation", 0, 100, 50)
        self.add_slider(50, 250, 400, 20, "Volume", 0, 100, 50)
    
    def add_button(self, x: int, y: int, width: int, height: int,
                   label: str, color: Tuple[int, int, int],
                   active_color: Tuple[int, int, int],
                   callback: Optional[Callable] = None) -> Button:
        """
        Ajouter un bouton
        
        Args:
            x, y: Position
            width, height: Dimensions
            label: Texte du bouton
            color: Couleur normale
            active_color: Couleur active
            callback: Fonction de rappel
            
        Returns:
            Le bouton créé
        """
        button = Button(x, y, width, height, label, color, active_color, callback)
        self.buttons.append(button)
        return button
    
    def add_slider(self, x: int, y: int, width: int, height: int,
                   label: str, min_val: float, max_val: float,
                   current_val: float = None) -> Slider:
        """
        Ajouter un curseur
        
        Args:
            x, y: Position
            width, height: Dimensions
            label: Texte du curseur
            min_val: Valeur min
            max_val: Valeur max
            current_val: Valeur actuelle
            
        Returns:
            Le curseur créé
        """
        if current_val is None:
            current_val = (min_val + max_val) / 2
        
        slider = Slider(x, y, width, height, label, min_val, max_val, current_val)
        self.sliders.append(slider)
        return slider
    
    def check_hand_interaction(self, hand_x: float, hand_y: float) -> Tuple[Optional[str], Optional[float]]:
        """
        Vérifier les interactions avec la main
        
        Args:
            hand_x, hand_y: Position normalisée de la main (0-1)
            
        Returns:
            Tuple (élément_cliqué, valeur) ou (None, None)
        """
        # Convertir les coordonnées normalisées en pixels
        px = int(hand_x * self.width)
        py = int(hand_y * self.height)
        
        # Vérifier les boutons
        for button in self.buttons:
            if button.contains_point(px, py):
                button.is_active = True
                if button.callback:
                    button.callback()
                return (f"button:{button.label}", None)
            else:
                button.is_active = False
        
        # Vérifier les curseurs
        for slider in self.sliders:
            if (slider.x <= px <= slider.x + slider.width and
                slider.y <= py <= slider.y + slider.height):
                slider.is_active = True
                value = slider.update(px)
                return (f"slider:{slider.label}", value)
            else:
                slider.is_active = False
        
        return (None, None)
    
    def draw(self, frame: np.ndarray) -> np.ndarray:
        """
        Dessiner l'interface
        
        Args:
            frame: Image à annoter
            
        Returns:
            Image annotée
        """
        # Fond gris
        frame[:] = (40, 40, 40)
        
        # Titre
        cv2.putText(frame, "Interface Interactive - Détection de Mains",
                   (10, 30),
                   cv2.FONT_HERSHEY_SIMPLEX, 0.8,
                   (0, 255, 255), 2)
        
        # Boutons
        for button in self.buttons:
            button.draw(frame)
        
        # Curseurs
        for slider in self.sliders:
            slider.draw(frame)
        
        return frame
    
    def display_info(self, frame: np.ndarray, gesture: str, hands_count: int) -> np.ndarray:
        """
        Afficher les informations
        
        Args:
            frame: Image à annoter
            gesture: Geste détecté
            hands_count: Nombre de mains détectées
            
        Returns:
            Image annotée
        """
        cv2.putText(frame, f"Mains détectées: {hands_count}",
                   (10, self.height - 50),
                   cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                   (255, 255, 0), 1)
        
        cv2.putText(frame, f"Geste: {gesture}",
                   (10, self.height - 20),
                   cv2.FONT_HERSHEY_SIMPLEX, 0.6,
                   (0, 255, 255), 1)
        
        return frame
