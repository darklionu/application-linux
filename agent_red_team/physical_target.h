/**
 * Système de ciblage et d'attaque sur objets physiques
 * Permet aux agents de cibler et d'attaquer des ressources physiques
 */

#ifndef PHYSICAL_TARGET_H
#define PHYSICAL_TARGET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "real_port_scanner.h"
#include "real_attack_systems.h"

#define MAX_TARGETS 50
#define TARGET_NAME_LEN 128
#define TARGET_IP_LEN 32
#define PORT_LEN 10
#define MAX_PORTS 20
#define MAX_ATTACKS 100

/**
 * États d'une cible physique
 */
typedef enum {
    TARGET_ONLINE,
    TARGET_OFFLINE,
    TARGET_COMPROMISED,
    TARGET_PROTECTED,
    TARGET_DESTROYED
} TargetState;

/**
 * Types d'attaque sur une cible physique
 */
typedef enum {
    ATTACK_SCAN,
    ATTACK_EXPLOIT,
    ATTACK_DOS,
    ATTACK_MALWARE,
    ATTACK_HARDWARE_DISABLE,
    ATTACK_SHUTDOWN,
    ATTACK_LOCK,
    ATTACK_PHYSICAL_DAMAGE
} AttackType;

/**
 * Résultat d'une attaque
 */
typedef struct {
    int success;
    int damage_level;  // 0-100: gravité de la compromission
    char message[512];
    int detection_risk;  // Risque d'être détecté
} AttackResult;

/**
 * Un port physique/services
 */
typedef struct {
    int port_number;
    char service[64];
    int is_open;
    int exploitable;
} Port;

/**
 * Une cible physique
 */
typedef struct {
    char name[TARGET_NAME_LEN];
    char ip_address[TARGET_IP_LEN];
    TargetState state;
    int is_connected;
    int integrity;              // 0-100: intégrité du système
    int access_level;           // 0-3: niveau d'accès obtenu
    Port ports[MAX_PORTS];
    int ports_count;
    int compromise_level;       // 0-100: niveau de compromission
    int has_backup;             // Possède une sauvegarde?
    int is_monitored;           // Sous surveillance?
    time_t compromise_time;     // Moment de la compromission
} PhysicalTarget;

/**
 * Historique d'une attaque
 */
typedef struct {
    char target_name[TARGET_NAME_LEN];
    AttackType attack_type;
    int success;
    time_t attack_time;
} AttackLog;

/**
 * Système d'attaque
 */
typedef struct {
    PhysicalTarget targets[MAX_TARGETS];
    int targets_count;
    AttackLog attack_history[MAX_ATTACKS];
    int attacks_count;
    int total_compromises;
    int total_damage;
} AttackSystem;

// ============================================================================
// Prototypes de fonctions
// ============================================================================

// Initialisation
AttackSystem* attack_system_init(void);
void attack_system_free(AttackSystem* sys);

// Gestion des cibles
void add_target(AttackSystem* sys, const char* name, const char* ip);
PhysicalTarget* find_target(AttackSystem* sys, const char* name);
void scan_target(PhysicalTarget* target);
void print_target_info(const PhysicalTarget* target);

// Attaques
AttackResult execute_attack(AttackSystem* sys, PhysicalTarget* target, AttackType type, int agent_skill);
int exploit_vulnerability(PhysicalTarget* target, int port, int agent_skill);
void simulate_dos_attack(PhysicalTarget* target);
void deploy_malware(PhysicalTarget* target, int severity);
void physical_shutdown(PhysicalTarget* target);

// Gestion des ports
void scan_ports(PhysicalTarget* target);
int find_open_port(PhysicalTarget* target);

// Logs et statistiques
void log_attack(AttackSystem* sys, const char* target, AttackType type, int success);
void print_attack_history(const AttackSystem* sys);
void print_system_status(const AttackSystem* sys);

// ============================================================================
// Implémentation
// ============================================================================

AttackSystem* attack_system_init(void) {
    AttackSystem* sys = (AttackSystem*)malloc(sizeof(AttackSystem));
    if (!sys) {
        perror("Erreur: impossible d'allouer la mémoire pour le système d'attaque");
        return NULL;
    }
    
    sys->targets_count = 0;
    sys->attacks_count = 0;
    sys->total_compromises = 0;
    sys->total_damage = 0;
    
    return sys;
}

void attack_system_free(AttackSystem* sys) {
    if (sys) {
        free(sys);
    }
}

void add_target(AttackSystem* sys, const char* name, const char* ip) {
    if (sys->targets_count >= MAX_TARGETS) {
        printf("[!] Limite de cibles atteinte\n");
        return;
    }
    
    PhysicalTarget* target = &sys->targets[sys->targets_count];
    strncpy(target->name, name, TARGET_NAME_LEN - 1);
    strncpy(target->ip_address, ip, TARGET_IP_LEN - 1);
    
    target->state = TARGET_ONLINE;
    target->is_connected = 1;
    target->integrity = 100;
    target->access_level = 0;
    target->ports_count = 0;
    target->compromise_level = 0;
    target->has_backup = (rand() % 100) > 50 ? 1 : 0;
    target->is_monitored = (rand() % 100) > 30 ? 1 : 0;
    target->compromise_time = 0;
    
    sys->targets_count++;
    printf("[+] Cible ajoutée: %s (%s)\n", name, ip);
}

PhysicalTarget* find_target(AttackSystem* sys, const char* name) {
    for (int i = 0; i < sys->targets_count; i++) {
        if (strcmp(sys->targets[i].name, name) == 0) {
            return &sys->targets[i];
        }
    }
    return NULL;
}

void scan_ports(PhysicalTarget* target) {
    if (!target) return;
    
    target->ports_count = 0;
    
    // SCAN RÉEL: Utiliser la vraie fonction de scan
    // Les ports sont maintenant scannés réellement via connexions TCP
    PortScanResult results[MAX_PORTS];
    int results_count = 0;
    
    // Ports préférés à scanner en priorité (pour plus de vitesse)
    int preferred_ports[] = {
        21, 22, 23, 25, 53, 80, 110, 143, 389, 443, 
        445, 465, 587, 993, 995, 1433, 3306, 3389, 5432, 
        5985, 8080, 8443, 8888, 27017, 6379, 9200, 0
    };
    
    printf("    [Scan RÉEL: Test des connexions TCP...]\n");
    
    for (int i = 0; preferred_ports[i] != 0 && target->ports_count < MAX_PORTS; i++) {
        int port = preferred_ports[i];
        int response_time = 0;
        
        // SCAN RÉEL du port
        int is_open = scan_single_port_tcp(target->ip_address, port, &response_time);
        
        if (is_open == 1) {  // Port réellement ouvert
            Port* p = &target->ports[target->ports_count];
            p->port_number = port;
            p->is_open = 1;
            p->exploitable = (rand() % 100) < 70 ? 1 : 0;  // Probabilité d'exploitation
            strncpy(p->service, get_service_name(port), sizeof(p->service) - 1);
            
            target->ports_count++;
        }
    }
}

void scan_target(PhysicalTarget* target) {
    if (!target) return;
    
    printf("\n[*] Scan de la cible: %s (%s)\n", target->name, target->ip_address);
    printf("[*] État: %s\n", target->state == TARGET_ONLINE ? "EN LIGNE" : "HORS LIGNE");
    printf("[*] Intégrité: %d%%\n", target->integrity);
    
    scan_ports(target);
    
    printf("[+] %d port(s) découvert(s):\n", target->ports_count);
    for (int i = 0; i < target->ports_count; i++) {
        printf("    - Port %d/%s [%s]%s\n", 
               target->ports[i].port_number,
               target->ports[i].service,
               target->ports[i].is_open ? "OUVERT" : "FERMÉ",
               target->ports[i].exploitable ? " [EXPLOITABLE]" : "");
    }
}

void print_target_info(const PhysicalTarget* target) {
    if (!target) return;
    
    printf("\n╔════════════════════════════════════════╗\n");
    printf("║  CIBLE PHYSIQUE: %-21s║\n", target->name);
    printf("╠════════════════════════════════════════╣\n");
    printf("║  IP:              %-22s║\n", target->ip_address);
    printf("║  État:            %-22s║\n", 
           target->state == TARGET_ONLINE ? "EN LIGNE" : 
           target->state == TARGET_OFFLINE ? "HORS LIGNE" :
           target->state == TARGET_COMPROMISED ? "COMPROMISE" :
           target->state == TARGET_DESTROYED ? "DÉTRUITE" : "PROTÉGÉE");
    printf("║  Intégrité:       %d%%                  ║\n", target->integrity);
    printf("║  Accès:           %d/3                 ║\n", target->access_level);
    printf("║  Compromission:   %d%%                 ║\n", target->compromise_level);
    printf("║  Monitoring:      %s               ║\n", target->is_monitored ? "OUI" : "NON");
    printf("║  Sauvegarde:      %s               ║\n", target->has_backup ? "OUI" : "NON");
    printf("╚════════════════════════════════════════╝\n");
}

int find_open_port(PhysicalTarget* target) {
    if (!target) return -1;
    
    for (int i = 0; i < target->ports_count; i++) {
        if (target->ports[i].is_open && target->ports[i].exploitable) {
            return i;
        }
    }
    return -1;
}

int exploit_vulnerability(PhysicalTarget* target, int port_idx, int agent_skill) {
    if (!target || port_idx < 0 || port_idx >= target->ports_count) {
        return 0;
    }
    
    Port* port = &target->ports[port_idx];
    
    // Chance d'exploitation basée sur la compétence de l'agent
    int success_chance = 40 + agent_skill + 10;  // 40-100%
    
    if (rand() % 100 < success_chance) {
        target->compromise_level = 50 + (rand() % 30);
        target->access_level = 1;
        target->compromise_time = time(NULL);
        
        printf("[+] Exploitation réussie du port %d (%s)\n", port->port_number, port->service);
        return 1;
    }
    
    printf("[!] Exploitation échouée - détection risquée\n");
    return 0;
}

void deploy_malware(PhysicalTarget* target, int severity) {
    if (!target) return;
    
    target->compromise_level = 70 + (severity * 3);
    if (target->compromise_level > 100) target->compromise_level = 100;
    
    target->access_level = 2;
    target->integrity -= severity;
    
    if (target->integrity < 0) target->integrity = 0;
    
    printf("[+] Malware déployé [Gravité: %d]\n", severity);
    printf("[+] Niveau de compromission: %d%%\n", target->compromise_level);
}

void simulate_dos_attack(PhysicalTarget* target) {
    if (!target) return;
    
    target->state = TARGET_OFFLINE;
    target->is_connected = 0;
    target->integrity -= 20;
    
    printf("[+] Attaque DoS exécutée - Cible hors ligne\n");
}

void physical_shutdown(PhysicalTarget* target) {
    if (!target) return;
    
    target->state = TARGET_DESTROYED;
    target->is_connected = 0;
    target->integrity = 0;
    target->access_level = 3;
    
    printf("[+] Arrêt forcé d'objets physiques - Cible DÉTRUITE\n");
}

AttackResult execute_attack(AttackSystem* sys, PhysicalTarget* target, AttackType type, int agent_skill) {
    AttackResult result = {0};
    
    if (!target) {
        strcpy(result.message, "Cible non trouvée");
        result.detection_risk = 0;
        return result;
    }
    
    printf("\n[>] Attaque sur %s: ", target->name);
    
    switch (type) {
        case ATTACK_SCAN: {
            printf("SCAN RÉEL\n");
            
            // VRAI SCAN - Utilise nmap ou scanner TCP custom
            char output_file[256];
            snprintf(output_file, sizeof(output_file), "/tmp/scan_%ld.txt", time(NULL));
            
            result.success = real_scan_attack(target->ip_address, output_file);
            scan_target(target);  // Aussi faire notre scan custom
            
            result.damage_level = 0;
            result.detection_risk = 15;
            snprintf(result.message, sizeof(result.message), 
                     "Scan réel réussi - %d port(s) découvert(s)", target->ports_count);
            break;
        }
            
        case ATTACK_EXPLOIT: {
            printf("EXPLOITATION RÉELLE\n");
            
            // VRAIE EXPLOITATION - Teste les vulnérabilités réelles
            int port_idx = find_open_port(target);
            if (port_idx >= 0) {
                Port* port = &target->ports[port_idx];
                result.success = real_exploit_attack(target->ip_address, 
                                                    port->port_number,
                                                    port->service);
                if (result.success) {
                    target->compromise_level = 60 + (agent_skill / 2);
                    target->access_level = 2;
                    result.damage_level = 40;
                    strcpy(result.message, "Exploitation réussie - Accès système obtenu");
                } else {
                    result.damage_level = 10;
                    strcpy(result.message, "Exploitation échouée - Credentials invalides");
                }
                result.detection_risk = 55;
            } else {
                result.success = 0;
                result.detection_risk = 20;
                strcpy(result.message, "Pas de port exploitable");
            }
            break;
        }
            
        case ATTACK_DOS: {
            printf("DOS/DDOS RÉEL\n");
            
            // VRAI DOS/DDOS - Lance les outils réels disponibles
            result.success = real_dos_attack(target->ip_address, 80, 10);
            simulate_dos_attack(target);
            
            result.damage_level = 70;
            result.detection_risk = 100;
            strcpy(result.message, "Attaque DoS/DDoS lancée - Cible dégrade");
            break;
        }
            
        case ATTACK_MALWARE: {
            printf("DÉPLOIEMENT RÉEL\n");
            
            // VRAI DÉPLOIEMENT - Génération de payload et simulation
            const char* malware_types[] = {"backdoor", "trojan", "worm", "ransomware"};
            int idx = rand() % 4;
            
            result.success = real_malware_deployment(target->ip_address, malware_types[idx]);
            deploy_malware(target, 80 + agent_skill);
            
            result.damage_level = 85;
            result.detection_risk = 70;
            snprintf(result.message, sizeof(result.message),
                     "Malware %s déployé - Persistence établie", malware_types[idx]);
            break;
        }
            
        case ATTACK_SHUTDOWN: {
            printf("ARRÊT FORCÉ\n");
            
            // VRAI SHUTDOWN - Persistence, escalade et shutdown
            real_persistence_and_escalation(target->ip_address);
            real_shell_access(target->ip_address, 4444);
            physical_shutdown(target);
            
            result.success = 1;
            result.damage_level = 100;
            result.detection_risk = 100;
            strcpy(result.message, "Cible détruite - Arrêt forcé réussi");
            break;
        }
            
        case ATTACK_HARDWARE_DISABLE: {
            printf("DÉSACTIVATION MATÉRIELLE\n");
            
            // Exfiltration de données RÉELLE
            real_data_exfiltration(target->ip_address, "credentials,database,files");
            
            target->state = TARGET_DESTROYED;
            target->is_connected = 0;
            target->integrity = 0;
            
            result.success = 1;
            result.damage_level = 100;
            result.detection_risk = 100;
            strcpy(result.message, "Matériel désactivé - Données exfiltrées");
            break;
        }
            
        case ATTACK_PHYSICAL_DAMAGE: {
            printf("DOMMAGES PHYSIQUES\n");
            
            // Rapport d'attaque complet
            const char* attacks[] = {"scan", "exploit", "malware", "dos", NULL};
            generate_attack_report(target->ip_address, (const char**)attacks);
            
            result.success = 1;
            result.damage_level = 100;
            result.detection_risk = 100;
            strcpy(result.message, "Dommages physiques complets");
            break;
        }
            
        default:
            result.success = 0;
            strcpy(result.message, "Type d'attaque inconnu");
    }
    
    if (result.success) {
        sys->total_damage += result.damage_level;
        sys->total_compromises++;
    }
    
    log_attack(sys, target->name, type, result.success);
    return result;
}

void log_attack(AttackSystem* sys, const char* target, AttackType type, int success) {
    if (sys->attacks_count >= MAX_ATTACKS) return;
    
    AttackLog* log = &sys->attack_history[sys->attacks_count];
    strncpy(log->target_name, target, TARGET_NAME_LEN - 1);
    log->attack_type = type;
    log->success = success;
    log->attack_time = time(NULL);
    
    sys->attacks_count++;
}

void print_attack_history(const AttackSystem* sys) {
    if (!sys) return;
    
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║         HISTORIQUE DES ATTAQUES (%d)              ║\n", sys->attacks_count);
    printf("╠═══════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < sys->attacks_count && i < 10; i++) {
        AttackLog* log = &sys->attack_history[i];
        printf("║ [%s] %s - %s                        ║\n",
               log->success ? "OK" : "KO",
               log->target_name,
               log->attack_type == ATTACK_SCAN ? "SCAN" :
               log->attack_type == ATTACK_EXPLOIT ? "EXPLOIT" :
               log->attack_type == ATTACK_DOS ? "DOS" :
               log->attack_type == ATTACK_MALWARE ? "MALWARE" :
               log->attack_type == ATTACK_SHUTDOWN ? "SHUTDOWN" : "AUTRE");
    }
    
    if (sys->attacks_count > 10) {
        printf("║ ... et %d autre(s)                              ║\n", sys->attacks_count - 10);
    }
    
    printf("╠═══════════════════════════════════════════════════╣\n");
    printf("║ Compromissions: %d | Dégâts totaux: %d%%           ║\n",
           sys->total_compromises, sys->total_damage / sys->attacks_count);
    printf("╚═══════════════════════════════════════════════════╝\n");
}

void print_system_status(const AttackSystem* sys) {
    if (!sys) return;
    
    printf("\n╔═════════════════════════════════════════╗\n");
    printf("║     ÉTAT DU SYSTÈME D'ATTAQUE          ║\n");
    printf("╠═════════════════════════════════════════╣\n");
    printf("║ Cibles: %d                            ║\n", sys->targets_count);
    printf("║ Attaques réussies: %d                  ║\n", sys->total_compromises);
    printf("║ Dégâts cumulés: %d%%                    ║\n", 
           sys->attacks_count > 0 ? sys->total_damage / sys->attacks_count : 0);
    printf("╚═════════════════════════════════════════╝\n");
}

#endif // PHYSICAL_TARGET_H
