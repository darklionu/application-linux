# 🛡️ Agent Manager - Documentation

## Présentation

Le **Agent Manager** est un contrôleur d'outils pour une red team. Il détecte les outils disponibles, gère leur activation et exécute des techniques sans système d'expérience ni niveaux.

## Fonctionnalités

### 1. **Gestion d'Outils et Techniques**
- Détection automatique des outils dans le dossier `../tools/`
- Activation/désactivation des outils disponibles
- Exécution de techniques associées aux outils

### 2. **Gestion des Outils Système**
- Détection automatique des outils dans le dossier `../tools/`
- Activation/désactivation des outils disponibles
- Vérification de la disponibilité avant exécution

### 3. **Mode Automatique**
- L'agent progresse en boucle sans intervention
- Choix automatique des meilleures techniques
- Sauvegarde périodique de la progression

### 4. **Techniques Dynamiques**
- Les techniques sont associées aux outils disponibles
- Déblocage progressif selon le niveau
- Exécution avec taux de succès variable

## Structure des Fichiers

```
agent_manager/
├── agent.h          # En-têtes et structures
├── agent_core.c     # Logique de l'agent
├── tool_loader.c    # Gestion des techniques et outils
├── manager.c        # Interface utilisateur et menu
├── Makefile         # Configuration de compilation
└── README.md        # Documentation
```

## Compilation

```bash
cd "agent_manager"
make              # Compilation
make run          # Compilation et exécution
make rebuild      # Nettoyage + recompilation
```

## Utilisation

### Menu Principal

```
1. Afficher le statut      - Voir furtivité, outils et techniques
2. Exécuter une technique  - Choisir et lancer une attaque
3. Gérer les outils        - Activer/désactiver les outils système
4. Mode automatique        - Exécution automatique de techniques
5. Afficher techniques     - Liste complète des attaques disponibles
6. Sauvegarder l'agent     - Exporter la progression
0. Quitter
```

### Gestion des Outils

Les outils sont détectés automatiquement depuis le dossier `../tools/`:
- **nmap** : Reconnaissance et scan
- **john** : Cracking de mots de passe
- **hdra** : Attaques personnalisées

Pour activer/désactiver un outil :
1. Allez dans le menu "Gérer les outils"
2. Sélectionnez le numéro de l'outil
3. L'état change (activé ↔ désactivé)

### Mode Évolution Automatique

L'agent évolue seul sans interaction :
1. Choisissez le mode automatique
2. Entrez le nombre d'itérations
3. L'agent exécute des techniques en séquence
4. Il progresse et débloque automatiquement

## Système de Progression

### Progression
- Pas de système d'XP ni de niveaux
- Toutes les techniques sont disponibles immédiatement
- La réussite dépend de l'outil activé et du taux de succès de la technique

### Techniques Disponibles
- Reconnaissance (nmap)
- Scan (nmap)
- Exploitation (john, hdra)
- Mouvement latéral
- Persistance

### Furtivité
- Débute à 100%
- Baisse à chaque échec (-20%)
- Baisse moins en succès partiel (-10%)
- Augmente légèrement en succès complet

## Améliorations Apportées

✅ **Évolution Adaptative** : Difficulté qui change progressivement
✅ **Système d'Outils** : Intégration des outils système
✅ **Mode Automatique** : Progression sans intervention
✅ **Interface Améliorée** : Menus et affichages clairs
✅ **Gestion d'outils** : Activation et usage des outils détectés
✅ **Sauvegarde** : Persistance de la progression

## Développement Futur

- [ ] Système de missions spécifiques
- [ ] Réseau de cibles multiples
- [ ] Partage de techniques entre agents
- [ ] Apprentissage machine local
- [ ] Interface graphique
