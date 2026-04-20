# TOOLS SYSTEM - Makefile
# ========================
# Compilation des outils Red Team

CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lssl -lcrypto

# Outils
TOOLS = analyse_de_fichier hdra_ssh nmap_scanner analyse_fichier_john

# Cibles
.PHONY: all clean help

all: $(TOOLS)

# Analyse de fichier
analyse_de_fichier: analyse_de_fichier.c
	$(CC) $(CFLAGS) -o $@ $< -lmagic

# SSH Hardening Red Agent
hdra_ssh: hdra_ssh.c
	$(CC) $(CFLAGS) -o $@ $< -lssh

# Nmap Scanner
nmap_scanner: nmap_scanner.c
	$(CC) $(CFLAGS) -o $@ $< -lm

# John Hash Cracker
analyse_fichier_john: analyse_fichier_john.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Nettoyage
clean:
	rm -f $(TOOLS) *.o

# Compilation simple sans dépendances externes
simple:
	$(CC) $(CFLAGS) -o analyse_de_fichier analyse_de_fichier.c
	$(CC) $(CFLAGS) -o nmap_scanner nmap_scanner.c
	$(CC) $(CFLAGS) -o analyse_fichier_john analyse_fichier_john.c $(LDFLAGS)
	@echo "[+] Compilation simple terminée (hdra_ssh requis libssh)"

help:
	@echo "Outils disponibles:"
	@echo "  - analyse_de_fichier: Analyse fichiers (texte, PDF, archives)"
	@echo "  - hdra_ssh: Audit et exploitation SSH"
	@echo "  - nmap_scanner: Scan réseau et ports TCP"
	@echo "  - analyse_fichier_john: Cracking de hashes et audit mots de passe"
	@echo ""
	@echo "Cibles Make:"
	@echo "  make all         - Compile tous les outils"
	@echo "  make simple      - Compile sans dépendances externes"
	@echo "  make clean       - Supprime les binaires"
	@echo "  make help        - Affiche cette aide"
