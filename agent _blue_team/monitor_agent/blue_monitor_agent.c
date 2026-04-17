#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blue_agent.h"

void show_menu(void) {
    printf("\n=== BLUE TEAM MONITORING AGENT ===\n");
    printf("1. Afficher le statut\n");
    printf("2. Examiner les journaux\n");
    printf("3. Surveiller le réseau\n");
    printf("4. Analyse comportementale\n");
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
    blue_agent_init(&agent, "Monitor-Agent");
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
                blue_agent_action(&agent, "Examen des journaux", 75);
                break;
            case 3:
                blue_agent_action(&agent, "Surveillance réseau", 80);
                break;
            case 4:
                blue_agent_action(&agent, "Analyse comportementale", 70);
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
