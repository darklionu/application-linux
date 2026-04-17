CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -I./include
LDFLAGS := 

# Pour SFML (optionnel) - décommenter si SFML est installé
# CXXFLAGS += -lsfml-graphics -lsfml-window -lsfml-system
# LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

SOURCES := src/gui_agent_panel.cpp src/gui_button.cpp
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := libgui.a

.PHONY: all clean help

all: $(TARGET)
	@echo "[✓] Bibliothèque GUI compilée: $(TARGET)"

$(TARGET): $(OBJECTS)
	ar rcs $@ $^
	@echo "[✓] Bibliothèque statique créée"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJECTS) $(TARGET)
	@echo "[✓] Nettoyage terminé"

help:
	@echo "Commandes disponibles:"
	@echo "  make all     - Compiler la bibliothèque GUI"
	@echo "  make clean   - Nettoyer les fichiers compilés"
	@echo "  make help    - Afficher cette aide"
