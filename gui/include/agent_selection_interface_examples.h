/**
 * EXEMPLES D'INTERFACES PERSONNALISÉES
 * 
 * Ce fichier montre différentes façons d'ajouter votre interface
 * personnalisée au système de sélection d'agents.
 * 
 * À AJOUTER dans: gui/include/agent_selection_interface.h
 * 
 * Exemples fournis:
 * 1. Interface Texte Avancée (ASCII Art)
 * 2. Interface Graphique Qt
 * 3. Interface Graphique SFML
 * 4. Interface Web (Serveur HTTP)
 */

// ============================================================================
// EXEMPLE 1: INTERFACE TEXTE AVANCÉE (ASCII ART)
// ============================================================================

#include <iomanip>

class AdvancedTextInterface : public AgentSelectionInterface {
private:
    AgentSelectionSystem& selection_system;

public:
    AdvancedTextInterface(AgentSelectionSystem& system)
        : selection_system(system) {}

    void displayDashboard() {
        std::cout << "\033[2J\033[H"; // Effacer l'écran
        
        std::cout << R"(
╔════════════════════════════════════════════════════════════════════════════╗
║                                                                            ║
║                  🎯 GESTIONNAIRE D'AGENTS - DASHBOARD                    ║
║                                                                            ║
╠════════════════════════════════════════════════════════════════════════════╣
║                                                                            ║
║  AGENTS DISPONIBLES          │  AGENTS SÉLECTIONNÉS      │  STATISTIQUES  ║
║  ─────────────────────────   │  ──────────────────────   │  ───────────── ║
)" << "\n";

        // Afficher les disponibles à gauche
        auto available = selection_system.getAvailableAgents();
        for (size_t i = 0; i < available.size() && i < 5; i++) {
            std::cout << "║  [" << (i+1) << "] " << std::setw(20) << available[i].name 
                      << " │  ";
        }

        // Afficher les sélectionnés au centre
        auto selected = selection_system.getSelectedAgents();
        std::cout << "  Sélectionnés: " << selected.size() << "/" 
                  << selection_system.getMaxAgentsAllowed() << " ║\n";

        std::cout << R"(║                          │                        │                    ║
╠════════════════════════════════════════════════════════════════════════════╣
║ COMMANDES RAPIDES:                                                         ║
║ s <nom>      - Sélectionner   |  c - Effacer  |  e - Exécuter  |  q - Quit║
╚════════════════════════════════════════════════════════════════════════════╝
)" << "\n";
    }

    void runAdvancedTextMenu() {
        while (true) {
            displayDashboard();
            
            std::cout << "➜ Commande: ";
            char cmd;
            std::cin >> cmd;
            
            if (cmd == 'q') break;
            else if (cmd == 's') {
                std::cout << "Nom de l'agent: ";
                std::string name;
                std::cin.ignore();
                std::getline(std::cin, name);
                selection_system.selectAgent(name);
            }
            else if (cmd == 'c') {
                selection_system.clearSelection();
            }
            else if (cmd == 'e') {
                executeSelectedAgents();
            }
            
            std::cout << "Appuyez sur Entrée...";
            std::cin.ignore();
        }
    }
};

// ============================================================================
// EXEMPLE 2: INTERFACE GRAPHIQUE QT (À COMPILER AVEC QT)
// ============================================================================

#ifdef USE_QT
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class QtAgentSelectionUI : public QMainWindow {
    Q_OBJECT

private:
    AgentSelectionSystem& selection_system;
    QListWidget* available_list;
    QListWidget* selected_list;
    QPushButton* select_btn;
    QPushButton* deselect_btn;
    QPushButton* execute_btn;

public:
    QtAgentSelectionUI(AgentSelectionSystem& system)
        : selection_system(system) {
        
        setWindowTitle("Agent Manager - Qt Interface");
        setGeometry(100, 100, 800, 600);
        
        // Créer layout
        QWidget* central = new QWidget;
        QHBoxLayout* main_layout = new QHBoxLayout;
        
        // Liste des agents disponibles
        QVBoxLayout* left_layout = new QVBoxLayout;
        left_layout->addWidget(new QLabel("Agents Disponibles:"));
        available_list = new QListWidget;
        left_layout->addWidget(available_list);
        
        // Boutons
        QVBoxLayout* button_layout = new QVBoxLayout;
        select_btn = new QPushButton("Sélectionner >");
        deselect_btn = new QPushButton("< Désélectionner");
        execute_btn = new QPushButton("Exécuter");
        button_layout->addWidget(select_btn);
        button_layout->addWidget(deselect_btn);
        button_layout->addWidget(execute_btn);
        
        // Liste des agents sélectionnés
        QVBoxLayout* right_layout = new QVBoxLayout;
        right_layout->addWidget(new QLabel("Agents Sélectionnés:"));
        selected_list = new QListWidget;
        right_layout->addWidget(selected_list);
        
        main_layout->addLayout(left_layout);
        main_layout->addLayout(button_layout);
        main_layout->addLayout(right_layout);
        
        central->setLayout(main_layout);
        setCentralWidget(central);
        
        // Remplir la liste des disponibles
        refreshAvailableList();
        
        // Connecter les signaux
        connect(select_btn, &QPushButton::clicked, this, &QtAgentSelectionUI::onSelect);
        connect(deselect_btn, &QPushButton::clicked, this, &QtAgentSelectionUI::onDeselect);
        connect(execute_btn, &QPushButton::clicked, this, &QtAgentSelectionUI::onExecute);
    }

private slots:
    void onSelect() {
        auto item = available_list->currentItem();
        if (item) {
            selection_system.selectAgent(item->text().toStdString());
            refreshUI();
        }
    }

    void onDeselect() {
        auto item = selected_list->currentItem();
        if (item) {
            selection_system.deselectAgent(item->text().toStdString());
            refreshUI();
        }
    }

    void onExecute() {
        if (selection_system.validateDependencies()) {
            // Lancer l'exécution
            auto selected = selection_system.getSelectedAgents();
            for (const auto& agent : selected) {
                // Exécuter agent.path
            }
        }
    }

private:
    void refreshAvailableList() {
        available_list->clear();
        auto available = selection_system.getAvailableAgents();
        for (const auto& agent : available) {
            available_list->addItem(QString::fromStdString(agent.name));
        }
    }

    void refreshUI() {
        refreshAvailableList();
        selected_list->clear();
        auto selected = selection_system.getSelectedAgents();
        for (const auto& agent : selected) {
            QString text = QString::number(agent.execution_order) + ". " 
                         + QString::fromStdString(agent.name);
            selected_list->addItem(text);
        }
    }
};
#endif // USE_QT

// ============================================================================
// EXEMPLE 3: INTERFACE GRAPHIQUE SFML (À COMPILER AVEC SFML)
// ============================================================================

#ifdef USE_SFML
#include <SFML/Graphics.hpp>

class SFMLAgentSelectionUI {
private:
    AgentSelectionSystem& selection_system;
    sf::RenderWindow window;
    
    struct Button {
        sf::RectangleShape shape;
        std::string label;
        std::function<void()> callback;
    };
    
    std::vector<Button> buttons;

public:
    SFMLAgentSelectionUI(AgentSelectionSystem& system)
        : selection_system(system),
          window(sf::VideoMode(1200, 800), "Agent Manager - SFML") {
        
        window.setFramerateLimit(60);
        createButtons();
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }

private:
    void createButtons() {
        // Créer des boutons pour chaque agent disponible
        auto available = selection_system.getAvailableAgents();
        
        for (size_t i = 0; i < available.size(); i++) {
            Button btn;
            btn.shape.setSize({150, 50});
            btn.shape.setPosition(50 + (i % 3) * 200, 100 + (i / 3) * 80);
            btn.shape.setFillColor(sf::Color::Blue);
            btn.label = available[i].name;
            
            // Capturer le nom dans une lambda
            auto name = available[i].name;
            btn.callback = [this, name]() {
                selection_system.selectAgent(name);
            };
            
            buttons.push_back(btn);
        }
    }

    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                
                for (auto& btn : buttons) {
                    if (btn.shape.getGlobalBounds().contains(
                        static_cast<sf::Vector2f>(mouse_pos))) {
                        btn.callback();
                    }
                }
            }
        }
    }

    void render() {
        window.clear(sf::Color::Black);
        
        // Dessiner les boutons
        for (auto& btn : buttons) {
            window.draw(btn.shape);
            // Ajouter le texte (nécessite sf::Font et sf::Text)
        }
        
        window.display();
    }
};
#endif // USE_SFML

// ============================================================================
// EXEMPLE 4: INTERFACE WEB (SERVEUR HTTP)
// ============================================================================

#include <thread>
#include <sstream>

class WebAgentSelectionUI {
private:
    AgentSelectionSystem& selection_system;
    int server_port;
    bool running;

public:
    WebAgentSelectionUI(AgentSelectionSystem& system, int port = 8080)
        : selection_system(system), server_port(port), running(false) {}

    void start() {
        running = true;
        std::cout << "Serveur démarré sur http://localhost:" << server_port << "\n";
        
        // Lancer le serveur dans un thread séparé
        // (Implémentation simplifiée - utiliser une vraie librairie HTTP en production)
    }

    void stop() {
        running = false;
    }

private:
    // Endpoints REST API
    std::string handleGetAgents() const {
        std::stringstream json;
        json << "{\"agents\": [";
        
        auto available = selection_system.getAvailableAgents();
        for (size_t i = 0; i < available.size(); i++) {
            if (i > 0) json << ",";
            json << "{\"name\": \"" << available[i].name 
                 << "\", \"type\": \"" << available[i].type << "\"}";
        }
        
        json << "]}";
        return json.str();
    }

    std::string handleSelectAgent(const std::string& agent_name) {
        bool success = selection_system.selectAgent(agent_name);
        return success ? "{\"status\": \"ok\"}" : "{\"status\": \"error\"}";
    }

    std::string handleExecute() {
        if (selection_system.validateDependencies()) {
            return "{\"status\": \"executing\"}";
        }
        return "{\"status\": \"error\", \"reason\": \"invalid dependencies\"}";
    }
};

// ============================================================================
// COMMENT UTILISER CES EXEMPLES
// ============================================================================

/*
 * OPTION 1: Interface Texte Avancée (Aucune dépendance)
 * 
 * Dans votre main:
 * 
 *   AdvancedTextInterface interface(selection_system);
 *   interface.runAdvancedTextMenu();
 * 
 * Compilation:
 *   g++ -std=c++17 -o app main.cpp
 * 
 * 
 * OPTION 2: Interface Qt (Dépend de Qt5/Qt6)
 * 
 * Définir la macro: -DUSE_QT
 * Compiler avec Qt tools: qmake && make
 * 
 * Dans votre main:
 *   QApplication app(argc, argv);
 *   QtAgentSelectionUI window(selection_system);
 *   window.show();
 *   return app.exec();
 * 
 * 
 * OPTION 3: Interface SFML (Dépend de SFML)
 * 
 * Définir la macro: -DUSE_SFML
 * Compiler: g++ -DUSE_SFML -lsfml-graphics -lsfml-window -lsfml-system
 * 
 * Dans votre main:
 *   SFMLAgentSelectionUI ui(selection_system);
 *   ui.run();
 * 
 * 
 * OPTION 4: Interface Web (Dépend d'une librairie HTTP)
 * 
 * Recommandations: crow, pistache, ou boost.asio
 * 
 * Dans votre main:
 *   WebAgentSelectionUI web_ui(selection_system, 8080);
 *   web_ui.start();
 *   // Navigateur: http://localhost:8080
 */

#endif // AGENT_SELECTION_INTERFACE_EXAMPLES_H
