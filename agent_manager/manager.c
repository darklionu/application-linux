#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "agent.h"

extern Technique all_techniques[];
extern int techniques_count;

void show_welcome() {
    printf("\n");
    printf("╔═══════════════════════════════════════════╗\n");
    printf("║        🛡️  AGENT MANAGER 🛡️                ║\n");
    printf("║       Red Team Tool Controller           ║\n");
    printf("╚═══════════════════════════════════════════╝\n\n");
}

void show_main_menu() {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Afficher le statut\n");
    printf("2. Exécuter une technique\n");
    printf("3. Gérer les outils disponibles\n");
    printf("4. Mode évolution automatique\n");
    printf("5. Afficher toutes les techniques\n");
    printf("6. Sauvegarder l'agent\n");
    printf("0. Quitter\n");
    printf("Choix: ");
}

void select_and_manage_tools(Agent *a) {
    if (a->tools_count == 0) {
        printf("[!] Aucun outil détecté!\n");
        return;
    }

    printf("\n=== GESTION DES OUTILS ===\n");
    printf("Outils disponibles:\n");
    
    for (int i = 0; i < a->tools_count; i++) {
        printf("[%d] %s %-20s %s\n", 
               i + 1, 
               a->tools[i].enabled ? "[X]" : "[ ]",
               a->tools[i].name,
               a->tools[i].path);
    }

    printf("\nEntrez le numéro de l'outil à bascule (-1 pour quitter): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return;
    }

    if (choice >= 1 && choice <= a->tools_count) {
        a->tools[choice - 1].enabled = !a->tools[choice - 1].enabled;
        printf("[OK] Outil '%s' %s\n", 
               a->tools[choice - 1].name,
               a->tools[choice - 1].enabled ? "activé" : "désactivé");
    }
}

void execute_technique_menu(Agent *a) {
    printf("\n=== TECHNIQUES DISPONIBLES ===\n");
    
    for (int i = 0; i < a->techniques_count; i++) {
        printf("[%d] %s\n",
               i + 1,
               a->techniques[i].name);
    }

    printf("\nChoisissez une technique (-1 pour quitter): ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        return;
    }

    if (choice >= 1 && choice <= a->techniques_count) {
        agent_execute(a, choice - 1);
    } else if (choice != -1) {
        printf("[!] Choix invalide\n");
    }
}

void show_all_techniques() {
    printf("\n=== TOUTES LES TECHNIQUES ===\n");
    static const char *phase_names[] = {
        "Reconnaissance", "Scan", "Exploitation",
        "Mouvement latéral", "Persistance"
    };

    for (int i = 0; i < techniques_count; i++) {
        printf("[%d] %-30s | Phase: %-18s | Outil: %-10s | Succès: %d%%\n",
               i + 1,
               all_techniques[i].name,
               phase_names[all_techniques[i].phase],
               all_techniques[i].tool_name,
               all_techniques[i].success_rate);
    }
}

void save_agent(Agent *a) {
    char filename[128];
    snprintf(filename, sizeof(filename), "agent_%s.save", a->name);
    
    FILE *f = fopen(filename, "wb");
    if (!f) {
        printf("[!] Erreur lors de la création du fichier\n");
        return;
    }

    fwrite(a, sizeof(Agent), 1, f);
    fclose(f);
    printf("[OK] Agent sauvegardé dans '%s'\n", filename);
}

void auto_evolution_menu(Agent *a) {
    printf("\n=== MODE ÉVOLUTION AUTOMATIQUE ===\n");
    printf("Entrez le nombre d'itérations: ");
    
    int iterations;
    if (scanf("%d", &iterations) != 1 || iterations < 1) {
        while (getchar() != '\n');
        printf("[!] Nombre invalide\n");
        return;
    }

    a->auto_mode = 1;
    agent_auto_progress(a, iterations);
    a->auto_mode = 0;

    printf("\n[OK] Mode automatique terminé\n");
    agent_print_status(a);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    srand(time(NULL));
    show_welcome();

    // Créer et initialiser l'agent
    Agent agent;
    agent_init(&agent, "Agent-001");

    // Charger les outils
    printf("[*] Chargement des outils...\n");
    load_tools(&agent, "../tools");

    agent_print_status(&agent);

    int running = 1;
    while (running) {
        show_main_menu();
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                agent_print_status(&agent);
                break;

            case 2:
                execute_technique_menu(&agent);
                break;

            case 3:
                select_and_manage_tools(&agent);
                break;

            case 4:
                auto_evolution_menu(&agent);
                break;

            case 5:
                show_all_techniques();
                break;

            case 6:
                save_agent(&agent);
                break;

            case 0:
                printf("\n[*] Au revoir!\n");
                running = 0;
                break;

            default:
                printf("[!] Choix invalide\n");
        }
    }

    return 0;
}
