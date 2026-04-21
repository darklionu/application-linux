/**
 * AGENT BLUE TEAM - Défense contre les attaques Red Team
 * Détection, prévention et réaction aux attaques
 */

#ifndef BLUE_TEAM_AGENT_H
#define BLUE_TEAM_AGENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>

#define MAX_DEFENSE_RULES 50
#define MAX_IDS_ALERTS 100
#define MAX_INCIDENTS 50

/**
 * Types de défense
 */
typedef enum {
    DEFENSE_FIREWALL,
    DEFENSE_IDS,
    DEFENSE_HONEYPOT,
    DEFENSE_CONTAINMENT,
    DEFENSE_RESPONSE
} DefenseType;

/**
 * États d'alerte
 */
typedef enum {
    ALERT_NONE = 0,
    ALERT_SUSPICIOUS = 1,
    ALERT_WARNING = 2,
    ALERT_CRITICAL = 3
} AlertLevel;

/**
 * Une règle de défense
 */
typedef struct {
    char name[128];
    DefenseType type;
    int enabled;
    int detection_rate;      // Probabilité de détection (0-100)
    int block_rate;          // Probabilité de bloquer (0-100)
    char description[256];
    time_t created_at;
} DefenseRule;

/**
 * Une alerte IDS
 */
typedef struct {
    AlertLevel level;
    char attack_type[64];
    char source_ip[32];
    char description[512];
    time_t detected_at;
    int is_blocked;
} IDSAlert;

/**
 * Un incident
 */
typedef struct {
    int incident_id;
    char description[512];
    AlertLevel severity;
    int response_time;      // en secondes
    int contained;
    time_t timestamp;
} Incident;

/**
 * Agent Blue Team
 */
typedef struct {
    char name[64];
    int active;
    int alert_level;
    DefenseRule rules[MAX_DEFENSE_RULES];
    int rules_count;
    IDSAlert alerts[MAX_IDS_ALERTS];
    int alerts_count;
    Incident incidents[MAX_INCIDENTS];
    int incidents_count;
    int total_threats_detected;
    int total_threats_blocked;
    FILE* log_file;
} BlueTeamAgent;

// Fonctions Blue Team
BlueTeamAgent* blue_team_init(const char* name);
void blue_team_free(BlueTeamAgent* agent);
void blue_team_add_defense(BlueTeamAgent* agent, const char* name, DefenseType type, int detection_rate);
int blue_team_detect_attack(BlueTeamAgent* agent, const char* attack_type, const char* source_ip, AlertLevel level);
int blue_team_block_attack(BlueTeamAgent* agent, const char* source_ip);
void blue_team_create_incident(BlueTeamAgent* agent, const char* description, AlertLevel severity);
void blue_team_print_status(BlueTeamAgent* agent);
void blue_team_print_alerts(BlueTeamAgent* agent);
void blue_team_respond_to_attack(BlueTeamAgent* agent, const char* attack_type);

#endif
