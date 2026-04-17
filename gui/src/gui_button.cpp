#include "../include/gui_button.h"

Button::Button(const std::string& text, int x, int y, int w, int h)
    : label(text), x_pos(x), y_pos(y), width(w), height(h),
      is_hovered(false), is_clicked(false) {}

void Button::render() {
    // Placeholder pour le rendu (à implémenter avec SFML/Qt)
}

void Button::update(int mouse_x, int mouse_y, bool mouse_clicked) {
    // Vérifier si la souris est sur le bouton
    bool hovered = (mouse_x >= x_pos && mouse_x <= x_pos + width &&
                   mouse_y >= y_pos && mouse_y <= y_pos + height);
    
    is_hovered = hovered;
    
    if (hovered && mouse_clicked) {
        is_clicked = true;
        if (callback) {
            callback();
        }
    } else {
        is_clicked = false;
    }
}
