# 🛡️ Bot Gestionnaire Agent - Documentation

## Présentation

Le **Bot Gestionnaire Agent** est un système d'évolution autonome et adaptative pour un agent red team. L'agent gagne de l'expérience, débloque de nouvelles techniques, et peut évoluer automatiquement selon les demandes de l'utilisateur.

## Fonctionnalités

### 1. **Système d'Expérience Adaptatif**
- L'XP requise pour monter de niveau s'adapte progressivement
- Bonus d'XP croissant avec le niveau de l'agent
- Bonifications par phase d'attaque

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
bot gestionnaire/
├── agent.h          # En-têtes et structures
├── agent.c          # Logique de l'agent
├── technique.c      # Gestion des techniques et outils
├── main.c           # Interface utilisateur et menu
├── Makefile         # Configuration de compilation
└── README.md        # Documentation
```

## Compilation

```bash
cd "bot gestionnaire"
make              # Compilation
make run          # Compilation et exécution
make rebuild      # Nettoyage + recompilation
```

## Utilisation

### Menu Principal

```
1. Afficher le statut      - Voir niveau, XP, furtivité, outils
2. Exécuter une technique  - Choisir et lancer une attaque
3. Gérer les outils        - Activer/désactiver les outils système
4. Mode automatique        - Évolution continue
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

### Niveaux et Expérience
- **Niveau 1-10** : Progression classique
- **Bonus adaptatif** : Plus de XP avec les niveaux élevés
- **Réduction progressive** : La difficulté s'adapte après le niveau 3

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
✅ **XP Dynamique** : Récompenses basées sur le niveau et la phase
✅ **Sauvegarde** : Persistance de la progression

## Développement Futur

- [ ] Système de missions spécifiques
- [ ] Réseau de cibles multiples
- [ ] Partageage d'expérience entre agents
- [ ] Apprentissage machine local
- [ ] Interface graphique
