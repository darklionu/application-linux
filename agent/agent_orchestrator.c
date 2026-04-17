#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void show_menu(void) {
    printf("\n=== AGENT ORCHESTRATOR ===\n");
    printf("1. Lancer l'agent red team Recon\n");
    printf("2. Lancer l'agent red team Exploit\n");
    printf("3. Lancer l'agent red team Black Hat\n");
    printf("4. Lancer l'agent blue team Monitoring\n");
    printf("5. Lancer l'agent blue team Response\n");
    printf("6. Lancer la simulation complète\n");
    printf("0. Quitter\n");
    printf("Choix: ");
}

static int run_command(const char *command) {
    printf("\n[+] Exécution: %s\n", command);
    int result = system(command);
    if (result == -1) {
        printf("[!] Impossible d'exécuter la commande\n");
        return 1;
    }
    return result != 0;
}

int main(void) {
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
                run_command("./red_recon_agent/red_recon_agent");
                break;
            case 2:
                run_command("./red_exploit_agent/red_exploit_agent");
                break;
            case 3:
                run_command("./red_blackhat_agent/red_blackhat_agent");
                break;
            case 4:
                run_command("./\"agent _blue_team\"/blue_monitor_agent");
                break;
            case 5:
                run_command("./\"agent _blue_team\"/blue_response_agent");
                break;
            case 6:
                run_command("./red_recon_agent/red_recon_agent");
                run_command("./red_exploit_agent/red_exploit_agent");
                run_command("./red_blackhat_agent/red_blackhat_agent");
                run_command("./\"agent _blue_team\"/blue_monitor_agent");
                run_command("./\"agent _blue_team\"/blue_response_agent");
                break;
            case 0:
                running = 0;
                break;
            default:
                printf("[!] Choix invalide\n");
                break;
        }
    }

    return 0;
}
