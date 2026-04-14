#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "agent.h"

extern Technique all_techniques[];
extern int       techniques_count;

static const char *phase_names[] = {
    "Reconnaissance", "Scan", "Exploitation",
    "Mouvement latéral", "Persistance"
};

static int exp_required(int level) {
    return 40 * level * level;  // Expérience progressivement plus exigeante
}

// Calculer un bonus d'XP adaptatif
static int calculate_adaptive_xp(Agent *a, Technique *t) {
    int base_xp = t->xp_reward;
    int multiplier = 100 + (a->level * 5); // Bonus pour les niveaux élevés
    int phase_bonus = (t->phase % 3) * 10; // Bonus par phase
    return (base_xp * multiplier / 100) + phase_bonus;
}

// Initialise l'agent
void agent_init(Agent *a, const char *name) {
    strncpy(a->name, name, NAME_LEN - 1);
    a->level = 1;
    a->experience = 0;
    a->exp_to_next = exp_required(1);
    a->stealth = 100;
    a->techniques_count = 0;
    a->tools_count = 0;
    a->auto_mode = 0;

    // Charger les techniques par défaut
    if (techniques_count == 0) {
        create_default_techniques();
    }

    // Débloquer les techniques du niveau 1
    for (int i = 0; i < techniques_count; i++) {
        if (all_techniques[i].min_level <= 1) {
            all_techniques[i].unlocked = 1;
            a->techniques[a->techniques_count++] = all_techniques[i];
        }
    }
}

// Baisser l'XP requis pour les niveaux futurs (évolution adaptative)
static void adapt_difficulty(Agent *a) {
    if (a->experience >= a->exp_to_next * 0.5) {
        a->exp_to_next = (int)(a->exp_to_next * 0.9); // Réduire de 10%
    }
}

static void unlock_new_techniques(Agent *a) {
    for (int i = 0; i < techniques_count; i++) {
        if (all_techniques[i].min_level == a->level &&
            !all_techniques[i].unlocked) {
            all_techniques[i].unlocked = 1;
            if (a->techniques_count < MAX_TECHNIQUES) {
                a->techniques[a->techniques_count++] = all_techniques[i];
            }
            printf("  [+] Nouvelle technique : %s (%s)\n",
                   all_techniques[i].name,
                   phase_names[all_techniques[i].phase]);
        }
    }
}

static void gain_xp(Agent *a, int xp) {
    a->experience += xp;
    printf("  [XP] +%d XP (%d / %d)\n", xp, a->experience, a->exp_to_next);

    while (a->level < MAX_LEVEL && a->experience >= a->exp_to_next) {
        a->experience -= a->exp_to_next;
        a->level++;
        a->exp_to_next = exp_required(a->level);

        printf("\n  ⭐ LEVEL UP → Niveau %d ! ⭐\n", a->level);
        unlock_new_techniques(a);
        
        // Adaptation progressive
        if (a->level % 3 == 0 && a->level > 3) {
            printf("  [!] Difficultés réduites (adaptation automatique)\n");
            adapt_difficulty(a);
        }
    }
}

ActionResult agent_execute(Agent *a, int tech_index) {
    if (tech_index < 0 || tech_index >= a->techniques_count)
        return RESULT_FAILURE;

    Technique *t = &a->techniques[tech_index];

    if (!t->unlocked || t->min_level > a->level) {
        printf("  [!] Technique verrouillée : %s\n", t->name);
        return RESULT_FAILURE;
    }

    // Vérifier si l'outil est disponible
    int tool_available = 0;
    for (int i = 0; i < a->tools_count; i++) {
        if (a->tools[i].enabled && strcmp(a->tools[i].name, t->tool_name) == 0) {
            tool_available = 1;
            break;
        }
    }

    if (strlen(t->tool_name) > 0 && !tool_available) {
        printf("  [!] Outil '%s' non disponible pour %s\n", t->tool_name, t->name);
        return RESULT_FAILURE;
    }

    printf("\n[>] Exécution : %s (Phase: %s)\n",
           t->name, phase_names[t->phase]);
    
    if (strlen(t->tool_name) > 0) {
        printf("    Outil : %s\n", t->tool_name);
    }

    int roll = rand() % 100;
    int adaptive_xp = calculate_adaptive_xp(a, t);

    if (roll < t->success_rate) {
        printf("  [✓] Succès !\n");
        gain_xp(a, adaptive_xp);
        return RESULT_SUCCESS;
    } else if (roll < t->success_rate + 20) {
        printf("  [~] Succès partiel, furtivité compromise.\n");
        a->stealth -= 10;
        gain_xp(a, adaptive_xp / 2);
        return RESULT_PARTIAL;
    } else {
        printf("  [✗] Échec — détection !\n");
        a->stealth -= 20;
        if (a->stealth < 0) a->stealth = 0;
        return RESULT_FAILURE;
    }
}

int agent_auto_choose(Agent *a, Phase target_phase) {
    int best = -1;
    int best_xp = -1;

    for (int i = 0; i < a->techniques_count; i++) {
        Technique *t = &a->techniques[i];
        if (t->unlocked && t->phase == target_phase && t->xp_reward > best_xp) {
            best = i;
            best_xp = t->xp_reward;
        }
    }
    return best;
}

// Mode automatique : progresser en boucle
void agent_auto_progress(Agent *a, int iterations) {
    printf("\n[AUTO] Démarrage du mode évolution automatique (%d itérations)\n", iterations);
    
    for (int i = 0; i < iterations; i++) {
        Phase phases[] = {PHASE_RECON, PHASE_SCAN, PHASE_EXPLOIT, PHASE_LATERAL, PHASE_PERSIST};
        Phase phase = phases[i % 5];
        
        int tech = agent_auto_choose(a, phase);
        if (tech != -1) {
            agent_execute(a, tech);
        }
        
        if ((i + 1) % 5 == 0) {
            printf("\n[AUTO] Progression : %d/%d itérations\n", i + 1, iterations);
            agent_print_status(a);
        }
    }
}

void agent_print_status(const Agent *a) {
    printf("\n╔═══════════════════════════════════╗\n");
    printf("║  AGENT : %-23s║\n", a->name);
    printf("╠═══════════════════════════════════╣\n");
    printf("║  Niveau   : %-25d║\n", a->level);
    printf("║  XP       : %-4d / %-20d║\n", a->experience, a->exp_to_next);
    printf("║  Furtivité: %-25d║\n", a->stealth);
    printf("║  Mode    : %s%-22s║\n", a->auto_mode ? "AUTO" : "MANUAL", "");
    printf("╠═══════════════════════════════════╣\n");
    printf("║  TECHNIQUES DISPONIBLES (%d)        ║\n", a->techniques_count);
    for (int i = 0; i < a->techniques_count && i < 8; i++) {
        printf("║  [%d] %-29s║\n", i + 1, a->techniques[i].name);
    }
    if (a->techniques_count > 8) {
        printf("║  ... et %d autre(s)               ║\n", a->techniques_count - 8);
    }
    printf("╠═══════════════════════════════════╣\n");
    printf("║  OUTILS DISPONIBLES (%d)           ║\n", a->tools_count);
    for (int i = 0; i < a->tools_count && i < 5; i++) {
        printf("║  %s %-28s║\n", a->tools[i].enabled ? "[X]" : "[ ]", a->tools[i].name);
    }
    if (a->tools_count > 5) {
        printf("║  ... et %d autre(s)               ║\n", a->tools_count - 5);
    }
    printf("╚═══════════════════════════════════╝\n\n");
}