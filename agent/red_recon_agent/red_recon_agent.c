#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "agent_types.h"

extern Technique all_techniques[];
extern int techniques_count;

void create_recon_techniques(void) {
    techniques_count = 0;

    Technique recon[] = {
        { "Ping Test",          "nmap",    PHASE_RECON,   95, 0 },
        { "DNS Lookup",         "nmap",    PHASE_RECON,   90, 0 },
        { "Network Recon",      "nmap",    PHASE_RECON,   92, 0 },
        { "Banner Grab",        "nmap",    PHASE_SCAN,    85, 0 },
        { "Asset Discovery",    "outils",  PHASE_RECON,   80, 0 }
    };

    for (int i = 0; i < (int)(sizeof(recon) / sizeof(recon[0])); i++) {
        all_techniques[i] = recon[i];
        techniques_count++;
    }
}

void show_menu(void) {
    printf("\n=== RED TEAM RECON AGENT ===\n");
    printf("1. Afficher le statut\n");
    printf("2. Exécuter une technique\n");
    printf("3. Mode automatique (recon cycle)\n");
    printf("0. Quitter\n");
    printf("Choix: ");
}

int main(void) {
    srand((unsigned)time(NULL));

    create_recon_techniques();

    Agent agent;
    agent_init(&agent, "Recon-Agent");
    load_tools(&agent, "../tools");

    // Activer prioritairement les outils de reconnaissance
    for (int i = 0; i < agent.tools_count; i++) {
        if (strcmp(agent.tools[i].name, "nmap") == 0 || strcmp(agent.tools[i].name, "outils") == 0) {
            agent.tools[i].enabled = 1;
        }
    }

    agent_print_status(&agent);

    int running = 1;
    while (running) {
        show_menu();
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
                printf("\nTechnique à exécuter (1-%d): ", agent.techniques_count);
                if (scanf("%d", &choice) == 1 && choice >= 1 && choice <= agent.techniques_count) {
                    agent_execute(&agent, choice - 1);
                } else {
                    printf("[!] Choix invalide\n");
                }
                break;
            case 3:
                printf("\n[+] Mode automatique reconnaissance activé\n");
                for (int i = 0; i < 5; i++) {
                    int tech = agent_auto_choose(&agent, PHASE_RECON);
                    if (tech != -1) {
                        agent_execute(&agent, tech);
                    }
                }
                agent_print_status(&agent);
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("[!] Choix invalide\n");
        }
    }

    return 0;
}
