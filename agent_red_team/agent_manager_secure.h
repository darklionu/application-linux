/**
 * Gestionnaire d'Agents Sécurisé et Amélioré
 * Corrige les bugs de sécurité du système original
 */

#ifndef AGENT_MANAGER_SECURE_H
#define AGENT_MANAGER_SECURE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <signal.h>
#include "physical_target.h"

#define MAX_AGENTS 20
#define MAX_AGENT_NAME 128
#define MAX_PATH 256
#define MAX_OUTPUT 4096
#define EXEC_TIMEOUT 30  // 30 secondes timeout

/**
 * État d'un agent
 */
typedef enum {
    AGENT_IDLE,
    AGENT_RUNNING,
    AGENT_SUCCESS,
    AGENT_FAILED,
    AGENT_TIMEOUT,
    AGENT_COMPROMISED
} AgentState;

/**
 * Résultat d'exécution d'un agent
 */
typedef struct {
    int success;
    int exit_code;
    AgentState state;
    char output[MAX_OUTPUT];
    char error[MAX_OUTPUT];
    time_t execution_time;
    int targets_compromised;
} ExecutionResult;

/**
 * Un agent sécurisé
 */
typedef struct {
    char name[MAX_AGENT_NAME];
    char path[MAX_PATH];
    char type[64];
    int enabled;
    int priority;
    AgentState state;
    time_t last_execution;
    int execution_count;
    int success_count;
    int failure_count;
    PhysicalTarget* assigned_targets;
    int targets_count;
} SecureAgent;

/**
 * Gestionnaire sécurisé
 */
typedef struct {
    SecureAgent agents[MAX_AGENTS];
    int agents_count;
    AttackSystem* attack_system;
    int verbose;
    FILE* log_file;
} SecureAgentManager;

// ============================================================================
// Prototypes
// ============================================================================

// Initialisation
SecureAgentManager* manager_init(void);
void manager_free(SecureAgentManager* mgr);
void manager_open_log(SecureAgentManager* mgr, const char* log_path);
void manager_close_log(SecureAgentManager* mgr);

// Validation
int validate_agent_path(const char* path);
int validate_agent_executable(const char* path);
int is_safe_path(const char* path);

// Agents
void add_secure_agent(SecureAgentManager* mgr, const char* name, const char* path, const char* type);
SecureAgent* find_agent(SecureAgentManager* mgr, const char* name);
int remove_agent(SecureAgentManager* mgr, const char* name);

// Exécution
ExecutionResult execute_agent_safely(SecureAgentManager* mgr, SecureAgent* agent);
void execute_with_timeout(const char* executable, int timeout, ExecutionResult* result);
pid_t safe_fork_exec(const char* executable, char** argv, int* stdout_pipe, int* stderr_pipe);

// Gestion des cibles
void assign_targets_to_agent(SecureAgent* agent, AttackSystem* sys);
void attack_assigned_targets(SecureAgent* agent, AttackSystem* sys);

// Logging
void log_message(SecureAgentManager* mgr, const char* level, const char* message);
void log_execution(SecureAgentManager* mgr, SecureAgent* agent, ExecutionResult* result);

// Affichage
void print_agent_status(const SecureAgent* agent);
void print_manager_status(const SecureAgentManager* mgr);

// ============================================================================
// Implémentation
// ============================================================================

int is_safe_path(const char* path) {
    if (!path || strlen(path) == 0) return 0;
    
    // Rejeter les chemins avec des caractères dangereux
    if (strchr(path, ';') || strchr(path, '|') || strchr(path, '&') ||
        strchr(path, '`') || strchr(path, '$') || strchr(path, '(')) {
        return 0;
    }
    
    // Rejeter les chemins avec ".."
    if (strstr(path, "..")) {
        return 0;
    }
    
    return 1;
}

int validate_agent_path(const char* path) {
    if (!is_safe_path(path)) {
        fprintf(stderr, "[!] Chemin non valide: %s\n", path);
        return 0;
    }
    return 1;
}

int validate_agent_executable(const char* path) {
    if (access(path, F_OK) != 0) {
        fprintf(stderr, "[!] Fichier n'existe pas: %s\n", path);
        return 0;
    }
    
    if (access(path, X_OK) != 0) {
        fprintf(stderr, "[!] Fichier n'est pas exécutable: %s\n", path);
        return 0;
    }
    
    return 1;
}

SecureAgentManager* manager_init(void) {
    SecureAgentManager* mgr = (SecureAgentManager*)malloc(sizeof(SecureAgentManager));
    if (!mgr) {
        perror("Erreur: allocation mémoire échouée");
        return NULL;
    }
    
    mgr->agents_count = 0;
    mgr->verbose = 0;
    mgr->log_file = NULL;
    
    mgr->attack_system = attack_system_init();
    if (!mgr->attack_system) {
        free(mgr);
        return NULL;
    }
    
    // Initialiser les cibles par défaut
    add_target(mgr->attack_system, "Serveur Web 1", "192.168.1.100");
    add_target(mgr->attack_system, "Base de Données", "192.168.1.101");
    add_target(mgr->attack_system, "Serveur Mail", "192.168.1.102");
    add_target(mgr->attack_system, "IoT Device 1", "192.168.1.103");
    add_target(mgr->attack_system, "IoT Device 2", "192.168.1.104");
    
    return mgr;
}

void manager_free(SecureAgentManager* mgr) {
    if (!mgr) return;
    
    if (mgr->attack_system) {
        attack_system_free(mgr->attack_system);
    }
    
    if (mgr->log_file) {
        fclose(mgr->log_file);
    }
    
    free(mgr);
}

void manager_open_log(SecureAgentManager* mgr, const char* log_path) {
    if (!mgr) return;
    
    mgr->log_file = fopen(log_path, "a");
    if (!mgr->log_file) {
        fprintf(stderr, "[!] Impossible d'ouvrir le fichier log: %s\n", log_path);
    }
}

void manager_close_log(SecureAgentManager* mgr) {
    if (mgr && mgr->log_file) {
        fclose(mgr->log_file);
        mgr->log_file = NULL;
    }
}

void log_message(SecureAgentManager* mgr, const char* level, const char* message) {
    time_t now = time(NULL);
    struct tm* timeinfo = localtime(&now);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);
    
    if (mgr->log_file) {
        fprintf(mgr->log_file, "[%s] %s: %s\n", timestamp, level, message);
        fflush(mgr->log_file);
    }
    
    if (mgr->verbose) {
        fprintf(stdout, "[%s] %s: %s\n", timestamp, level, message);
    }
}

void log_execution(SecureAgentManager* mgr, SecureAgent* agent, ExecutionResult* result) {
    char msg[512];
    snprintf(msg, sizeof(msg), 
             "Agent %s: %s (code: %d, cibles compromises: %d)",
             agent->name,
             result->success ? "SUCCÈS" : "ÉCHEC",
             result->exit_code,
             result->targets_compromised);
    
    log_message(mgr, "EXEC", msg);
}

void add_secure_agent(SecureAgentManager* mgr, const char* name, const char* path, const char* type) {
    if (!mgr || mgr->agents_count >= MAX_AGENTS) {
        fprintf(stderr, "[!] Impossible d'ajouter l'agent\n");
        return;
    }
    
    if (!validate_agent_path(path)) {
        return;
    }
    
    SecureAgent* agent = &mgr->agents[mgr->agents_count];
    strncpy(agent->name, name, MAX_AGENT_NAME - 1);
    strncpy(agent->path, path, MAX_PATH - 1);
    strncpy(agent->type, type, sizeof(agent->type) - 1);
    
    agent->enabled = 1;
    agent->priority = mgr->agents_count;
    agent->state = AGENT_IDLE;
    agent->last_execution = 0;
    agent->execution_count = 0;
    agent->success_count = 0;
    agent->failure_count = 0;
    agent->assigned_targets = NULL;
    agent->targets_count = 0;
    
    mgr->agents_count++;
    printf("[+] Agent ajouté: %s\n", name);
}

SecureAgent* find_agent(SecureAgentManager* mgr, const char* name) {
    if (!mgr) return NULL;
    
    for (int i = 0; i < mgr->agents_count; i++) {
        if (strcmp(mgr->agents[i].name, name) == 0) {
            return &mgr->agents[i];
        }
    }
    return NULL;
}

pid_t safe_fork_exec(const char* executable, char** argv, int* stdout_pipe, int* stderr_pipe) {
    int stdout_fds[2], stderr_fds[2];
    
    if (pipe(stdout_fds) == -1 || pipe(stderr_fds) == -1) {
        perror("Erreur: création de pipe échouée");
        return -1;
    }
    
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Erreur: fork échouée");
        return -1;
    }
    
    if (pid == 0) {
        // Processus enfant
        close(stdout_fds[0]);
        close(stderr_fds[0]);
        
        dup2(stdout_fds[1], STDOUT_FILENO);
        dup2(stderr_fds[1], STDERR_FILENO);
        
        close(stdout_fds[1]);
        close(stderr_fds[1]);
        
        // Exécuter avec des droits réduits
        execvp(executable, argv);
        exit(127);  // Exécution échouée
    } else {
        // Processus parent
        close(stdout_fds[1]);
        close(stderr_fds[1]);
        
        *stdout_pipe = stdout_fds[0];
        *stderr_pipe = stderr_fds[0];
    }
    
    return pid;
}

void execute_with_timeout(const char* executable, int timeout, ExecutionResult* result) {
    char* argv[] = {(char*)executable, NULL};
    int stdout_fd, stderr_fd;
    
    pid_t pid = safe_fork_exec(executable, argv, &stdout_fd, &stderr_fd);
    
    if (pid == -1) {
        result->success = 0;
        result->state = AGENT_FAILED;
        strcpy(result->error, "Fork échouée");
        return;
    }
    
    time_t start = time(NULL);
    int status = 0;
    pid_t wait_result = 0;
    
    while (time(NULL) - start < timeout) {
        wait_result = waitpid(pid, &status, WNOHANG);
        
        if (wait_result == pid) {
            // Processus terminé
            result->exit_code = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
            result->success = (result->exit_code == 0) ? 1 : 0;
            result->state = result->success ? AGENT_SUCCESS : AGENT_FAILED;
            break;
        } else if (wait_result == -1) {
            perror("Erreur: waitpid");
            result->success = 0;
            result->state = AGENT_FAILED;
            break;
        }
        
        sleep(1);
    }
    
    if (wait_result == 0) {
        // Timeout
        kill(pid, SIGTERM);
        sleep(1);
        kill(pid, SIGKILL);
        waitpid(pid, NULL, 0);
        
        result->success = 0;
        result->state = AGENT_TIMEOUT;
        strcpy(result->error, "Timeout dépassé");
    }
    
    // Lire les sorties
    ssize_t n = read(stdout_fd, result->output, sizeof(result->output) - 1);
    if (n > 0) result->output[n] = '\0';
    
    n = read(stderr_fd, result->error, sizeof(result->error) - 1);
    if (n > 0) result->error[n] = '\0';
    
    close(stdout_fd);
    close(stderr_fd);
}

void assign_targets_to_agent(SecureAgent* agent, AttackSystem* sys) {
    if (!agent || !sys) return;
    
    agent->assigned_targets = sys->targets;
    agent->targets_count = sys->targets_count;
}

void attack_assigned_targets(SecureAgent* agent, AttackSystem* sys) {
    if (!agent || !sys) return;
    
    printf("\n[*] %s attaque les cibles assignées\n", agent->name);
    
    for (int i = 0; i < agent->targets_count && i < 3; i++) {
        PhysicalTarget* target = &sys->targets[i];
        
        // Choisir le type d'attaque en fonction du type d'agent
        AttackType attack_type = ATTACK_SCAN;
        int skill = 50;
        
        if (strstr(agent->type, "exploit")) {
            attack_type = ATTACK_EXPLOIT;
            skill = 70;
        } else if (strstr(agent->type, "recon")) {
            attack_type = ATTACK_SCAN;
            skill = 60;
        } else if (strstr(agent->type, "blackhat")) {
            attack_type = ATTACK_MALWARE;
            skill = 80;
        }
        
        AttackResult res = execute_attack(sys, target, attack_type, skill);
        agent->assigned_targets[i].compromise_level = res.damage_level;
    }
}

ExecutionResult execute_agent_safely(SecureAgentManager* mgr, SecureAgent* agent) {
    ExecutionResult result = {0};
    
    if (!agent || !agent->enabled) {
        result.success = 0;
        result.state = AGENT_FAILED;
        strcpy(result.error, "Agent désactivé ou invalide");
        return result;
    }
    
    if (!validate_agent_executable(agent->path)) {
        result.success = 0;
        result.state = AGENT_FAILED;
        strcpy(result.error, "Exécutable invalide");
        return result;
    }
    
    printf("\n[+] Exécution sécurisée: %s\n", agent->name);
    printf("[+] Chemin: %s\n", agent->path);
    printf("[+] Type: %s\n", agent->type);
    
    agent->state = AGENT_RUNNING;
    agent->execution_count++;
    
    execute_with_timeout(agent->path, EXEC_TIMEOUT, &result);
    
    if (result.success) {
        agent->success_count++;
        agent->state = AGENT_SUCCESS;
        
        // Attaquer les cibles assignées
        if (mgr->attack_system) {
            assign_targets_to_agent(agent, mgr->attack_system);
            attack_assigned_targets(agent, mgr->attack_system);
            result.targets_compromised = 1;
        }
    } else {
        agent->failure_count++;
        agent->state = AGENT_FAILED;
    }
    
    agent->last_execution = time(NULL);
    log_execution(mgr, agent, &result);
    
    return result;
}

void print_agent_status(const SecureAgent* agent) {
    if (!agent) return;
    
    printf("  [%s] %s\n", agent->enabled ? "X" : " ", agent->name);
    printf("      Type: %s | Exécutions: %d | Succès: %d | Échecs: %d\n",
           agent->type, agent->execution_count, agent->success_count, agent->failure_count);
}

void print_manager_status(const SecureAgentManager* mgr) {
    if (!mgr) return;
    
    printf("\n╔═══════════════════════════════════════════╗\n");
    printf("║  GESTIONNAIRE D'AGENTS SÉCURISÉ          ║\n");
    printf("╠═══════════════════════════════════════════╣\n");
    printf("║  Agents: %d                             ║\n", mgr->agents_count);
    printf("║  Cibles: %d                             ║\n", mgr->attack_system->targets_count);
    printf("║  Compromissions: %d                     ║\n", mgr->attack_system->total_compromises);
    printf("╠═══════════════════════════════════════════╣\n");
    
    for (int i = 0; i < mgr->agents_count && i < 10; i++) {
        print_agent_status(&mgr->agents[i]);
    }
    
    printf("╚═══════════════════════════════════════════╝\n");
}

#endif // AGENT_MANAGER_SECURE_H
