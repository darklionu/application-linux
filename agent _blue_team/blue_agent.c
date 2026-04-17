#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "blue_agent.h"

void blue_agent_init(BlueAgent *agent, const char *name) {
    strncpy(agent->name, name, BLUE_NAME_LEN - 1);
    agent->name[BLUE_NAME_LEN - 1] = '\0';
    agent->alert_level = 10;
    agent->readiness = 80;
}

void blue_agent_print_status(const BlueAgent *agent) {
    printf("\n╔═════════════════════════════════╗\n");
    printf("║ BLUE AGENT : %-18s║\n", agent->name);
    printf("╠═════════════════════════════════╣\n");
    printf("║ Niveau d'alerte : %-15d║\n", agent->alert_level);
    printf("║ Préparation      : %-15d║\n", agent->readiness);
    printf("╚═════════════════════════════════╝\n");
}

BlueResult blue_agent_action(BlueAgent *agent, const char *action_name, int success_rate) {
    printf("\n[>] Action : %s\n", action_name);
    int roll = rand() % 100;
    if (roll < success_rate) {
        printf("  [✓] Succès\n");
        agent->alert_level = agent->alert_level > 5 ? agent->alert_level - 5 : 0;
        agent->readiness = agent->readiness + 5;
        if (agent->readiness > 100) agent->readiness = 100;
        return BLUE_RESULT_STABLE;
    }
    if (roll < success_rate + 20) {
        printf("  [~] Succès partiel, alerte modérée\n");
        agent->alert_level += 10;
        agent->readiness = agent->readiness > 10 ? agent->readiness - 10 : 0;
        return BLUE_RESULT_ALERT;
    }
    printf("  [✗] Échec, intrusion non bloquée\n");
    agent->alert_level += 20;
    agent->readiness = agent->readiness > 20 ? agent->readiness - 20 : 0;
    return BLUE_RESULT_DETECTED;
}
