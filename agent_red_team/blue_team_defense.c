/**
 * AGENT BLUE TEAM - Implémentation Complète
 * Défense contre les attaques Red Team avec détection et réaction
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include "blue_team_agent.h"

// Couleurs pour l'affichage
#define RED "\033[1;31m"
#define GRN "\033[1;32m"
#define YEL "\033[1;33m"
#define CYN "\033[1;36m"
#define WHT "\033[1;37m"
#define RST "\033[0m"

/**
 * Initialiser un agent Blue Team
 */
BlueTeamAgent* blue_team_init(const char* name) {
    BlueTeamAgent* agent = (BlueTeamAgent*)malloc(sizeof(BlueTeamAgent));
    if (!agent) return NULL;
    
    strncpy(agent->name, name, sizeof(agent->name) - 1);
    agent->name[sizeof(agent->name) - 1] = '\0';
    
    agent->active = 1;
    agent->alert_level = ALERT_NONE;
    agent->rules_count = 0;
    agent->alerts_count = 0;
    agent->incidents_count = 0;
    agent->total_threats_detected = 0;
    agent->total_threats_blocked = 0;
    
    // Créer un fichier log
    char log_file[256];
    snprintf(log_file, sizeof(log_file), "blue_team_%ld.log", time(NULL));
    agent->log_file = fopen(log_file, "w");
    if (agent->log_file) {
        fprintf(agent->log_file, "=== Blue Team Agent Log - %s ===\n", name);
        fprintf(agent->log_file, "Démarrage: %s\n\n", ctime(&(time_t){time(NULL)}));
    }
    
    // Ajouter les règles de défense par défaut
    blue_team_add_defense(agent, "Firewall Stateful", DEFENSE_FIREWALL, 85);
    blue_team_add_defense(agent, "IDS Network", DEFENSE_IDS, 80);
    blue_team_add_defense(agent, "Honeypot Low Interaction", DEFENSE_HONEYPOT, 60);
    blue_team_add_defense(agent, "Process Isolation", DEFENSE_CONTAINMENT, 75);
    blue_team_add_defense(agent, "Incident Response", DEFENSE_RESPONSE, 90);
    
    return agent;
}

/**
 * Libérer les ressources du Blue Team Agent
 */
void blue_team_free(BlueTeamAgent* agent) {
    if (!agent) return;
    if (agent->log_file) {
        fprintf(agent->log_file, "\nAgent fermé: %s\n", ctime(&(time_t){time(NULL)}));
        fclose(agent->log_file);
    }
    free(agent);
}

/**
 * Ajouter une règle de défense
 */
void blue_team_add_defense(BlueTeamAgent* agent, const char* name, DefenseType type, int detection_rate) {
    if (!agent || agent->rules_count >= MAX_DEFENSE_RULES) return;
    
    DefenseRule* rule = &agent->rules[agent->rules_count];
    strncpy(rule->name, name, sizeof(rule->name) - 1);
    rule->name[sizeof(rule->name) - 1] = '\0';
    
    rule->type = type;
    rule->enabled = 1;
    rule->detection_rate = detection_rate;
    rule->block_rate = detection_rate * 0.85;  // 85% des attaques détectées sont bloquées
    rule->created_at = time(NULL);
    
    const char* type_names[] = {
        "Firewall", "IDS", "Honeypot", "Containment", "Response"
    };
    snprintf(rule->description, sizeof(rule->description),
             "%s - Détection: %d%%, Blocage: %d%%",
             type_names[type], detection_rate, rule->block_rate);
    
    agent->rules_count++;
}

/**
 * Détecter une attaque
 */
int blue_team_detect_attack(BlueTeamAgent* agent, const char* attack_type, 
                            const char* source_ip, AlertLevel level) {
    if (!agent || agent->alerts_count >= MAX_IDS_ALERTS) return 0;
    
    // Simuler la détection basée sur les règles de défense
    int detected = 0;
    int blocked = 0;
    
    for (int i = 0; i < agent->rules_count; i++) {
        if (!agent->rules[i].enabled) continue;
        
        // Simuler la détection avec probabilité
        if ((rand() % 100) < agent->rules[i].detection_rate) {
            detected = 1;
            
            // Vérifier si l'attaque est bloquée
            if ((rand() % 100) < agent->rules[i].block_rate) {
                blocked = 1;
            }
            break;
        }
    }
    
    if (detected) {
        IDSAlert* alert = &agent->alerts[agent->alerts_count];
        alert->level = level;
        strncpy(alert->attack_type, attack_type, sizeof(alert->attack_type) - 1);
        alert->attack_type[sizeof(alert->attack_type) - 1] = '\0';
        strncpy(alert->source_ip, source_ip, sizeof(alert->source_ip) - 1);
        alert->source_ip[sizeof(alert->source_ip) - 1] = '\0';
        
        snprintf(alert->description, sizeof(alert->description),
                 "Attaque détectée: %s depuis %s [%s]",
                 attack_type, source_ip, blocked ? "BLOQUÉE" : "DÉTECTÉE");
        
        alert->detected_at = time(NULL);
        alert->is_blocked = blocked;
        
        agent->alerts_count++;
        agent->total_threats_detected++;
        
        if (blocked) {
            agent->total_threats_blocked++;
        }
        
        // Mettre à jour le niveau d'alerte
        if (level > agent->alert_level) {
            agent->alert_level = level;
        }
        
        // Logger l'alerte
        if (agent->log_file) {
            fprintf(agent->log_file, "[%s] %s - Source: %s - État: %s\n",
                    level == ALERT_CRITICAL ? "CRITIQUE" :
                    level == ALERT_WARNING ? "ATTENTION" : "SUSPECT",
                    attack_type, source_ip, blocked ? "BLOQUÉE" : "DÉTECTÉE");
        }
        
        return blocked ? 1 : -1;  // 1 = bloquée, -1 = détectée, 0 = non détectée
    }
    
    return 0;
}

/**
 * Créer un incident
 */
void blue_team_create_incident(BlueTeamAgent* agent, const char* description, AlertLevel severity) {
    if (!agent || agent->incidents_count >= MAX_INCIDENTS) return;
    
    Incident* incident = &agent->incidents[agent->incidents_count];
    incident->incident_id = agent->incidents_count + 1;
    strncpy(incident->description, description, sizeof(incident->description) - 1);
    incident->description[sizeof(incident->description) - 1] = '\0';
    incident->severity = severity;
    incident->response_time = rand() % 300 + 30;  // 30-330 secondes
    incident->contained = (rand() % 100) < 80 ? 1 : 0;  // 80% de chance de containment
    incident->timestamp = time(NULL);
    
    agent->incidents_count++;
    
    if (agent->log_file) {
        fprintf(agent->log_file, "\n[INCIDENT #%d]\n", incident->incident_id);
        fprintf(agent->log_file, "Description: %s\n", description);
        fprintf(agent->log_file, "Temps de réponse: %ds\n", incident->response_time);
        fprintf(agent->log_file, "Contenu: %s\n", incident->contained ? "OUI" : "NON");
    }
}

/**
 * Afficher le statut du Blue Team Agent
 */
void blue_team_print_status(const BlueTeamAgent* agent) {
    if (!agent) return;
    
    printf("\n");
    printf("╔═════════════════════════════════════════════════════════╗\n");
    printf("║        AGENT BLUE TEAM - STATUS                        ║\n");
    printf("╠═════════════════════════════════════════════════════════╣\n");
    printf("║ Nom: %-50s║\n", agent->name);
    printf("║ État: %s%-46s║\n", agent->active ? GRN "ACTIF" RST : RED "INACTIF" RST, "");
    printf("║ Niveau d'alerte: %d (", agent->alert_level);
    
    switch (agent->alert_level) {
        case ALERT_NONE: printf("AUCUN"); break;
        case ALERT_SUSPICIOUS: printf("SUSPECT"); break;
        case ALERT_WARNING: printf("ATTENTION"); break;
        case ALERT_CRITICAL: printf(RED "CRITIQUE" RST); break;
    }
    printf(")%-28s║\n", "");
    
    printf("╠═════════════════════════════════════════════════════════╣\n");
    printf("║ Règles de défense: %d                                   ║\n", agent->rules_count);
    for (int i = 0; i < agent->rules_count && i < 5; i++) {
        printf("║   [%d] %s (Détection: %d%%)  ║\n", 
               i+1, agent->rules[i].name, agent->rules[i].detection_rate);
    }
    
    printf("╠═════════════════════════════════════════════════════════╣\n");
    printf("║ Alertes détectées: %d                                   ║\n", agent->total_threats_detected);
    printf("║ Attaques bloquées: %d                                   ║\n", agent->total_threats_blocked);
    printf("║ Incidents: %d                                           ║\n", agent->incidents_count);
    printf("╚═════════════════════════════════════════════════════════╝\n");
}

/**
 * Afficher les alertes récentes
 */
void blue_team_print_alerts(const BlueTeamAgent* agent) {
    if (!agent || agent->alerts_count == 0) {
        printf("\nAucune alerte\n");
        return;
    }
    
    printf("\n" GRN "=== ALERTES IDS ===" RST "\n");
    for (int i = 0; i < agent->alerts_count && i < 10; i++) {
        const IDSAlert* alert = &agent->alerts[i];
        printf("[%d] %s - Source: %s - %s\n",
               i+1, alert->attack_type, alert->source_ip,
               alert->is_blocked ? GRN "BLOQUÉE" RST : YEL "DÉTECTÉE" RST);
    }
}

/**
 * Lancer un scan de sécurité
 */
int blue_team_security_scan(BlueTeamAgent* agent) {
    if (!agent) return 0;
    
    printf("\n" CYN "[*] Scan de sécurité en cours..." RST "\n");
    
    int vulnerabilities = 0;
    
    // Simuler différents types d'analyses
    const char* scan_types[] = {
        "Analyse des ports ouverts",
        "Vérification des services actifs",
        "Test de firewall",
        "Analyse des logs",
        "Vérification des permissions"
    };
    
    for (int i = 0; i < 5; i++) {
        printf("    [*] %s... ", scan_types[i]);
        sleep(1);
        
        int found = rand() % 3;
        if (found > 0) {
            printf(YEL "Anomalies détectées: %d" RST "\n", found);
            vulnerabilities += found;
        } else {
            printf(GRN "OK" RST "\n");
        }
    }
    
    printf("\n[+] Scan terminé - Total: %d anomalies\n", vulnerabilities);
    return vulnerabilities;
}

/**
 * Afficher les statistiques
 */
void blue_team_print_statistics(const BlueTeamAgent* agent) {
    if (!agent) return;
    
    printf("\n" GRN "=== STATISTIQUES ===" RST "\n");
    printf("Menaces détectées: %d\n", agent->total_threats_detected);
    printf("Menaces bloquées: %d\n", agent->total_threats_blocked);
    
    if (agent->total_threats_detected > 0) {
        int block_rate = (agent->total_threats_blocked * 100) / agent->total_threats_detected;
        printf("Taux de blocage: %d%%\n", block_rate);
    }
    
    printf("Incidents: %d\n", agent->incidents_count);
}
