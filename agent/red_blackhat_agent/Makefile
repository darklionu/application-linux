CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I../../agent_red_team
LDFLAGS = -lm

TARGET = red_blackhat_agent
SRCS = red_blackhat_agent.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ ../../agent_red_team/agent_engine.c ../../agent_red_team/tool_manager.c $(LDFLAGS)
	@echo "[+] Compilation réussie: $(TARGET)"

clean:
	rm -f $(TARGET)
	@echo "[+] Nettoyage blackhat agent terminé"

.PHONY: all clean
