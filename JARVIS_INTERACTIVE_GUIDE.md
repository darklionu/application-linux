# 🤖 JARVIS INTERACTIVE - Guide Complet

## 🎉 Améliorations Apportées

### ✨ Interface Plus Interactive
- **Affichage en temps réel** des agents sélectionnés
- **Informations détaillées** pour chaque agent (type, chemin, priorité, dépendances)
- **Compteur de sélection** en direct
- **Aide contextuelle** intégrée

### 🔍 Autocomplétion Intelligente
- Suggestions de commandes basées sur votre saisie
- Suggestions d'agents disponibles
- Correction des erreurs avec aide

### 💡 Meilleure Ergonomie
- **Banneau JARVIS** au démarrage
- **Affichage structuré** des listes d'agents
- **Confirmations** avant les actions critiques
- **Historique** des commandes saisies

### 📊 Informations Enrichies
- Affichage des dépendances de chaque agent
- Priorités visibles
- État de chaque agent (actif/inactif)

---

## 🚀 Utilisation

### Démarrer JARVIS Interactive

```bash
cd /workspaces/application-linux/gui
make -f Makefile_interactive run
```

### Première Session

```
┌─ AGENTS DISPONIBLES ──────────────────────────────────────┐
│ [ ] * Red Recon Agent                (red_recon)
│ [ ] * Red Exploit Agent              (red_exploit)
│ [ ] * Red BlackHat Agent             (red_blackhat)
│ [ ] * Blue Monitor Agent             (blue_monitor)
│ [ ] * Blue Response Agent            (blue_response)
└───────────────────────────────────────────────────────────┘
Sélectionnés: 0/10
```

---

## 📖 Commandes Disponibles

### 1. **select <nom>** - Sélectionner un agent

Syntaxe avec support des noms multi-mots:
```
[JARVIS] > select Red Recon Agent
✓ Agent sélectionné: Red Recon Agent

┌─ INFO AGENT ───────────────────────────────────────────────┐
│ Nom:          Red Recon Agent
│ Type:         red_recon
│ Chemin:       ./agent/red_recon_agent/red_recon_agent
│ Priorité:     6
│ Dépendances:  (Aucune)
└───────────────────────────────────────────────────────────┘
```

**Avantages:**
- Affiche les informations complètes de l'agent
- Indique les dépendances
- Mise à jour immédiate du compteur

---

### 2. **deselect <nom>** - Désélectionner un agent

```
[JARVIS] > deselect Red Recon Agent
✓ Agent désélectionné: Red Recon Agent
```

---

### 3. **status** - Afficher l'état complet

```
[JARVIS] > status

┌─ AGENTS DISPONIBLES ──────────────────────────────────────┐
│ [X] * Red Recon Agent                (red_recon)
│ [X] * Red Exploit Agent              (red_exploit)
│ [ ] * Red BlackHat Agent             (red_blackhat)
│ [ ] * Blue Monitor Agent             (blue_monitor)
│ [ ] * Blue Response Agent            (blue_response)
└───────────────────────────────────────────────────────────┘
Sélectionnés: 2/10
```

---

### 4. **validate** - Vérifier les dépendances

```
[JARVIS] > validate
✓ Dépendances valides
```

Ou avec erreur:
```
[JARVIS] > validate
✗ Dépendances manquantes ou incompatibles
```

---

### 5. **execute** - Lancer l'exécution

```
[JARVIS] > execute

⚠ EXÉCUTION DES AGENTS SÉLECTIONNÉS:
   [1] Red Recon Agent (red_recon)
   [2] Red Exploit Agent (red_exploit)

Confirmez? (y/n): y

▶ Exécution...
   ✓ Red Recon Agent en cours...
   ✓ Red Exploit Agent en cours...

✓ Tous les agents ont été lancés
```

**Sécurité:**
- Affichage de la liste avant exécution
- Demande de confirmation
- Possibilité d'annuler

---

### 6. **clear** - Effacer la sélection

```
[JARVIS] > clear
✓ Sélection effacée
```

---

### 7. **swap <n1> <n2>** - Inverser l'ordre

```
[JARVIS] > swap 1 2
✓ Ordre échangé: 1 <-> 2
```

---

### 8. **snapshot** - Sauvegarder l'état

```
[JARVIS] > snapshot
✓ État sauvegardé
```

---

### 9. **help** - Afficher l'aide

```
[JARVIS] > help

┌─ COMMANDES DISPONIBLES ───────────────────────────────────┐
│                                                             │
│  select <nom>          Sélectionner un agent               │
│  deselect <nom>        Désélectionner un agent             │
│  clear                 Effacer toute la sélection          │
│  swap <n1> <n2>        Inverser l'ordre d'exécution        │
│  validate              Vérifier les dépendances            │
│  status                Afficher l'état détaillé            │
│  execute               Lancer l'exécution                  │
│  snapshot              Sauvegarder l'état actuel           │
│  help                  Afficher cette aide                 │
│  exit                  Quitter J.A.R.V.I.S                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

### 10. **exit** - Quitter

```
[JARVIS] > exit
✓ J.A.R.V.I.S fermé. Au revoir!
```

---

## 🔧 Exemple Complet de Session

```bash
$ ./jarvis_interactive

╔═══════════════════════════════════════════════════════════╗
║                                                           ║
║           J.A.R.V.I.S - INTERACTIVE AGENT CONTROL         ║
║         (Just A Rather Very Intelligent System)           ║
║                                                           ║
╚═══════════════════════════════════════════════════════════╝

┌─ AGENTS DISPONIBLES ──────────────────────────────────────┐
│ [ ] * Red Recon Agent                (red_recon)
│ [ ] * Red Exploit Agent              (red_exploit)
│ [ ] * Red BlackHat Agent             (red_blackhat)
│ [ ] * Blue Monitor Agent             (blue_monitor)
│ [ ] * Blue Response Agent            (blue_response)
└───────────────────────────────────────────────────────────┘
Sélectionnés: 0/10

┌─ COMMANDES DISPONIBLES ───────────────────────────────────┐
│                                                             │
│  select <nom>          Sélectionner un agent               │
│  ... [aide complète] ...                                  │
│  exit                  Quitter J.A.R.V.I.S                │
│                                                             │
└─────────────────────────────────────────────────────────────┘

[JARVIS] > select Red Recon Agent
✓ Agent 'Red Recon Agent' sélectionné (ordre: 1)
✓ Agent sélectionné: Red Recon Agent

┌─ INFO AGENT ───────────────────────────────────────────────┐
│ Nom:          Red Recon Agent
│ Type:         red_recon
│ Chemin:       ./agent/red_recon_agent/red_recon_agent
│ Priorité:     6
│ Dépendances:  (Aucune)
└───────────────────────────────────────────────────────────┘

[JARVIS] > select Red Exploit Agent
✓ Agent 'Red Exploit Agent' sélectionné (ordre: 2)
✓ Agent sélectionné: Red Exploit Agent

┌─ INFO AGENT ───────────────────────────────────────────────┐
│ Nom:          Red Exploit Agent
│ Type:         red_exploit
│ Chemin:       ./agent/red_exploit_agent/red_exploit_agent
│ Priorité:     7
│ Dépendances:  red_recon
└───────────────────────────────────────────────────────────┘

[JARVIS] > status

┌─ AGENTS DISPONIBLES ──────────────────────────────────────┐
│ [X] * Red Recon Agent                (red_recon)
│ [X] * Red Exploit Agent              (red_exploit)
│ [ ] * Red BlackHat Agent             (red_blackhat)
│ [ ] * Blue Monitor Agent             (blue_monitor)
│ [ ] * Blue Response Agent            (blue_response)
└───────────────────────────────────────────────────────────┘
Sélectionnés: 2/10

[JARVIS] > validate
✓ Dépendances valides

[JARVIS] > execute

⚠ EXÉCUTION DES AGENTS SÉLECTIONNÉS:
   [1] Red Recon Agent (red_recon)
   [2] Red Exploit Agent (red_exploit)

Confirmez? (y/n): y

▶ Exécution...
   ✓ Red Recon Agent en cours...
   ✓ Red Exploit Agent en cours...

✓ Tous les agents ont été lancés

[JARVIS] > exit
✓ J.A.R.V.I.S fermé. Au revoir!
```

---

## 📊 Agents Disponibles

### Red Team

| Agent | Type | Dépendances | Usage |
|-------|------|-------------|-------|
| **Red Recon Agent** | red_recon | Aucune | Reconnaissance initiale |
| **Red Exploit Agent** | red_exploit | red_recon | Exploitation après reconnaissance |
| **Red BlackHat Agent** | red_blackhat | red_exploit, red_recon | Attaques avancées |

### Blue Team

| Agent | Type | Dépendances | Usage |
|-------|------|-------------|-------|
| **Blue Monitor Agent** | blue_monitor | Aucune | Surveillance et monitoring |
| **Blue Response Agent** | blue_response | blue_monitor | Réponse aux incidents |

---

## 🎯 Scénarios Courants

### Scénario 1: Reconnaissance Complète

```
[JARVIS] > select Red Recon Agent
[JARVIS] > validate
[JARVIS] > execute
```

### Scénario 2: Exploitation Contrôlée

```
[JARVIS] > select Red Recon Agent
[JARVIS] > select Red Exploit Agent
[JARVIS] > validate
[JARVIS] > execute
```

### Scénario 3: Monitoring et Réponse

```
[JARVIS] > select Blue Monitor Agent
[JARVIS] > select Blue Response Agent
[JARVIS] > validate
[JARVIS] > execute
```

---

## ⚡ Tips & Tricks

1. **Utilisez `status`** avant `execute` pour vérifier la sélection
2. **Vérifiez toujours** les dépendances avec `validate`
3. **Les noms multi-mots** sont supportés (ex: "Red Recon Agent")
4. **Confirmez toujours** les exécutions critiques
5. **Utilisez `snapshot`** pour sauvegarder des configurations

---

## 🔄 Compilation et Build

```bash
# Mode production
make -f Makefile_interactive all

# Mode exécution
make -f Makefile_interactive run

# Mode debug
make -f Makefile_interactive debug

# Nettoyage
make -f Makefile_interactive clean
```

---

## ✅ Features

✓ Interface interactive avec affichage enrichi
✓ Autocomplétion intelligente
✓ Support des noms multi-mots
✓ Affichage détaillé des agents
✓ Validation des dépendances
✓ Confirmation avant exécution
✓ Historique des commandes
✓ Aide contextuelle intégrée
✓ Gestion propre des erreurs
✓ Production-ready

---

**Status:** ✅ Prêt à l'emploi
**Version:** 2.1 - Interactive Edition
