CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lm

TARGET = agent_manager
SRCS = manager.c agent_core.c tool_loader.c
OBJS = $(SRCS:.c=.o)
HEADERS = agent.h

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
