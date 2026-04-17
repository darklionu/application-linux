#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "agent_types.h"

extern Technique all_techniques[];
extern int techniques_count;

void create_blackhat_techniques(void) {
    techniques_count = 0;

    Technique blackhat[] = {
        { "Spear Phishing",       "outils",   PHASE_RECON,   70, 0 },
        { "Exploit Zero-Day",     "hdra",     PHASE_EXPLOIT, 45, 0 },
        { "Pass the Hash",        "john",     PHASE_LATERAL, 50, 0 },
        { "Privilege Escalation", "outils",   PHASE_LATERAL, 55, 0 },
        { "Backdoor Deployment",  "hdra",     PHASE_PERSIST, 40, 0 },
        { "Ransomware Spread",     "outils",   PHASE_PERSIST, 35, 0 }
    };

    for (int i = 0; i < (int)(sizeof(blackhat) / sizeof(blackhat[0])); i++) {
        all_techniques[i] = blackhat[i];
        techniques_count++;
    }
}

void show_menu(void) {
    printf("\n=== BLACK HAT AGENT ===\n");
    printf("1. Afficher le statut\n");
    printf("2. Exécuter une technique\n");
    printf("3. Mode automatique agressif\n");
    printf("0. Quitter\n");
    printf("Choix: ");
}

int main(void) {
    srand((unsigned)time(NULL));

    create_blackhat_techniques();

    Agent agent;
    agent_init(&agent, "BlackHat-Agent");
    load_tools(&agent, "../tools");

    for (int i = 0; i < agent.tools_count; i++) {
        if (strcmp(agent.tools[i].name, "john") == 0 ||
            strcmp(agent.tools[i].name, "hdra") == 0 ||
            strcmp(agent.tools[i].name, "outils") == 0) {
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
                printf("\n[+] Mode automatique agressif activé\n");
                for (int i = 0; i < 6; i++) {
                    Phase phase = (i % 2 == 0) ? PHASE_EXPLOIT : PHASE_LATERAL;
                    int tech = agent_auto_choose(&agent, phase);
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
