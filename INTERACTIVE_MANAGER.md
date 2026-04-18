# 🚀 GESTIONNAIRE D'AGENTS INTERACTIF

## Vue d'ensemble

Ce système permet de lancer les agents Red Team et Blue Team avec une sélection complète des cibles et attaques.

## 🎯 Fonctionnalités principales

- **Sélection des cibles** : Choisir une ou plusieurs cibles à attaquer
- **Sélection des attaques** : Choisir les techniques d'attaque parmi 15+ options
- **Exécution des agents** : Lancer les agents Red Team et Blue Team automatiquement
- **Gestion des logs** : Tous les événements sont enregistrés
- **Configuration flexible** : Modifier les cibles et attaques facilement

## 📋 Installation

### 1. Compiler tous les agents

```bash
cd /workspaces/application-linux
./compile_all_agents.sh
```

### 2. Rendre le script exécutable

```bash
chmod +x quick_start_manager.sh
```

## 🚀 Utilisation

### Lancement simple

```bash
cd /workspaces/application-linux
./quick_start_manager.sh
```

### Menu principal

```
════════════════════════════════════════════════
  GESTIONNAIRE D'AGENTS CENTRALISÉ
════════════════════════════════════════════════

Options:
  1) Lancer un scénario d'attaque personnalisé
  2) Lancer Red Team uniquement
  3) Lancer Blue Team uniquement
  4) Voir le dernier log
  5) Configurer les cibles
  6) Configurer les attaques
  0) Quitter
```

## 📊 Sélection des cibles

Les cibles disponibles par défaut :

| Numéro | Nom | Adresse | Description |
|--------|-----|---------|-------------|
| 1 | localhost | 127.0.0.1 | Machine locale |
| 2 | gateway | 192.168.1.1 | Passerelle réseau |
| 3 | webserver | 192.168.1.100 | Serveur web |
| 4 | database | 192.168.1.50 | Serveur base de données |
| 5 | workstation | 192.168.1.25 | Poste de travail |
| 6 | nas | 192.168.1.200 | Serveur NAS |
| 7 | DNS | 8.8.8.8 | Serveur DNS externe |
| 8 | SSH | 192.168.1.30 | Serveur SSH |

### Options de sélection :

- **A** : Sélectionner TOUS les cibles
- **C** : Sélection personnalisée (ex: `1 2 3`)
- **Q** : Quitter

## 🎮 Sélection des attaques

Les attaques disponibles :

| Numéro | Attaque | Outil | Description |
|--------|---------|-------|-------------|
| 1 | Ping Test | nmap | Test de ping simple |
| 2 | DNS Lookup | nmap | Recherche DNS |
| 3 | Network Recon | nmap | Reconnaissance réseau |
| 4 | Banner Grab | nmap | Récupération de bannière |
| 5 | Asset Discovery | outils | Découverte d'actifs |
| 6 | Port Scan | nmap | Scan de ports |
| 7 | Brute Force SSH | john | Force brute SSH |
| 8 | Brute Force Web | hdra | Force brute Web |
| 9 | SQL Injection | hdra | Injection SQL |
| 10 | XSS Attack | hdra | Attaque XSS |
| 11 | DDoS Simulation | outils | Simulation DDoS |
| 12 | Vulnerability Scan | nmap | Scan de vulnérabilités |
| 13 | Malware Simulation | outils | Simulation malware |
| 14 | Privilege Escalation | hdra | Escalade de privilèges |
| 15 | Data Exfiltration | outils | Exfiltration de données |

### Options de sélection :

- **A** : Sélectionner TOUTES les attaques
- **B** : Attaques basiques (reconnaissance uniquement)
- **M** : Attaques moyennes (mix reconnaissance/attaque)
- **C** : Sélection personnalisée (ex: `1 3 5`)
- **Q** : Quitter

## 🤖 Agents disponibles

### Red Team (Attaque)

1. **Red Recon Agent** - Reconnaissance réseau
2. **Red Exploit Agent** - Exploitation de vulnérabilités
3. **Red BlackHat Agent** - Attaques avancées

### Blue Team (Défense)

1. **Blue Monitor Agent** - Surveillance et détection
2. **Blue Response Agent** - Réponse aux incidents

## 📁 Configuration des cibles

Éditer le fichier `.targets.conf` :

```bash
# Dans le menu principal, choisir option 5
# Ou éditer directement :
nano .targets.conf
```

Format :
```
NOM_CIBLE|ADRESSE|DESCRIPTION
```

Exemple :
```
localhost|127.0.0.1|Machine locale
webserver|192.168.1.100|Serveur web
```

## 📁 Configuration des attaques

Éditer le fichier `.attacks.conf` :

```bash
# Dans le menu principal, choisir option 6
# Ou éditer directement :
nano .attacks.conf
```

Format :
```
NOM_ATTAQUE|OUTIL|DESCRIPTION
```

Exemple :
```
Ping Test|nmap|Test de ping simple
SQL Injection|hdra|Injection SQL
```

## 📊 Logs et historique

Les logs sont enregistrés dans `agents_execution.log` :

```bash
# Voir les derniers logs
tail -f agents_execution.log

# Voir les logs d'une date précise
grep "2024-04-18" agents_execution.log
```

## 🔧 Scénarios d'utilisation

### Scénario 1 : Test simple

```
1. Lancer un scénario d'attaque personnalisé
2. Sélectionner : 1 (localhost)
3. Sélectionner : 1 2 3 (Ping Test, DNS Lookup, Network Recon)
4. Attendre la fin de l'exécution
```

### Scénario 2 : Reconnaissance complète

```
1. Lancer un scénario d'attaque personnalisé
2. Sélectionner : A (tous les cibles)
3. Sélectionner : B (attaques basiques)
4. Attendre la fin de l'exécution
```

### Scénario 3 : Attaque complète

```
1. Lancer un scénario d'attaque personnalisé
2. Sélectionner : 1 3 6 (localhost, webserver, DNS)
3. Sélectionner : A (toutes les attaques)
4. Attendre la fin de l'exécution
```

## ⚙️ Personnalisation avancée

### Ajouter une nouvelle cible

```bash
echo "mynewhost|192.168.1.99|Mon serveur personnel" >> .targets.conf
```

### Ajouter une nouvelle attaque

```bash
echo "My Attack|mytools|Description de mon attaque" >> .attacks.conf
```

### Ajouter un nouvel outil

1. Placer l'outil dans le dossier `/workspaces/application-linux/tools/`
2. Référencer l'outil dans `.attacks.conf`

## 🐛 Dépannage

### Les agents ne se lancent pas

```bash
# Recompiler tous les agents
./compile_all_agents.sh

# Vérifier les permissions
chmod +x ./quick_start_manager.sh
chmod +x agent/red_recon_agent/red_recon_agent
chmod +x agent/red_exploit_agent/red_exploit_agent
chmod +x agent/red_blackhat_agent/red_blackhat_agent
```

### Erreur "tools" non trouvé

```bash
# Le script crée automatiquement les liens symboliques
# Si le problème persiste :
ln -sf /workspaces/application-linux/tools agent/red_recon_agent/tools
ln -sf /workspaces/application-linux/tools agent/red_exploit_agent/tools
ln -sf /workspaces/application-linux/tools agent/red_blackhat_agent/tools
```

### Pas de logs

```bash
# Vérifier les permissions d'écriture
ls -la agents_execution.log
chmod 666 agents_execution.log
```

## 📈 Améliorations futures

- [ ] Interface graphique web
- [ ] Sauvegarde des scénarios
- [ ] Analyse des logs en temps réel
- [ ] Statistiques des attaques
- [ ] Alertes personnalisées
- [ ] Intégration avec des outils externes

## 📞 Support

Pour plus d'informations, consulter :

- [README.md](./README.md) - Vue d'ensemble du projet
- [README_AGENT_MANAGER.md](./README_AGENT_MANAGER.md) - Gestionnaire d'agents C++
- [agent_red_team/README.md](./agent_red_team/README.md) - Red Team
- [gui/README.md](./gui/README.md) - Interface graphique

## 🔐 Notes de sécurité

⚠️ **Ce système est destiné à un usage éducatif et de test en environnement contrôlé uniquement.**

- Ne pas utiliser sur des systèmes en production
- Obtenir l'autorisation avant tout test d'attaque
- Respecter les lois et réglementations applicables

## 📄 Licence

Voir [LICENSE](../LICENSE) pour plus de détails.
