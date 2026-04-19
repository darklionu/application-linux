# 🚀 Guide d'Utilisation JARVIS - Interface Multi-Agents

## 📌 Démarrage Rapide

### 1. Compilation

```bash
cd /workspaces/application-linux/gui

# Option A: Interface Interactive (Recommandée)
make -f Makefile_interactive all

# Option B: Interface Originale
make -f Makefile_jarvis all
```

### 2. Exécution

```bash
# Interface Interactive
./jarvis_interactive

# Interface Originale
./jarvis
```

## 🎮 Commandes Disponibles

| Commande | Syntaxe | Description |
|----------|---------|-------------|
| **select** | `select Agent de Reconnaissance Red` | Sélectionne un agent |
| **deselect** | `deselect Agent de Reconnaissance Red` | Désélectionne un agent |
| **clear** | `clear` | Efface toute la sélection |
| **swap** | `swap 1 2` | Inverse l'ordre des agents 1 et 2 |
| **validate** | `validate` | Vérifie que les dépendances sont valides |
| **status** | `status` | Affiche l'état actuel de la sélection |
| **execute** | `execute` | Lance l'exécution des agents sélectionnés |
| **snapshot** | `snapshot` | Sauvegarde l'état actuel de la sélection |
| **help** | `help` | Affiche l'aide complète |
| **exit** | `exit` ou `quit` | Quitte le système JARVIS |

## 👥 Agents Disponibles

### Équipe Red (Attaque)
1. **Agent de Reconnaissance Red** (red_recon)
   - Priorité: 6
   - Dépendances: Aucune
   - Chemin: `./agent/red_recon_agent/red_recon_agent`

2. **Agent d'Exploitation Red** (red_exploit)
   - Priorité: 7
   - Dépendances: Agent de Reconnaissance Red
   - Chemin: `./agent/red_exploit_agent/red_exploit_agent`

3. **Agent Red BlackHat** (red_blackhat)
   - Priorité: 9
   - Dépendances: Agent de Reconnaissance Red, Agent d'Exploitation Red
   - Chemin: `./agent/red_blackhat_agent/red_blackhat_agent`

### Équipe Blue (Défense)
1. **Agent de Surveillance Blue** (blue_monitor)
   - Priorité: 6
   - Dépendances: Aucune
   - Chemin: `./agent_blue_team/monitor_agent/blue_monitor_agent`

2. **Agent de Réponse Blue** (blue_response)
   - Priorité: 8
   - Dépendances: Agent de Surveillance Blue
   - Chemin: `./agent_blue_team/response_agent/blue_response_agent`

## 📋 Exemple d'Utilisation

```bash
$ ./jarvis_interactive

 ╔═══════════════════════════════════════════════════════════╗
 ║           J.A.R.V.I.S - INTERACTIVE AGENT CONTROL         ║
 ║         (Just A Rather Very Intelligent System)           ║
 ╚═══════════════════════════════════════════════════════════╝

 ┌─ AGENTS DISPONIBLES ──────────────────────────────────────┐
 │ [ ] * Agent de Reconnaissance Red    (red_recon)
 │ [ ] * Agent d'Exploitation Red       (red_exploit)
 │ [ ] * Agent Red BlackHat             (red_blackhat)
 │ [ ] * Agent de Surveillance Blue     (blue_monitor)
 │ [ ] * Agent de Réponse Blue         (blue_response)
 └───────────────────────────────────────────────────────────┘
 Sélectionnés: 0/10

 [JARVIS] > select Agent de Reconnaissance Red
 ✓ Agent 'Agent de Reconnaissance Red' sélectionné (ordre: 1)

 [JARVIS] > select Agent d'Exploitation Red
 ✓ Agent 'Agent d'Exploitation Red' sélectionné (ordre: 2)

 [JARVIS] > validate
 ✓ Dépendances valides

 [JARVIS] > status
 ┌────────────────────────────────────────────────────────────┐
 │ État de la Sélection d'Agents
 │ Agents sélectionnés: 2 / 10
 │ [1] Agent de Reconnaissance Red (red_recon)
 │ [2] Agent d'Exploitation Red (red_exploit)
 └────────────────────────────────────────────────────────────┘

 [JARVIS] > execute
 ⚠ Exécution des agents sélectionnés:
    [1] Agent de Reconnaissance Red (red_recon)
    [2] Agent d'Exploitation Red (red_exploit)
 
 Confirmez? (y/n): y
 
 ▶ Exécution...
    ✓ Agent de Reconnaissance Red en cours...
    ✓ Agent d'Exploitation Red en cours...
 
 ✓ Tous les agents ont été lancés

 [JARVIS] > exit
 ✓ J.A.R.V.I.S fermé. Au revoir!
```

## 🔧 Options de Compilation

### Mode Debug
```bash
make -f Makefile_interactive debug
# Lance gdb automatiquement
```

### Nettoyage
```bash
make -f Makefile_interactive clean
# Supprime les fichiers compilés
```

### Aide Make
```bash
make -f Makefile_interactive help
# Affiche toutes les cibles disponibles
```

## ⚙️ Configuration des Agents

### Ajouter un Agent

Modifiez `gui/src/main_interactive.cpp` ou `gui/src/main_jarvis.cpp`:

```cpp
system.addAvailableAgent(
    "Nom de l'Agent",           // Nom affiché
    "type_agent",               // Type interne
    "./chemin/vers/executable", // Chemin
    true                        // Activé
);
```

### Modifier les Dépendances

Éditez `gui/include/agent_selection_system.h`, méthode `addAvailableAgent()`:

```cpp
if (type == "type_agent") {
    agent.dependencies = {"dependance1", "dependance2"};
    agent.priority = 7;
}
```

## 📊 Système de Priorités

Les agents ont des priorités de 1 à 10:
- **5**: Priorité par défaut
- **6**: Agents de surveillance/reconnaissance
- **7-8**: Agents d'action
- **9**: Agents critiques (Red BlackHat)

## ✅ Vérification de Compilation

```bash
cd /workspaces/application-linux/gui
make -f Makefile_interactive all
```

Résultat attendu:
```
✓ Exécutable créé: jarvis_interactive
✓ Compilation réussie! Exécutez avec: make run
```

## 🆘 Dépannage

### "Commande inconnue"
- Vérifiez l'orthographe exacte de la commande
- Le système suggère des alternatives

### "Agent non trouvé"
- Utilisez le nom complet exact de l'agent
- Tapez `help` pour voir la liste des agents

### Erreur de dépendances
- Avant d'exécuter, tapez `validate`
- Les dépendances sont vérifiées automatiquement

## 📝 Notes

- Les identifiants internes (red_recon, blue_monitor) restent en anglais pour la compatibilité
- Les noms affichés sont entièrement en français
- Le système supporte jusqu'à 10 agents sélectionnés simultanément
- L'historique des commandes est disponible avec les flèches (interface interactive)

## 🎯 Cas d'Usage Typiques

### Scénario 1: Reconnaissance Red Team
```
select Agent de Reconnaissance Red
validate
execute
```

### Scénario 2: Chaîne Complète Red Team
```
select Agent de Reconnaissance Red
select Agent d'Exploitation Red
select Agent Red BlackHat
validate
execute
```

### Scénario 3: Monitoring Blue Team
```
select Agent de Surveillance Blue
select Agent de Réponse Blue
validate
execute
```

## 📞 Support

Pour plus d'informations, consultez:
- [TRADUCTION_FR_COMPLETE.md](TRADUCTION_FR_COMPLETE.md)
- [JARVIS_GUIDE.md](JARVIS_GUIDE.md)
- [JARVIS_INTERACTIVE_GUIDE.md](JARVIS_INTERACTIVE_GUIDE.md)

---

**J.A.R.V.I.S v2.0** - Interface Multi-Agents Entièrement en Français 🇫🇷
