#ifndef GUI_WINDOW_H
#define GUI_WINDOW_H

#include <string>
#include <vector>
#include <functional>

/**
 * @class GUIWindow
 * @brief Classe principale pour la fenêtre graphique du gestionnaire d'agents
 */
class GUIWindow {
private:
    int window_width;
    int window_height;
    std::string window_title;
    bool is_running;

public:
    GUIWindow(int width = 1200, int height = 800, const std::string& title = "Agent Manager - GUI");
    ~GUIWindow() = default;

    // Gestion de la fenêtre
    void create();
    void run();
    void close();
    bool isRunning() const { return is_running; }

    // Interface utilisateur
    void renderMainMenu();
    void renderAgentList();
    void renderExecutionPanel();
    void renderLogs();

    // Accesseurs
    int getWidth() const { return window_width; }
    int getHeight() const { return window_height; }
};

#endif // GUI_WINDOW_H
