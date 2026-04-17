#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "agent_types.h"

extern Technique all_techniques[];
extern int       techniques_count;

static const char *phase_names[] = {
    "Reconnaissance", "Scan", "Exploitation",
    "Mouvement latéral", "Persistance"
};

// Initialise l'agent
void agent_init(Agent *a, const char *name) {
    strncpy(a->name, name, NAME_LEN - 1);
    a->stealth = 100;
    a->techniques_count = 0;
    a->tools_count = 0;
    a->auto_mode = 0;

    // Charger les techniques par défaut
    if (techniques_count == 0) {
        create_default_techniques();
    }

    // Débloquer toutes les techniques dès le départ
    for (int i = 0; i < techniques_count; i++) {
        all_techniques[i].unlocked = 1;
        if (a->techniques_count < MAX_TECHNIQUES) {
            a->techniques[a->techniques_count++] = all_techniques[i];
        }
    }
}

ActionResult agent_execute(Agent *a, int tech_index) {
    if (tech_index < 0 || tech_index >= a->techniques_count)
        return RESULT_FAILURE;

    Technique *t = &a->techniques[tech_index];

    if (!t->unlocked) {
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

    if (roll < t->success_rate) {
        printf("  [✓] Succès !\n");
        return RESULT_SUCCESS;
    } else if (roll < t->success_rate + 20) {
        printf("  [~] Succès partiel, furtivité compromise.\n");
        a->stealth -= 10;
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
    int best_rate = -1;

    for (int i = 0; i < a->techniques_count; i++) {
        Technique *t = &a->techniques[i];
        if (t->unlocked && t->phase == target_phase && t->success_rate > best_rate) {
            best = i;
            best_rate = t->success_rate;
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