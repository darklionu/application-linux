# TOOLS SYSTEM - Red Team Toolkit
## Système d'Outils Intégré

### Vue d'Ensemble
Ensemble d'outils de sécurité offensive et défensive pour audit réseau, exploitation SSH, cracking de hashes et analyse de fichiers.

---

## Outils Disponibles

### 1. **analyse_de_fichier** - Analyse Fichiers
Extrait et analyse les fichiers (texte, PDF, archives).

```bash
./analyse_de_fichier <fichier>
```

**Formats supportés:**
- Texte (txt, md, log, conf)
- PDF (pdftotext requis)
- Archives (zip, tar, tar.gz, 7z)

**Fonctionnalités:**
- Détection type MIME
- Extraction statistiques texte
- Listing contenu archives
- Analyse de taille

**Dépendances:**
- libmagic (libmagic-dev)
- pdftotext (optionnel)

---

### 2. **hdra_ssh** - SSH Hardening & Defense Red Agent
Audit et exploitation SSH, test de connexions, audit de clés.

```bash
./hdra_ssh <action> <args...>
```

**Actions:**
- `banner <host> [port]` - Récupère bannière SSH
- `test <host> <user> <pass> [port]` - Test connexion
- `keyaudit <host> <user> [port]` - Audit clés SSH
- `brute <host> <user> <wordlist> [port]` - Brute force

**Exemples:**
```bash
./hdra_ssh banner 192.168.1.1
./hdra_ssh test 192.168.1.1 admin password123
./hdra_ssh keyaudit 192.168.1.1 root
./hdra_ssh brute 192.168.1.1 admin wordlist.txt
```

**Dépendances:**
- libssh-dev
- sshpass (optionnel)

---

### 3. **nmap_scanner** - Network Scanner
Scan réseau et ports TCP, résolution DNS, détection services.

```bash
./nmap_scanner <commande> <args...>
```

**Commandes:**
- `resolve <hostname>` - Résolution DNS
- `quick <ip>` - Scan ports courants
- `scan <ip> <port_min> <port_max>` - Scan plage ports
- `network <network>` - Scan réseau entier

**Exemples:**
```bash
./nmap_scanner quick 192.168.1.1
./nmap_scanner scan 192.168.1.1 1 1000
./nmap_scanner network 192.168.1.0/24
./nmap_scanner resolve example.com
```

**Caractéristiques:**
- TCP connect scan
- Détection services courants
- Timeout configurable
- Résolution DNS intégrée

---

### 4. **analyse_fichier_john** - Hash Cracking & Audit
Cracking de hashes et audit de mots de passe système.

```bash
./analyse_fichier_john <action> <args...>
```

**Actions:**
- `list <passwd>` - Énumère utilisateurs système
- `extract <shadow>` - Extrait hashes
- `crack <hash> <wordlist>` - Cracking par wordlist
- `detect <hash>` - Détecte type de hash
- `md5 <string>` - Génère MD5
- `sha256 <string>` - Génère SHA256

**Exemples:**
```bash
./analyse_fichier_john list /etc/passwd
./analyse_fichier_john extract /etc/shadow
./analyse_fichier_john detect "$6$abc123def..."
./analyse_fichier_john crack "$1$abc123" wordlist.txt
./analyse_fichier_john md5 "password123"
./analyse_fichier_john sha256 "password123"
```

**Types de hashes détectés:**
- MD5 / MD5-CRYPT
- SHA1 / SHA256 / SHA512
- SHA256-CRYPT / SHA512-CRYPT
- BCRYPT

**Dépendances:**
- libssl-dev (OpenSSL)

---

## Installation & Compilation

### Compilation complète
```bash
cd /workspaces/application-linux/tools
make all
```

### Compilation simple (sans dépendances externes optionnelles)
```bash
make simple
```

### Compilation individuelle
```bash
gcc -Wall -O2 -o analyse_de_fichier analyse_de_fichier.c -lmagic
gcc -Wall -O2 -o nmap_scanner nmap_scanner.c
gcc -Wall -O2 -o analyse_fichier_john analyse_fichier_john.c -lssl -lcrypto
gcc -Wall -O2 -o hdra_ssh hdra_ssh.c -lssh
```

### Installation des dépendances (Ubuntu/Debian)
```bash
sudo apt-get install -y \
    libmagic-dev \
    libssh-dev \
    libssl-dev \
    poppler-utils \
    sshpass
```

---

## Utilisation Avancée

### Scan réseau complet
```bash
./nmap_scanner network 192.168.1.0/24
./nmap_scanner quick 192.168.1.1
./nmap_scanner scan 192.168.1.1 1 65535
```

### Audit SSH
```bash
./hdra_ssh banner target.com
./hdra_ssh keyaudit target.com admin
```

### Cracking de hashes
```bash
# Extraire hashes du système
sudo ./analyse_fichier_john extract /etc/shadow

# Tenter cracking
./analyse_fichier_john crack '$6$rounds=656000$abc$...' rockyou.txt
```

### Analyse de fichier
```bash
./analyse_de_fichier rapport.pdf
./analyse_de_fichier archive.tar.gz
./analyse_de_fichier config.txt
```

---

## Problèmes Courants & Solutions

### ❌ Erreur: "cannot find -lmagic"
```bash
# Solution:
sudo apt-get install libmagic-dev
```

### ❌ Erreur: "cannot find -lssh"
```bash
# Solution:
sudo apt-get install libssh-dev
```

### ❌ Permissions refusées pour shadow
```bash
# Les fichiers système nécessitent root:
sudo ./analyse_fichier_john extract /etc/shadow
```

### ❌ Timeout de connexion SSH
```bash
# Augmentez le timeout en modifiant le source ou utilisez:
timeout 10 ./hdra_ssh test host user pass
```

---

## Amélioration du Système

### Version 2.0 Prévue
- [ ] Support HTTPS/TLS scan
- [ ] GPU acceleration pour cracking
- [ ] Intégration Redis pour cache
- [ ] API REST
- [ ] Interface Web
- [ ] Wordlist manager
- [ ] Custom payloads
- [ ] Logging centralisé

### Bugs Corrigés
✅ Suppression boucle symbolique infinite  
✅ Renommage fichiers avec espaces → noms lisibles  
✅ Completion fichiers vides → implémentations  
✅ Support formats multiples  
✅ Gestion erreurs robuste  

---

## Utilisation Légale ⚠️

**⚠️ Ces outils sont destinés uniquement à:**
- Tests de sécurité autorisés (pentest)
- Audit de sécurité interne
- Recherche en sécurité
- Environnements de test/lab

**Utilisation non autorisée = Illegal**

---

## Structure des Fichiers

```
tools/
├── Makefile                    # Compilation
├── README.md                   # Cette doc
├── analyse_de_fichier.c        # Analyse fichiers
├── hdra_ssh.c                  # SSH audit
├── nmap_scanner.c              # Network scan
└── analyse_fichier_john.c      # Hash cracking
```

---

## Logs & Verbosité

Tous les outils affichent:
- ✅ `[+]` Succès
- ⚠️ `[!]` Avertissement  
- ❌ `[-]` Erreur
- 🔵 `[*]` Information
- 🟣 `[>]` En-tête

Codes ANSI colorés supportés pour terminal.

---

## Support & Améliorations

Pour améliorer ce système:
1. Ajoutez de nouvelles fonctionnalités dans les fichiers .c
2. Testez avec `make simple` ou `make all`
3. Documentez les changes
4. Committez avec messages clairs

---

**Last Updated:** 2026-04-20  
**Version:** 2.0  
**Status:** Production Ready ✅
