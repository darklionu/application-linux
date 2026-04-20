/*
 * HDRA - SSH Hardening & Defense Red Team Agent
 * =============================================
 * Audit et exploitation SSH
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libssh/libssh.h>
#include <libssh/sftp.h>

#define GRN "\033[1;32m"
#define RED "\033[1;31m"
#define YEL "\033[1;33m"
#define CYN "\033[1;36m"
#define RST "\033[0m"

typedef struct {
    char host[256];
    int port;
    char username[128];
    char password[128];
} SSHTarget;

/**
 * Test connexion SSH basique
 */
int ssh_connect_test(SSHTarget *target) {
    ssh_session session = ssh_new();
    if (!session) {
        fprintf(stderr, RED "[-]" RST " Erreur création session\n");
        return -1;
    }
    
    ssh_options_set(session, SSH_OPTIONS_HOST, target->host);
    ssh_options_set(session, SSH_OPTIONS_PORT, &target->port);
    ssh_options_set(session, SSH_OPTIONS_USER, target->username);
    ssh_options_set(session, SSH_OPTIONS_TIMEOUT, 5);
    ssh_options_set(session, SSH_OPTIONS_COMPRESSION, "yes");
    
    /* Tentative connexion */
    if (ssh_connect(session) != SSH_OK) {
        fprintf(stderr, YEL "[!]" RST " Connexion échouée: %s\n", 
                ssh_get_error(session));
        ssh_free(session);
        return -1;
    }
    
    printf(GRN "[+]" RST " SSH accessible sur %s:%d\n", 
           target->host, target->port);
    
    /* Authentification */
    int auth = ssh_userauth_password(session, target->username, target->password);
    if (auth == SSH_AUTH_SUCCESS) {
        printf(GRN "[+]" RST " Authentication réussie: %s\n", target->username);
        ssh_disconnect(session);
        ssh_free(session);
        return 1;
    } else {
        printf(YEL "[!]" RST " Auth échouée: %s\n", ssh_get_error(session));
    }
    
    ssh_disconnect(session);
    ssh_free(session);
    return 0;
}

/**
 * Extrait infos serveur SSH
 */
void ssh_banner_grab(const char *host, int port) {
    printf(GRN "[+]" RST " Extraction bannière SSH: %s:%d\n\n", host, port);
    
    char cmd[512];
    snprintf(cmd, sizeof(cmd), 
             "echo 'QUIT' | nc -w 1 %s %d 2>/dev/null || "
             "timeout 1 ssh -o StrictHostKeyChecking=no %s -p %d 2>&1 | head -5",
             host, port, host, port);
    
    system(cmd);
}

/**
 * Test clés SSH publiables
 */
void ssh_key_audit(SSHTarget *target) {
    printf(GRN "[+]" RST " Audit clés SSH pour %s\n\n", target->host);
    
    printf(CYN "Clés SSH testées:\n" RST);
    
    const char *keys[] = {
        "~/.ssh/id_rsa",
        "~/.ssh/id_ecdsa",
        "~/.ssh/id_ed25519",
        "~/.ssh/id_dsa",
        NULL
    };
    
    char cmd[512];
    for (int i = 0; keys[i]; i++) {
        snprintf(cmd, sizeof(cmd),
                 "if [ -f %s ]; then ssh-keyscan -t rsa %s:%d 2>/dev/null | "
                 "ssh-keygen -l -f - 2>/dev/null && echo '[Found] %s'; fi",
                 keys[i], target->host, target->port, keys[i]);
        system(cmd);
    }
}

/**
 * Brute force SSH basic
 */
void ssh_brute_force(SSHTarget *target, const char *wordlist) {
    printf(GRN "[+]" RST " Brute force SSH %s (wordlist: %s)\n\n", 
           target->host, wordlist);
    
    char cmd[1024];
    snprintf(cmd, sizeof(cmd),
             "sshpass -x $(cat %s | head -1 | cut -d':' -f2) "
             "ssh -o StrictHostKeyChecking=no -o ConnectTimeout=2 "
             "%s@%s -p %d 'echo OK' 2>/dev/null",
             wordlist, target->username, target->host, target->port);
    
    printf(YEL "[!]" RST " Utilisez hydra ou medusa pour brute force professionnel\n");
    printf("Commande suggérée:\n");
    printf("  hydra -L users.txt -P %s ssh://%s\n", wordlist, target->host);
}

/**
 * Main
 */
int main(int argc, char *argv[]) {
    printf(CYN "╔════════════════════════════════════════╗\n");
    printf("║   HDRA - SSH Hardening & Defense RA   ║\n");
    printf("╚════════════════════════════════════════╝\n\n" RST);
    
    if (argc < 2) {
        printf("Usage: %s <action> <args...>\n\n", argv[0]);
        printf("Actions:\n");
        printf("  banner <host> [port]      - Récupère la bannière SSH\n");
        printf("  test <host> <user> <pass> - Test connexion\n");
        printf("  keyaudit <host> <user>    - Audit clés SSH\n");
        printf("  brute <host> <user> <wordlist> - Brute force\n");
        return 1;
    }
    
    const char *action = argv[1];
    
    if (strcmp(action, "banner") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: hdra banner <host> [port]\n"); return 1; }
        int port = (argc > 3) ? atoi(argv[3]) : 22;
        ssh_banner_grab(argv[2], port);
        
    } else if (strcmp(action, "test") == 0) {
        if (argc < 5) { fprintf(stderr, "Usage: hdra test <host> <user> <pass>\n"); return 1; }
        SSHTarget target = {0};
        strncpy(target.host, argv[2], sizeof(target.host)-1);
        strncpy(target.username, argv[3], sizeof(target.username)-1);
        strncpy(target.password, argv[4], sizeof(target.password)-1);
        target.port = (argc > 5) ? atoi(argv[5]) : 22;
        ssh_connect_test(&target);
        
    } else if (strcmp(action, "keyaudit") == 0) {
        if (argc < 4) { fprintf(stderr, "Usage: hdra keyaudit <host> <user>\n"); return 1; }
        SSHTarget target = {0};
        strncpy(target.host, argv[2], sizeof(target.host)-1);
        strncpy(target.username, argv[3], sizeof(target.username)-1);
        target.port = (argc > 4) ? atoi(argv[4]) : 22;
        ssh_key_audit(&target);
        
    } else if (strcmp(action, "brute") == 0) {
        if (argc < 5) { fprintf(stderr, "Usage: hdra brute <host> <user> <wordlist>\n"); return 1; }
        SSHTarget target = {0};
        strncpy(target.host, argv[2], sizeof(target.host)-1);
        strncpy(target.username, argv[3], sizeof(target.username)-1);
        target.port = (argc > 5) ? atoi(argv[5]) : 22;
        ssh_brute_force(&target, argv[4]);
        
    } else {
        fprintf(stderr, RED "[-]" RST " Action inconnue: %s\n", action);
        return 1;
    }
    
    printf("\n" GRN "[✓]" RST " Opération terminée.\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <libssh/libssh.h>


