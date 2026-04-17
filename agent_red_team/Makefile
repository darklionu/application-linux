CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I.
LDFLAGS = -lm

TARGET = agent_manager_app
SRCS = ui_menu.c agent_engine.c tool_manager.c
OBJS = $(SRCS:.c=.o)
HEADERS = agent_types.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "[+] Compilation réussie: $(TARGET)"

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "[+] Nettoyage terminé"

rebuild: clean all

.PHONY: all run clean rebuild
