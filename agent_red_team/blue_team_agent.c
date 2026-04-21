/**
 * IMPLÉMENTATION AGENT BLUE TEAM
 */

#include "blue_team_agent.h"
#include <unistd.h>
#include <stdarg.h>

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
    
    // Ouvrir le fichier log
    agent->log_file = fopen("/tmp/blue_team.log", "a");
    if (agent->log_file) {
        fprintf(agent->log_file, "[%ld] Agent Blue Team initialise: %s\n", time(NULL), name);
        fflush(agent->log_file);
    }
    
    // Ajouter les défenses par défaut
    blue_team_add_defense(agent, "Firewall Stateless", DEFENSE_FIREWALL, 70);
    blue_team_add_defense(agent, "IDS Snort", DEFENSE_IDS, 80);
    blue_team_add_defense(agent, "Honeypot SSH", DEFENSE_HONEYPOT, 60);
    blue_team_add_defense(agent, "Containment Network", DEFENSE_CONTAINMENT, 85);
    blue_team_add_defense(agent, "Rapid Response Team", DEFENSE_RESPONSE, 90);
    
    printf("[+] Agent Blue Team créé: %s\n", name);
    return agent;
}

/**
 * Libérer un agent Blue Team
 */
void blue_team_free(BlueTeamAgent* agent) {
    if (!agent) return;
    
    if (agent->log_file) {
        fprintf(agent->log_file, "[%ld] Agent Blue Team terminé: %s\n", time(NULL), agent->name);
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
    rule->block_rate = detection_rate - 20;  // Toujours un peu moins que la détection
    if (rule->block_rate < 0) rule->block_rate = 0;
    
    snprintf(rule->description, sizeof(rule->description) - 1, "Defence %s (Type: %d)", name, type);
    rule->created_at = time(NULL);
    
    agent->rules_count++;
    
    if (agent->log_file) {
        fprintf(agent->log_file, "[+] Défense ajoutée: %s\n", name);
        fflush(agent->log_file);
    }
}

/**
 * Détecter une attaque
 */
int blue_team_detect_attack(BlueTeamAgent* agent, const char* attack_type, const char* source_ip, AlertLevel level) {
    if (!agent || agent->alerts_count >= MAX_IDS_ALERTS) return 0;
    
    int detected = 0;
    
    // Vérifier avec les règles de défense
    for (int i = 0; i < agent->rules_count; i++) {
        if (!agent->rules[i].enabled) continue;
        
        int roll = rand() % 100;
        if (roll < agent->rules[i].detection_rate) {
            detected = 1;
            break;
        }
    }
    
    if (detected) {
        IDSAlert* alert = &agent->alerts[agent->alerts_count];
        
        strncpy(alert->attack_type, attack_type, sizeof(alert->attack_type) - 1);
        alert->attack_type[sizeof(alert->attack_type) - 1] = '\0';
        
        strncpy(alert->source_ip, source_ip, sizeof(alert->source_ip) - 1);
        alert->source_ip[sizeof(alert->source_ip) - 1] = '\0';
        
        alert->level = level;
        alert->detected_at = time(NULL);
        alert->is_blocked = 0;
        
        snprintf(alert->description, sizeof(alert->description) - 1, 
                 "Attaque détectée: %s depuis %s", attack_type, source_ip);
        
        agent->alerts_count++;
        agent->total_threats_detected++;
        
        // Mettre à jour le niveau d'alerte
        if (level > agent->alert_level) {
            agent->alert_level = level;
        }
        
        if (agent->log_file) {
            fprintf(agent->log_file, "[ALERTE] %s depuis %s (Niveau: %d)\n", attack_type, source_ip, level);
            fflush(agent->log_file);
        }
        
        printf("[!] ALERTE DÉTECTÉE: %s depuis %s\n", attack_type, source_ip);
        return 1;
    }
    
    return 0;
}

/**
 * Bloquer une attaque
 */
int blue_team_block_attack(BlueTeamAgent* agent, const char* source_ip) {
    if (!agent) return 0;
    
    // Marquer les alertes comme bloquées
    for (int i = 0; i < agent->alerts_count; i++) {
        if (strcmp(agent->alerts[i].source_ip, source_ip) == 0 && !agent->alerts[i].is_blocked) {
            
            // Vérifier si on peut bloquer
            for (int j = 0; j < agent->rules_count; j++) {
                if (!agent->rules[j].enabled) continue;
                
                int roll = rand() % 100;
                if (roll < agent->rules[j].block_rate) {
                    agent->alerts[i].is_blocked = 1;
                    agent->total_threats_blocked++;
                    
                    printf("[+] ATTAQUE BLOQUÉE: %s\n", source_ip);
                    
                    if (agent->log_file) {
                        fprintf(agent->log_file, "[BLOQUÉ] Attaque depuis %s\n", source_ip);
                        fflush(agent->log_file);
                    }
                    
                    return 1;
                }
            }
        }
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
    incident->response_time = 0;
    incident->contained = 0;
    incident->timestamp = time(NULL);
    
    agent->incidents_count++;
    
    printf("[!] INCIDENT CRÉÉ: #%d - %s\n", incident->incident_id, description);
    
    if (agent->log_file) {
        fprintf(agent->log_file, "[INCIDENT] #%d - %s (Sévérité: %d)\n", 
                incident->incident_id, description, severity);
        fflush(agent->log_file);
    }
}

/**
 * Répondre à une attaque
 */
void blue_team_respond_to_attack(BlueTeamAgent* agent, const char* attack_type) {
    if (!agent) return;
    
    printf("\n[*] RÉPONSE AUX ATTAQUES\n");
    printf("════════════════════════════════════════\n");
    
    // Analyser le type d'attaque
    if (strstr(attack_type, "SCAN") || strstr(attack_type, "Scan")) {
        printf("[>] Attaque de reconnaissance détectée\n");
        printf("[*] Actions: Activation des logs avancés\n");
        printf("[*] Actions: Modification des banners\n");
        printf("[+] Fausse information envoyée à l'attaquant\n");
    }
    else if (strstr(attack_type, "EXPLOIT") || strstr(attack_type, "Exploit")) {
        printf("[>] Tentative d'exploitation détectée\n");
        printf("[*] Actions: Isolation du segment réseau\n");
        printf("[*] Actions: Collecte de preuves\n");
        printf("[+] Processus suspendu\n");
    }
    else if (strstr(attack_type, "MALWARE") || strstr(attack_type, "Malware")) {
        printf("[>] Déploiement de malware détecté\n");
        printf("[*] Actions: Containment immédiat\n");
        printf("[*] Actions: Snapshot du système\n");
        printf("[+] Communication malveillante bloquée\n");
    }
    else if (strstr(attack_type, "DOS") || strstr(attack_type, "DoS")) {
        printf("[>] Attaque DoS/DDoS détectée\n");
        printf("[*] Actions: Activation du rate limiting\n");
        printf("[*] Actions: Redirection vers honeypot\n");
        printf("[+] Trafic absorbé\n");
    }
    else {
        printf("[>] Attaque inconnue détectée\n");
        printf("[*] Actions: Mode défense maximum\n");
        printf("[+] Système en alert rouge\n");
    }
    
    printf("\n");
}

/**
 * Afficher le statut de l'agent Blue Team
 */
void blue_team_print_status(BlueTeamAgent* agent) {
    if (!agent) return;
    
    printf("\n╔═══════════════════════════════════════════════╗\n");
    printf("║        AGENT BLUE TEAM - STATUT             ║\n");
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║ Nom: %-39s║\n", agent->name);
    printf("║ État: %-38s║\n", agent->active ? "ACTIF" : "INACTIF");
    printf("║ Niveau d'alerte: %d (", agent->alert_level);
    
    switch (agent->alert_level) {
        case ALERT_NONE: printf("Aucun)"); break;
        case ALERT_SUSPICIOUS: printf("Suspect)"); break;
        case ALERT_WARNING: printf("Avertissement)"); break;
        case ALERT_CRITICAL: printf("Critique)"); break;
    }
    printf("%-10s║\n", "");
    
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║ Règles de défense: %d                        ║\n", agent->rules_count);
    for (int i = 0; i < agent->rules_count && i < 5; i++) {
        printf("║   [%d] %s (Détection: %d%%)  ║\n", 
               i+1, agent->rules[i].name, agent->rules[i].detection_rate);
    }
    
    printf("╠═══════════════════════════════════════════════╣\n");
    printf("║ STATISTIQUES:                               ║\n");
    printf("║   Menaces détectées: %d                      ║\n", agent->total_threats_detected);
    printf("║   Menaces bloquées: %d                       ║\n", agent->total_threats_blocked);
    printf("║   Incidents créés: %d                        ║\n", agent->incidents_count);
    printf("║   Alertes actives: %d                        ║\n", agent->alerts_count);
    printf("╚═══════════════════════════════════════════════╝\n\n");
}

/**
 * Afficher les alertes
 */
void blue_team_print_alerts(BlueTeamAgent* agent) {
    if (!agent || agent->alerts_count == 0) {
        printf("[*] Aucune alerte\n");
        return;
    }
    
    printf("\n╔═════════════════════════════════════════════════╗\n");
    printf("║           ALERTES IDS (%d)                    ║\n", agent->alerts_count);
    printf("╠═════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < agent->alerts_count && i < 10; i++) {
        IDSAlert* alert = &agent->alerts[i];
        printf("║ [%d] %s (IP: %s)\n", i+1, alert->attack_type, alert->source_ip);
        printf("║     Statut: %s\n", alert->is_blocked ? "BLOQUÉ" : "DÉTECTÉ");
    }
    
    printf("╚═════════════════════════════════════════════════╝\n\n");
}
