CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I.

TARGETS = blue_monitor_agent blue_response_agent

all: $(TARGETS)

blue_monitor_agent: monitor_agent/blue_monitor_agent.c blue_agent.c
	$(CC) $(CFLAGS) -o $@ monitor_agent/blue_monitor_agent.c blue_agent.c

blue_response_agent: response_agent/blue_response_agent.c blue_agent.c
	$(CC) $(CFLAGS) -o $@ response_agent/blue_response_agent.c blue_agent.c

clean:
	rm -f $(TARGETS)
	@echo "[+] Nettoyage Blue Team terminé"

.PHONY: all clean
