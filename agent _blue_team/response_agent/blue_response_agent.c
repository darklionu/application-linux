#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blue_agent.h"

void show_menu(void) {
    printf("\n=== BLUE TEAM RESPONSE AGENT ===\n");
    printf("1. Afficher le statut\n");
    printf("2. Isoler un hôte compromis\n");
    printf("3. Appliquer un correctif\n");
    printf("4. Restaurer un service\n");
    printf("0. Quitter\n");
    printf("Choix: ");
}

int prompt_int(void) {
    int value;
    if (scanf("%d", &value) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    return value;
}

int main(void) {
    srand((unsigned)time(NULL));

    BlueAgent agent;
    blue_agent_init(&agent, "Response-Agent");
    blue_agent_print_status(&agent);

    int running = 1;
    while (running) {
        show_menu();
        int choice = prompt_int();

        switch (choice) {
            case 1:
                blue_agent_print_status(&agent);
                break;
            case 2:
                blue_agent_action(&agent, "Isolement hôte", 85);
                break;
            case 3:
                blue_agent_action(&agent, "Application correctif", 80);
                break;
            case 4:
                blue_agent_action(&agent, "Restauration service", 78);
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
