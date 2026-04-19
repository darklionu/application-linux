/**
 * Programme Principal: Système Intégré d'Agents Attaquants
 * Intègre: Gestion sécurisée + Attaque physique + Outils réels
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "agent_manager_secure.h"
#include "physical_target.h"

// Prototypes des fonctions
void setup_agents(SecureAgentManager* mgr);
void setup_attack_scenario(SecureAgentManager* mgr);
void interactive_menu(SecureAgentManager* mgr);
void execute_scenario(SecureAgentManager* mgr, int scenario);
void execute_full_attack_chain(SecureAgentManager* mgr);
void demonstrate_physical_attacks(SecureAgentManager* mgr);

// ============================================================================
// CONFIGURATION DES AGENTS
// ============================================================================

void setup_agents(SecureAgentManager* mgr) {
    printf("[*] Configuration des agents...\n");
    
    // Agents Red Team
    add_secure_agent(mgr, "Red Recon Agent", "./agent/red_recon_agent/red_recon_agent", "red_recon");
    add_secure_agent(mgr, "Red Exploit Agent", "./agent/red_exploit_agent/red_exploit_agent", "red_exploit");
    add_secure_agent(mgr, "Red BlackHat Agent", "./agent/red_blackhat_agent/red_blackhat_agent", "red_blackhat");
    
    printf("[+] Agents configurés avec succès\n");
}

// ============================================================================
// AFFICHAGE DU MENU PRINCIPAL
// ============================================================================

void display_main_menu(void) {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║   SYSTÈME D'AGENTS ATTAQUANTS - MENU PRINCIPAL      ║\n");
    printf("║          (Avec attaque sur objets physiques)          ║\n");
    printf("╠════════════════════════════════════════════════════════╣\n");
    printf("║                                                        ║\n");
    printf("║  [ATTAQUES]                                           ║\n");
    printf("║    1. Reconnaissance & Scan                           ║\n");
    printf("║    2. Exploitation de vulnérabilités                  ║\n");
    printf("║    3. Déploiement de malware                          ║\n");
    printf("║    4. Attaque DoS/DDoS                               ║\n");
    printf("║    5. Chaîne d'attaque complète                       ║\n");
    printf("║    6. Démonstration attaques physiques                ║\n");
    printf("║                                                        ║\n");
    printf("║  [GESTION]                                            ║\n");
    printf("║    7. Voir l'état des agents                          ║\n");
    printf("║    8. Voir l'état des cibles                          ║\n");
    printf("║    9. Voir l'historique des attaques                  ║\n");
    printf("║   10. Mode interactif                                 ║\n");
    printf("║                                                        ║\n");
    printf("║  [QUITTER]                                            ║\n");
    printf("║    0. Quitter le programme                            ║\n");
    printf("║                                                        ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    printf("Choix: ");
}

// ============================================================================
// SCÉNARIOS D'ATTAQUE
// ============================================================================

void execute_scenario(SecureAgentManager* mgr, int scenario) {
    if (!mgr || !mgr->attack_system) return;
    
    printf("\n[*] Exécution du scénario %d...\n\n", scenario);
    
    switch (scenario) {
        case 1: {  // Reconnaissance
            printf("[>] SCÉNARIO: Reconnaissance & Scan\n");
            printf("[*] L'agent de reconnaissance scanne les cibles\n\n");
            
            for (int i = 0; i < mgr->attack_system->targets_count && i < 2; i++) {
                PhysicalTarget* target = &mgr->attack_system->targets[i];
                scan_target(target);
            }
            
            printf("\n[+] Phase de reconnaissance complétée\n");
            break;
        }
            
        case 2: {  // Exploitation
            printf("[>] SCÉNARIO: Exploitation de vulnérabilités\n");
            printf("[*] Tentative d'exploitation des ports ouverts\n\n");
            
            for (int i = 0; i < mgr->attack_system->targets_count && i < 2; i++) {
                PhysicalTarget* target = &mgr->attack_system->targets[i];
                
                // D'abord scanner
                scan_target(target);
                
                // Puis exploiter
                if (target->ports_count > 0) {
                    printf("\n[*] Exploitation en cours sur %s...\n", target->name);
                    AttackResult res = execute_attack(mgr->attack_system, target, ATTACK_EXPLOIT, 70);
                    printf("[*] Résultat: %s\n", res.message);
                }
            }
            
            printf("\n[+] Phase d'exploitation complétée\n");
            break;
        }
            
        case 3: {  // Malware
            printf("[>] SCÉNARIO: Déploiement de malware\n");
            printf("[*] Déploiement de payloads malveillants\n\n");
            
            for (int i = 0; i < mgr->attack_system->targets_count && i < 2; i++) {
                PhysicalTarget* target = &mgr->attack_system->targets[i];
                printf("\n[*] Cible: %s (%s)\n", target->name, target->ip_address);
                
                AttackResult res = execute_attack(mgr->attack_system, target, ATTACK_MALWARE, 75);
                printf("[+] %s\n", res.message);
                print_target_info(target);
            }
            
            printf("\n[+] Phase de malware complétée\n");
            break;
        }
            
        case 4: {  // DoS
            printf("[>] SCÉNARIO: Attaque DoS/DDoS\n");
            printf("[*] Génération d'attaques de déni de service\n\n");
            
            for (int i = 0; i < mgr->attack_system->targets_count && i < 2; i++) {
                PhysicalTarget* target = &mgr->attack_system->targets[i];
                printf("\n[*] Attaque DoS sur: %s\n", target->name);
                
                AttackResult res = execute_attack(mgr->attack_system, target, ATTACK_DOS, 60);
                printf("[+] %s\n", res.message);
            }
            
            printf("\n[+] Phase DoS complétée\n");
            break;
        }
            
        default:
            printf("[!] Scénario invalide\n");
    }
}

void execute_full_attack_chain(SecureAgentManager* mgr) {
    if (!mgr) return;
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║    CHAÎNE D'ATTAQUE COMPLÈTE             ║\n");
    printf("║  Reconnaissance → Exploitation → Malware ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    // Phase 1: Reconnaissance
    printf("\n[PHASE 1] RECONNAISSANCE\n");
    printf("═══════════════════════════════════════════\n");
    execute_scenario(mgr, 1);
    sleep(2);
    
    // Phase 2: Exploitation
    printf("\n[PHASE 2] EXPLOITATION\n");
    printf("═══════════════════════════════════════════\n");
    execute_scenario(mgr, 2);
    sleep(2);
    
    // Phase 3: Déploiement malware
    printf("\n[PHASE 3] PERSISTANCE (Malware)\n");
    printf("═══════════════════════════════════════════\n");
    execute_scenario(mgr, 3);
    sleep(2);
    
    printf("\n[+] Chaîne d'attaque complète exécutée\n");
    print_system_status(mgr->attack_system);
}

void demonstrate_physical_attacks(SecureAgentManager* mgr) {
    if (!mgr || !mgr->attack_system) return;
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║    DÉMONSTRATION: ATTAQUES PHYSIQUES     ║\n");
    printf("║   Shutdown forcé d'objets physiques      ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    for (int i = 0; i < mgr->attack_system->targets_count && i < 2; i++) {
        PhysicalTarget* target = &mgr->attack_system->targets[i];
        
        printf("\n[*] Attaque sur cible physique: %s\n", target->name);
        print_target_info(target);
        
        // Attaque shutdown forcé
        printf("\n[>] Tentative d'arrêt forcé...\n");
        AttackResult res = execute_attack(mgr->attack_system, target, ATTACK_SHUTDOWN, 80);
        printf("[+] Résultat: %s\n", res.message);
        
        // Montrer l'état final
        print_target_info(target);
    }
    
    printf("\n[+] Démonstration complète\n");
    print_system_status(mgr->attack_system);
}

// ============================================================================
// MODE INTERACTIF
// ============================================================================

void interactive_menu(SecureAgentManager* mgr) {
    if (!mgr) return;
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║    MODE INTERACTIF                       ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    
    int running = 1;
    
    while (running) {
        printf("\n[MODE INTERACTIF]\n");
        printf("  1. Attaquer une cible spécifique\n");
        printf("  2. Scanner une cible\n");
        printf("  3. Voir les cibles disponibles\n");
        printf("  4. Voir un agent spécifique\n");
        printf("  0. Retour au menu principal\n");
        printf("Choix: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        getchar();  // Consommer le newline
        
        switch (choice) {
            case 1: {
                printf("\nCibles disponibles:\n");
                for (int i = 0; i < mgr->attack_system->targets_count; i++) {
                    printf("  %d. %s (%s)\n", i+1, 
                           mgr->attack_system->targets[i].name,
                           mgr->attack_system->targets[i].ip_address);
                }
                printf("Choisir une cible (numéro): ");
                int target_idx;
                scanf("%d", &target_idx);
                getchar();
                
                if (target_idx > 0 && target_idx <= mgr->attack_system->targets_count) {
                    PhysicalTarget* target = &mgr->attack_system->targets[target_idx-1];
                    printf("Type d'attaque:\n");
                    printf("  1. Scan\n  2. Exploit\n  3. Malware\n  4. DoS\n");
                    int attack;
                    scanf("%d", &attack);
                    getchar();
                    
                    AttackType types[] = {0, ATTACK_SCAN, ATTACK_EXPLOIT, ATTACK_MALWARE, ATTACK_DOS};
                    if (attack > 0 && attack < 5) {
                        AttackResult res = execute_attack(mgr->attack_system, target, types[attack], 70);
                        printf("[+] Attaque exécutée: %s\n", res.message);
                    }
                }
                break;
            }
                
            case 2: {
                printf("\nCibles disponibles:\n");
                for (int i = 0; i < mgr->attack_system->targets_count; i++) {
                    printf("  %d. %s (%s)\n", i+1, 
                           mgr->attack_system->targets[i].name,
                           mgr->attack_system->targets[i].ip_address);
                }
                printf("Choisir une cible (numéro): ");
                int target_idx;
                scanf("%d", &target_idx);
                getchar();
                
                if (target_idx > 0 && target_idx <= mgr->attack_system->targets_count) {
                    scan_target(&mgr->attack_system->targets[target_idx-1]);
                }
                break;
            }
                
            case 3:
                print_system_status(mgr->attack_system);
                for (int i = 0; i < mgr->attack_system->targets_count; i++) {
                    print_target_info(&mgr->attack_system->targets[i]);
                }
                break;
                
            case 4:
                print_manager_status(mgr);
                break;
                
            case 0:
                running = 0;
                break;
                
            default:
                printf("[!] Choix invalide\n");
        }
    }
}

// ============================================================================
// PROGRAMME PRINCIPAL
// ============================================================================

int main(int argc, char* argv[]) {
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║    SYSTÈME D'AGENTS ATTAQUANTS AMÉLIORÉ             ║\n");
    printf("║   Attaque d'objets physiques + Gestion sécurisée     ║\n");
    printf("║              VERSION 2.0 - BUGS FIXÉS                ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    // Initialisation
    SecureAgentManager* mgr = manager_init();
    if (!mgr) {
        fprintf(stderr, "[!] Erreur d'initialisation du gestionnaire\n");
        return 1;
    }
    
    // Ouvrir le fichier de log
    manager_open_log(mgr, "/tmp/agents_attack.log");
    log_message(mgr, "INFO", "Système d'agents attaquants démarré");
    
    // Configuration verbeux si demandé
    if (argc > 1 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0)) {
        mgr->verbose = 1;
    }
    
    // Configuration des agents
    setup_agents(mgr);
    
    // Menu principal
    int choice = 1;
    
    while (choice != 0) {
        display_main_menu();
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        
        getchar();  // Consommer le newline
        
        switch (choice) {
            case 1:
                execute_scenario(mgr, 1);
                break;
            case 2:
                execute_scenario(mgr, 2);
                break;
            case 3:
                execute_scenario(mgr, 3);
                break;
            case 4:
                execute_scenario(mgr, 4);
                break;
            case 5:
                execute_full_attack_chain(mgr);
                break;
            case 6:
                demonstrate_physical_attacks(mgr);
                break;
            case 7:
                print_manager_status(mgr);
                break;
            case 8:
                print_system_status(mgr->attack_system);
                for (int i = 0; i < mgr->attack_system->targets_count; i++) {
                    print_target_info(&mgr->attack_system->targets[i]);
                }
                break;
            case 9:
                print_attack_history(mgr->attack_system);
                break;
            case 10:
                interactive_menu(mgr);
                break;
            case 0:
                printf("\n[+] Arrêt du système...\n");
                break;
            default:
                printf("[!] Choix invalide\n");
        }
    }
    
    // Nettoyage
    log_message(mgr, "INFO", "Système d'agents attaquants arrêté");
    manager_close_log(mgr);
    manager_free(mgr);
    
    printf("\n[+] Fin du programme\n");
    return 0;
}
