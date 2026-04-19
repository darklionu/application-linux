/**
 * Système de SCAN DE PORT RÉEL
 * Teste réellement les ports ouverts avec des connexions socket
 * Pas de simulation - connexions TCP réelles
 */

#ifndef REAL_PORT_SCANNER_H
#define REAL_PORT_SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/select.h>

#define MAX_PORTS_TO_SCAN 1000
#define SCAN_TIMEOUT 2  // 2 secondes par port

/**
 * Résultat d'un scan de port
 */
typedef struct {
    int port_number;
    int is_open;
    int port_state;  // 0=closed, 1=open, 2=filtered
    char service_name[64];
    int response_time_ms;
} PortScanResult;

/**
 * Mapping des ports courants et leurs services
 */
typedef struct {
    int port;
    const char* service;
    const char* protocol;
} ServiceMapping;

// Services courants
static const ServiceMapping COMMON_SERVICES[] = {
    {21, "FTP", "tcp"},
    {22, "SSH", "tcp"},
    {23, "Telnet", "tcp"},
    {25, "SMTP", "tcp"},
    {53, "DNS", "udp"},
    {80, "HTTP", "tcp"},
    {110, "POP3", "tcp"},
    {143, "IMAP", "tcp"},
    {443, "HTTPS", "tcp"},
    {445, "SMB", "tcp"},
    {3306, "MySQL", "tcp"},
    {3389, "RDP", "tcp"},
    {5432, "PostgreSQL", "tcp"},
    {5985, "WinRM", "tcp"},
    {8080, "HTTP-Alt", "tcp"},
    {8443, "HTTPS-Alt", "tcp"},
    {27017, "MongoDB", "tcp"},
    {6379, "Redis", "tcp"},
    {9200, "Elasticsearch", "tcp"},
    {0, NULL, NULL}  // Fin du tableau
};

// ============================================================================
// Prototypes de fonction
// ============================================================================

const char* get_service_name(int port);
int scan_single_port_tcp(const char* target_ip, int port, int* response_time);
void scan_ports_range(const char* target_ip, int start_port, int end_port, 
                      PortScanResult* results, int* results_count);
void print_scan_results(const PortScanResult* results, int count);
void perform_full_scan(const char* target_ip);

// ============================================================================
// Implémentation
// ============================================================================

const char* get_service_name(int port) {
    for (int i = 0; COMMON_SERVICES[i].port != 0; i++) {
        if (COMMON_SERVICES[i].port == port) {
            return COMMON_SERVICES[i].service;
        }
    }
    return "Unknown";
}

/**
 * Scan RÉEL d'un port TCP avec timeout
 * Retourne: 1 si ouvert, 0 si fermé, -1 si erreur
 */
int scan_single_port_tcp(const char* target_ip, int port, int* response_time) {
    if (!target_ip || !response_time) return -1;
    
    struct sockaddr_in addr;
    int sock = -1;
    int result = 0;
    
    clock_t start = clock();
    
    // Créer un socket TCP
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0) {
        perror("socket()");
        return -1;
    }
    
    // Mettre le socket en non-bloquant
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    
    // Préparer l'adresse cible
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    // Convertir l'IP
    if (inet_pton(AF_INET, target_ip, &addr.sin_addr) <= 0) {
        fprintf(stderr, "[!] Adresse IP invalide: %s\n", target_ip);
        close(sock);
        return -1;
    }
    
    // Essayer de se connecter
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        // Vérifier l'erreur
        if (errno == EINPROGRESS || errno == EWOULDBLOCK) {
            // La connexion est en cours, utiliser select() pour le timeout
            fd_set writefd;
            struct timeval tv;
            
            FD_ZERO(&writefd);
            FD_SET(sock, &writefd);
            
            tv.tv_sec = SCAN_TIMEOUT;
            tv.tv_usec = 0;
            
            // Attendre que le socket soit prêt en écriture
            int select_result = select(sock + 1, NULL, &writefd, NULL, &tv);
            
            if (select_result > 0) {
                // Vérifier si la connexion a réussi
                int error = 0;
                socklen_t len = sizeof(error);
                
                if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len) == 0) {
                    if (error == 0) {
                        // Port OUVERT!
                        result = 1;
                    } else {
                        // Port fermé ou filtré
                        result = 0;
                    }
                }
            } else if (select_result == 0) {
                // Timeout - port probablement filtré
                result = 0;
            } else {
                result = -1;
            }
        } else if (errno == ECONNREFUSED) {
            // Port fermé (connexion refusée)
            result = 0;
        } else {
            // Autres erreurs
            result = -1;
        }
    } else {
        // Connexion réussie immédiatement - port OUVERT!
        result = 1;
    }
    
    // Calculer le temps de réponse
    clock_t end = clock();
    *response_time = (int)((double)(end - start) / CLOCKS_PER_SEC * 1000);
    
    // Fermer le socket
    close(sock);
    
    return result;
}

/**
 * Scanner une plage de ports
 */
void scan_ports_range(const char* target_ip, int start_port, int end_port, 
                      PortScanResult* results, int* results_count) {
    if (!target_ip || !results || !results_count) return;
    
    *results_count = 0;
    
    printf("\n[*] Scan RÉEL des ports de %d à %d sur %s\n", 
           start_port, end_port, target_ip);
    printf("[*] Cela peut prendre quelques secondes...\n\n");
    
    for (int port = start_port; port <= end_port && *results_count < MAX_PORTS_TO_SCAN; port++) {
        printf("[+] Scan du port %d...", port);
        fflush(stdout);
        
        int response_time = 0;
        int is_open = scan_single_port_tcp(target_ip, port, &response_time);
        
        if (is_open == 1) {
            // Port ouvert!
            PortScanResult* result = &results[*results_count];
            result->port_number = port;
            result->is_open = 1;
            result->port_state = 1;
            result->response_time_ms = response_time;
            strncpy(result->service_name, get_service_name(port), sizeof(result->service_name) - 1);
            
            (*results_count)++;
            
            printf(" [OUVERT] %s (%dms)\n", result->service_name, response_time);
        } else if (is_open == 0) {
            printf(" [fermé]\n");
        } else {
            printf(" [erreur]\n");
        }
    }
    
    printf("\n[+] Scan terminé. %d port(s) ouvert(s) trouvé(s)\n", *results_count);
}

/**
 * Afficher les résultats du scan
 */
void print_scan_results(const PortScanResult* results, int count) {
    if (!results || count == 0) {
        printf("\n[!] Aucun port ouvert trouvé\n");
        return;
    }
    
    printf("\n╔════════════════════════════════════════════╗\n");
    printf("║         RÉSULTATS DU SCAN RÉEL            ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║ Port    │ Service         │ Temps    │ État ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < count; i++) {
        printf("║ %-7d │ %-15s │ %4dms   │ OPEN ║\n",
               results[i].port_number,
               results[i].service_name,
               results[i].response_time_ms);
    }
    
    printf("╚════════════════════════════════════════════╝\n");
}

/**
 * Scanner complet avec statistiques
 */
void perform_full_scan(const char* target_ip) {
    if (!target_ip) return;
    
    printf("\n╔════════════════════════════════════════════════════════╗\n");
    printf("║           SCAN DE PORT RÉEL - NMAP-LIKE              ║\n");
    printf("║  Utilise des connexions TCP réelles pour scanner      ║\n");
    printf("╚════════════════════════════════════════════════════════╝\n");
    
    printf("\n[*] Cible: %s\n", target_ip);
    printf("[*] Ports à scanner: 1-65535 (par défaut: 1-10000)\n");
    printf("[*] Mode: TCP Connect Scan (pas de privilèges root)\n");
    printf("[*] Timeout par port: %d secondes\n\n", SCAN_TIMEOUT);
    
    PortScanResult results[MAX_PORTS_TO_SCAN];
    int results_count = 0;
    
    // Scanner les ports courants d'abord (1-10000)
    clock_t start_time = clock();
    
    scan_ports_range(target_ip, 1, 10000, results, &results_count);
    
    clock_t end_time = clock();
    double scan_duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    
    // Afficher les résultats
    print_scan_results(results, results_count);
    
    printf("\n[*] Statistiques:\n");
    printf("    - Ports scannés: 10000\n");
    printf("    - Ports ouverts: %d\n", results_count);
    printf("    - Durée totale: %.2f secondes\n", scan_duration);
    printf("    - Vitesse moyenne: %.0f ports/sec\n", 10000 / scan_duration);
    
    // Afficher les services trouvés
    if (results_count > 0) {
        printf("\n[+] Services actifs découverts:\n");
        for (int i = 0; i < results_count; i++) {
            printf("    - Port %d/%s (%s)\n", 
                   results[i].port_number,
                   results[i].service_name,
                   "tcp");
        }
    }
}

/**
 * Scanner une cible avec ports préférés
 */
void quick_scan_preferred_ports(const char* target_ip) {
    if (!target_ip) return;
    
    printf("\n[*] Scan RAPIDE des ports préférés sur %s\n", target_ip);
    printf("[*] Utilise des connexions TCP réelles\n\n");
    
    // Ports à scanner en priorité
    int preferred_ports[] = {
        21, 22, 23, 25, 53, 80, 110, 143, 389, 443, 
        445, 465, 587, 993, 995, 1433, 3306, 3389, 5432, 
        5985, 8080, 8443, 8888, 27017, 6379, 9200, 0
    };
    
    PortScanResult results[MAX_PORTS_TO_SCAN];
    int results_count = 0;
    
    printf("[*] Scanning %d ports préférés...\n\n", 26);
    
    for (int i = 0; preferred_ports[i] != 0 && results_count < MAX_PORTS_TO_SCAN; i++) {
        int port = preferred_ports[i];
        int response_time = 0;
        
        printf("[+] Port %d (%s)... ", port, get_service_name(port));
        fflush(stdout);
        
        int is_open = scan_single_port_tcp(target_ip, port, &response_time);
        
        if (is_open == 1) {
            printf("[OUVERT] (%dms)\n", response_time);
            
            results[results_count].port_number = port;
            results[results_count].is_open = 1;
            results[results_count].response_time_ms = response_time;
            strncpy(results[results_count].service_name, get_service_name(port), 63);
            
            results_count++;
        } else if (is_open == 0) {
            printf("[fermé]\n");
        } else {
            printf("[erreur]\n");
        }
    }
    
    printf("\n");
    print_scan_results(results, results_count);
}

#endif // REAL_PORT_SCANNER_H
