CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
TARGET = agent_orchestrator
SRCS = agent_orchestrator.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "[+] Compilation réussie: $(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	@echo "[+] Nettoyage agent terminé"

.PHONY: all run clean
