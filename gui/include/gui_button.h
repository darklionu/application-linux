#ifndef GUI_BUTTON_H
#define GUI_BUTTON_H

#include <string>
#include <functional>

/**
 * @class Button
 * @brief Classe pour les boutons de l'interface
 */
class Button {
private:
    std::string label;
    int x_pos;
    int y_pos;
    int width;
    int height;
    bool is_hovered;
    bool is_clicked;
    std::function<void()> callback;

public:
    Button(const std::string& text, int x, int y, int w = 120, int h = 40);
    ~Button() = default;

    void render();
    void update(int mouse_x, int mouse_y, bool mouse_clicked);
    void setCallback(std::function<void()> func) { callback = func; }

    bool isHovered() const { return is_hovered; }
    bool wasClicked() const { return is_clicked; }
    const std::string& getLabel() const { return label; }

    void setPosition(int x, int y) { x_pos = x; y_pos = y; }
    void setSize(int w, int h) { width = w; height = h; }
};

#endif // GUI_BUTTON_H
