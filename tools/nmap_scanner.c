/*
 * NMAP_SCANNER - Port Scanning & Service Detection
 * ==================================================
 * Intégration de logique nmap-like pour scan réseau
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>

#define GRN "\033[1;32m"
#define RED "\033[1;31m"
#define YEL "\033[1;33m"
#define CYN "\033[1;36m"
#define RST "\033[0m"

#define TIMEOUT_MS 300
#define MAX_HOSTS 256

typedef struct {
    char status[16];
    int port;
    char service[64];
} PortInfo;

/**
 * Scanner TCP connect (syn) sur un port
 */
int scan_port_tcp(const char *ip, int port) {
    int sock;
    struct sockaddr_in target;
    struct timeval timeout;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 0;

    timeout.tv_sec = 0;
    timeout.tv_usec = TIMEOUT_MS * 1000; /* 300ms */

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

    target.sin_family      = AF_INET;
    target.sin_port        = htons(port);
    target.sin_addr.s_addr = inet_addr(ip);

    int result = connect(sock, (struct sockaddr*)&target, sizeof(target));
    close(sock);

    return (result == 0) ? 1 : 0;
}

/**
 * Retourne le nom du service pour un port
 */
const char *get_service_name(int port) {
    struct servent *se = getservbyport(htons(port), "tcp");
    if (se) return se->s_name;
    
    /* Services communs */
    switch (port) {
        case 21:   return "ftp";
        case 22:   return "ssh";
        case 23:   return "telnet";
        case 25:   return "smtp";
        case 53:   return "dns";
        case 80:   return "http";
        case 110:  return "pop3";
        case 143:  return "imap";
        case 443:  return "https";
        case 445:  return "smb";
        case 3306: return "mysql";
        case 3389: return "rdp";
        case 5432: return "postgresql";
        case 5900: return "vnc";
        case 6379: return "redis";
        case 8080: return "http-alt";
        case 8443: return "https-alt";
        case 9200: return "elasticsearch";
        default:   return "unknown";
    }
}

/**
 * Scan une plage de ports sur un hôte
 */
void scan_host(const char *ip, int port_min, int port_max) {
    printf(CYN "Scan de %s (ports %d-%d)...\n\n" RST, ip, port_min, port_max);
    
    int open_count = 0;
    
    for (int port = port_min; port <= port_max; port++) {
        if (scan_port_tcp(ip, port)) {
            const char *service = get_service_name(port);
            printf(GRN "[+] %d/tcp\t" RST "OPEN\t%s\n", port, service);
            open_count++;
        }
        
        /* Affiche la progression tous les 50 ports */
        if (port % 50 == 0) {
            printf(YEL "[*] Progression: %d/%d\r" RST, port - port_min, port_max - port_min);
            fflush(stdout);
        }
    }
    
    printf("\n" GRN "[✓]" RST " Scan terminé. %d port(s) ouverts.\n\n", open_count);
}

/**
 * Scan rapide des ports courants
 */
void quick_scan(const char *ip) {
    const int ports[] = {
        21, 22, 23, 25, 53, 80, 110, 143, 443, 445, 
        3306, 3389, 5432, 5900, 6379, 8080, 8443,
        0 /* sentinel */
    };
    
    printf(CYN "Quick scan de %s (ports courants)...\n\n" RST, ip);
    
    int open_count = 0;
    for (int i = 0; ports[i]; i++) {
        int port = ports[i];
        if (scan_port_tcp(ip, port)) {
            const char *service = get_service_name(port);
            printf(GRN "[+] %d/tcp\t" RST "OPEN\t%s\n", port, service);
            open_count++;
        }
    }
    
    printf("\n" GRN "[✓]" RST " Quick scan terminé. %d port(s) ouverts.\n\n", open_count);
}

/**
 * Scan d'une plage d'hôtes (classe C)
 */
void network_scan(const char *network) {
    printf(CYN "Scan réseau %s\n\n" RST, network);
    
    /* Parse simple XXX.XXX.XXX.0/24 */
    char base_ip[16];
    strncpy(base_ip, network, sizeof(base_ip)-1);
    
    /* Remplace le dernier octet par 1 */
    char *last_dot = strrchr(base_ip, '.');
    if (last_dot) *last_dot = '\0';
    
    int alive = 0;
    
    for (int i = 1; i < 254; i++) {
        char ip[32];
        snprintf(ip, sizeof(ip), "%s.%d", base_ip, i);
        
        /* Teste ports courants pour déterminer si hôte actif */
        int ports_to_check[] = {22, 80, 443, 0};
        int found = 0;
        
        for (int j = 0; ports_to_check[j]; j++) {
            if (scan_port_tcp(ip, ports_to_check[j])) {
                found = 1;
                break;
            }
        }
        
        if (found) {
            printf(GRN "[+]" RST " Hôte actif: %s\n", ip);
            alive++;
        }
        
        if (i % 50 == 0) {
            printf(YEL "[*] Progression: %d/253\r" RST, i);
            fflush(stdout);
        }
    }
    
    printf("\n" GRN "[✓]" RST " Scan réseau terminé. %d hôte(s) actif(s).\n\n", alive);
}

/**
 * Résolution DNS
 */
void resolve_host(const char *hostname) {
    struct addrinfo hints, *res, *p;
    char ip[INET6_ADDRSTRLEN];
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    
    printf(GRN "[+]" RST " Résolution DNS de %s:\n\n", hostname);
    
    if (getaddrinfo(hostname, NULL, &hints, &res) != 0) {
        printf(RED "[-]" RST " Résolution échouée\n");
        return;
    }
    
    for (p = res; p; p = p->ai_next) {
        void *addr;
        const char *ipver;
        
        if (p->ai_family == AF_INET) {
            addr = &((struct sockaddr_in *)p->ai_addr)->sin_addr;
            ipver = "IPv4";
        } else {
            addr = &((struct sockaddr_in6 *)p->ai_addr)->sin6_addr;
            ipver = "IPv6";
        }
        
        inet_ntop(p->ai_family, addr, ip, sizeof ip);
        printf("  %s: %s\n", ipver, ip);
    }
    
    freeaddrinfo(res);
    printf("\n");
}

/**
 * Main
 */
int main(int argc, char *argv[]) {
    printf(CYN "╔═══════════════════════════════════════╗\n");
    printf("║   NMAP_SCANNER - Network Scanner    ║\n");
    printf("╚═══════════════════════════════════════╝\n\n" RST);
    
    if (argc < 2) {
        printf("Usage: %s <commande> <args...>\n\n", argv[0]);
        printf("Commandes:\n");
        printf("  resolve <hostname>              - Résolution DNS\n");
        printf("  quick <ip>                      - Scan ports courants\n");
        printf("  scan <ip> <port_min> <port_max> - Scan plage ports\n");
        printf("  network <network>               - Scan réseau (ex: 192.168.1.0/24)\n");
        printf("\nExemples:\n");
        printf("  %s quick 192.168.1.1\n", argv[0]);
        printf("  %s scan 192.168.1.1 1 65535\n", argv[0]);
        printf("  %s network 192.168.1.0/24\n", argv[0]);
        return 1;
    }
    
    const char *cmd = argv[1];
    
    if (strcmp(cmd, "resolve") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s resolve <hostname>\n", argv[0]); return 1; }
        resolve_host(argv[2]);
        
    } else if (strcmp(cmd, "quick") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s quick <ip>\n", argv[0]); return 1; }
        quick_scan(argv[2]);
        
    } else if (strcmp(cmd, "scan") == 0) {
        if (argc < 5) { fprintf(stderr, "Usage: %s scan <ip> <port_min> <port_max>\n", argv[0]); return 1; }
        scan_host(argv[2], atoi(argv[3]), atoi(argv[4]));
        
    } else if (strcmp(cmd, "network") == 0) {
        if (argc < 3) { fprintf(stderr, "Usage: %s network <network>\n", argv[0]); return 1; }
        network_scan(argv[2]);
        
    } else {
        fprintf(stderr, RED "[-]" RST " Commande inconnue: %s\n", cmd);
        return 1;
    }
    
    return 0;
}