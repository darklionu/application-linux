#!/bin/bash
# 🚀 GUIDE DE DÉMARRAGE RAPIDE - Agent Manager

echo "╔════════════════════════════════════════════════════════╗"
echo "║            GUIDE DE DÉMARRAGE RAPIDE                  ║"
echo "╚════════════════════════════════════════════════════════╝"
echo ""

# Les commandes essentielles
cat << 'EOF'

📋 INSTALLATION ET COMPILATION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1. Se placer dans le répertoire:
   $ cd "agent_manager"

2. Compiler le projet:
   $ make                    # Compile uniquement
   $ make run                # Compile + lance
   $ make rebuild            # Nettoie + recompile

3. Exécuter:
   $ ./agent_manager      # Lancement manuel
   $ ./demo.sh           # Démonstration interactive


🎮 UTILISATION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

MENU PRINCIPAL:
  1 = Afficher le statut
  2 = Exécuter une technique
  3 = Gérer les outils (activer/désactiver)
  4 = Mode automatique (progression sans pause)
  5 = Voir toutes les techniques
  6 = Sauvegarder l'agent
  0 = Quitter


⚙️  GESTION DES OUTILS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Les outils disponibles sont dans: ../tools/
  • nmap    → Reconnaissance et Scan
  • john    → Cracking de mots de passe
  • hdra    → Attaques personnalisées

Comment activer un outil:
  1. Menu → 3 (Gérer les outils)
  2. Tapez le numéro de l'outil
  3. L'état change (activé ↔ désactivé)


🤖 MODE AUTOMATIQUE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Pour une progression autonome:
  1. Menu → 4 (Mode automatique)
  2. Entrez le nombre d'itérations (ex: 50)
  3. L'agent progresse automatiquement
  4. Affichage tous les 5 actions
  5. Déblocage auto des techniques


📊 COMPRENDRE LE STATUT
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Furtivité: 0-100% (baisse si détecté)
Outils:    [X] = activé, [ ] = désactivé


💡 CONSEILS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

1. Commencez par des techniques simples (niveau 1)
2. Activez les outils pour les techniques correspondantes
3. En cas d'échec, la furtivité baisse
4. Utilisez le mode auto pour progresser rapidement
5. Sauvegardez regулièrement (menu 6)


🔧 STRUCTURE DU PROJET
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

agent.h:        Structures et constantes
agent_core.c:   Logique de l'agent
tool_loader.c:  Gestion des techniques et outils
manager.c:      Interface utilisateur
Makefile:       Compilation

Le programme détecte automatiquement:
  • Les outils du dossier ../tools/
  • Les techniques disponibles
  • Le niveau de l'agent


📈 OBJECTIFS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

✓ Atteindre le niveau 20
✓ Débloquer toutes les techniques
✓ Activer tous les outils
✓ Maintenir une bonne furtivité
✓ Réussir toutes les techniques


❓ DÉPANNAGE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Q: Une technique est grise/indisponible?
R: Vérifiez que l'outil requis est activé dans le menu.

Q: Aucun outil détecté?
R: Vérifiez que le dossier ../tools/ existe.

Q: La compilation échoue?
R: Utilisez: make clean && make

Q: Comment modifier les techniques disponibles?
R: Changez les entrées dans tool_loader.c


🎯 EXEMPLE D'UTILISATION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

$ ./agent_manager
[Affiche le menu]
> 1          [Voir le statut actuel]
> 3          [Gérer les outils]
> 1          [Activer nmap]
> 2          [Activer john]
> 3          [Retour au menu]
> 2          [Exécuter une technique]
> 1          [Exécuter "Ping Test"]
[Succès!]
> 4          [Mode automatique]
> 50         [50 itérations]
[L'agent progresse...]
> 0          [Quitter]

EOF

echo ""
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
echo ""
read -p "Appuyez sur Entrée pour continuer..." dummy
