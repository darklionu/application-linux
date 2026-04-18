# 🎯 Gestionnaire d'Agents Interactif - Résumé des modifications

## ✅ Objectifs atteints

### 1. **Consolidation du GUI en un seul fichier** ✓
Le dossier `gui/` qui contenait plusieurs fichiers C++ (`gui_button.cpp`, `gui_window.h`, `gui_agent_panel.cpp`, etc.) a été consolidé. Le gestionnaire principal est maintenant complètement scriptable via `quick_start_manager.sh`.

### 2. **Sélection des cibles par robot** ✓
Chaque agent peut maintenant attaquer des cibles spécifiques choisies par l'utilisateur :
- **8 cibles prédéfinies** : localhost, gateway, webserver, database, etc.
- **Sélection flexible** : Une cible, plusieurs cibles, ou toutes les cibles
- **Configuration simple** : Éditer `.targets.conf` pour ajouter/modifier les cibles

### 3. **Sélection des attaques** ✓
L'utilisateur peut choisir exactement quelles attaques exécuter :
- **15 attaques disponibles** : Ping Test, DNS Lookup, Port Scan, Brute Force, Injection SQL, XSS, DDoS, etc.
- **3 profils prédéfinis** : Basique (reconnaissance), Moyen (mix), Personnalisé
- **Configuration simple** : Éditer `.attacks.conf` pour ajouter/modifier les attaques

## 📋 Changements effectués

### Fichiers créés/modifiés

| Fichier | État | Description |
|---------|------|-------------|
| `quick_start_manager.sh` | **Modifié** | Gestionnaire interactif complet avec menus de sélection |
| `compile_all_agents.sh` | **Modifié** | Script de compilation de tous les agents |
| `INTERACTIVE_MANAGER.md` | **Créé** | Documentation complète du gestionnaire |
| `.targets.conf` | **Auto-créé** | Configuration des cibles (8 cibles par défaut) |
| `.attacks.conf` | **Auto-créé** | Configuration des attaques (15 attaques par défaut) |

### Agents compilés et prêts

✓ Red Recon Agent - Reconnaissance réseau
✓ Red Exploit Agent - Exploitation
✓ Red BlackHat Agent - Attaques avancées
✓ Blue Monitor Agent - Surveillance
✓ Blue Response Agent - Réponse

## 🚀 Mode d'emploi rapide

### Démarrage

```bash
cd /workspaces/application-linux
./quick_start_manager.sh
```

### Menu principal

```
Options:
  1) Lancer un scénario d'attaque personnalisé
  2) Lancer Red Team uniquement
  3) Lancer Blue Team uniquement
  4) Voir le dernier log
  5) Configurer les cibles
  6) Configurer les attaques
  0) Quitter
```

### Exemple d'utilisation

**Scénario : Attaque du serveur web avec reconnaissance complète**

```
1. Choisir option 1 (Scénario personnalisé)
2. Sélectionner cible 3 (webserver - 192.168.1.100)
3. Sélectionner attaques B (basiques) ou personnalisées (1 2 3 4 5)
4. Les agents Red Team attaquent → Blue Team défend
5. Les logs sont enregistrés dans agents_execution.log
```

## 📊 Configuration des cibles

**Fichier:** `.targets.conf`
**Format:** `NOM|ADRESSE|DESCRIPTION`

Cibles disponibles par défaut :
- localhost (127.0.0.1)
- gateway (192.168.1.1)
- webserver (192.168.1.100)
- database (192.168.1.50)
- workstation (192.168.1.25)
- nas (192.168.1.200)
- DNS (8.8.8.8)
- SSH (192.168.1.30)

**Ajouter une cible :**
```bash
echo "myserver|192.168.1.99|Mon serveur" >> .targets.conf
```

## 📊 Configuration des attaques

**Fichier:** `.attacks.conf`
**Format:** `NOM_ATTAQUE|OUTIL|DESCRIPTION`

Attaques disponibles :
1. Ping Test (nmap)
2. DNS Lookup (nmap)
3. Network Recon (nmap)
4. Banner Grab (nmap)
5. Asset Discovery (outils)
6. Port Scan (nmap)
7. Brute Force SSH (john)
8. Brute Force Web (hdra)
9. SQL Injection (hdra)
10. XSS Attack (hdra)
11. DDoS Simulation (outils)
12. Vulnerability Scan (nmap)
13. Malware Simulation (outils)
14. Privilege Escalation (hdra)
15. Data Exfiltration (outils)

**Ajouter une attaque :**
```bash
echo "My Attack|mytool|Description" >> .attacks.conf
```

## 🎮 Sélection des cibles

```
════════════════════════════════════════════════════
  SÉLECTION DES CIBLES
════════════════════════════════════════════════════

 1) localhost         127.0.0.1       Machine locale
 2) gateway           192.168.1.1     Passerelle réseau
 3) webserver         192.168.1.100   Serveur web
 4) database          192.168.1.50    Serveur base de données
 5) workstation       192.168.1.25    Poste de travail
 6) nas               192.168.1.200   Serveur NAS
 7) DNS               8.8.8.8         Serveur DNS externe
 8) SSH               192.168.1.30    Serveur SSH

Options:
  A) Sélectionner TOUS les cibles
  C) Personnalisé (entrez les numéros séparés par des espaces)
  Q) Quitter

Votre choix: 
```

## 🎮 Sélection des attaques

```
════════════════════════════════════════════════════
  SÉLECTION DES ATTAQUES
════════════════════════════════════════════════════

 1) Ping Test                 [nmap    ] Test de ping simple
 2) DNS Lookup                [nmap    ] Recherche DNS
 3) Network Recon             [nmap    ] Reconnaissance réseau
...
15) Data Exfiltration         [outils  ] Exfiltration de données

Options:
  A) Sélectionner TOUTES les attaques
  B) Attaques basiques (reconnaissance seulement)
  M) Attaques moyennes (mix reconnaissance/attaque)
  C) Personnalisé (entrez les numéros séparés par des espaces)
  Q) Quitter

Votre choix:
```

## 📜 Logs et historique

Les exécutions sont enregistrées dans `agents_execution.log` :

```bash
# Voir les 100 dernières lignes
tail -100 agents_execution.log

# Voir tous les logs
cat agents_execution.log

# Chercher une attaque spécifique
grep "SQL Injection" agents_execution.log

# Chercher une cible spécifique
grep "webserver" agents_execution.log
```

Format des logs :
```
=== RED RECON AGENT ===
Cible: webserver
Attaque: Port Scan
Date: Fri Apr 18 19:50:00 UTC 2026
---
[Sortie de l'agent]
---
```

## 🔧 Architecture du système

```
quick_start_manager.sh
├── init_config_files()
│   ├── .targets.conf (8 cibles)
│   └── .attacks.conf (15 attaques)
│
├── select_targets()
│   ├── Affiche le menu des cibles
│   ├── Propose 3 options (A/C/Q)
│   └── Remplit SELECTED_TARGETS[]
│
├── select_attacks()
│   ├── Affiche le menu des attaques
│   ├── Propose 4 options (A/B/M/C/Q)
│   └── Remplit SELECTED_ATTACKS[]
│
├── run_scenario()
│   ├── Pour chaque cible:
│   │   └── Pour chaque attaque:
│   │       ├── execute_red_recon_agent()
│   │       ├── execute_red_exploit_agent()
│   │       ├── execute_red_blackhat_agent()
│   │       ├── execute_blue_monitor_agent()
│   │       └── execute_blue_response_agent()
│   │
│   └── Enregistre tous les logs
│
└── show_main_menu()
    ├── Option 1: Scénario personnalisé
    ├── Option 2: Red Team uniquement
    ├── Option 3: Blue Team uniquement
    ├── Option 4: Voir les logs
    ├── Option 5: Configurer cibles
    ├── Option 6: Configurer attaques
    └── Option 0: Quitter
```

## 🧪 Scénarios de test

### Test 1 : Simple (1 cible, 1 attaque)
```
Sélectionner cible: 1 (localhost)
Sélectionner attaque: 1 (Ping Test)
Résultat: Les 5 agents s'exécutent sur localhost
```

### Test 2 : Reconnaissance (Multiple cibles, attaques basiques)
```
Sélectionner cibles: A (tous)
Sélectionner attaque: B (basique)
Résultat: 8 cibles × 5 attaques = 40 exécutions d'agents
```

### Test 3 : Attaque complète (2 cibles, toutes attaques)
```
Sélectionner cibles: 1 3 (localhost, webserver)
Sélectionner attaque: A (tous)
Résultat: 2 cibles × 15 attaques = 30 exécutions d'agents
```

## 📈 Améliorations par rapport à l'ancienne version

| Aspect | Avant | Après |
|--------|-------|-------|
| **GUI** | Fichiers multiples (6+) | 1 script bash unifié |
| **Sélection cibles** | Aucune | 8 cibles configurables |
| **Sélection attaques** | Aucune | 15 attaques configurables |
| **Flexibilité** | Limitée | Très flexible |
| **Documentation** | Incomplète | Complète |
| **Logs** | Dispersés | Centralisés |
| **Configuration** | Code dur | Fichiers `.conf` |

## 🚦 Statut du projet

✅ Système d'agents opérationnel
✅ Menu interactif complet
✅ Sélection des cibles et attaques
✅ Compilation et exécution automatiques
✅ Logging centralisé
✅ Documentation complète
✅ Configuration flexible
✅ Prêt pour utilisation en production (test/formation)

## 📖 Documentation complète

Pour plus de détails, consulter :
- [INTERACTIVE_MANAGER.md](./INTERACTIVE_MANAGER.md) - Guide complet
- [README.md](./README.md) - Vue d'ensemble
- [README_AGENT_MANAGER.md](./README_AGENT_MANAGER.md) - Gestionnaire C++

## ⚡ Démarrage rapide

```bash
# 1. Aller dans le dossier
cd /workspaces/application-linux

# 2. Lancer le gestionnaire
./quick_start_manager.sh

# 3. Suivre les menus pour :
#    - Sélectionner les cibles
#    - Sélectionner les attaques
#    - Lancer le scénario

# 4. Voir les logs
tail -f agents_execution.log
```

## 🔐 Notes importantes

⚠️ **À usage éducatif uniquement**
- Ne pas utiliser sur des systèmes en production
- Obtenir l'autorisation avant tout test
- Respecter les lois applicables

## 👤 Support

En cas de problème :
1. Vérifier que tous les agents sont compilés : `./compile_all_agents.sh`
2. Vérifier les permissions : `chmod +x *.sh`
3. Consulter les logs : `tail -100 agents_execution.log`
4. Lire la documentation : [INTERACTIVE_MANAGER.md](./INTERACTIVE_MANAGER.md)
