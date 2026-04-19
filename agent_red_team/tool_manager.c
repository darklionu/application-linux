#include "agent_types.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

Technique all_techniques[MAX_TECHNIQUES];
int techniques_count = 0;

// Charger les outils depuis un dossier
int load_tools(Agent *a, const char *tools_dir) {
    DIR *dir = opendir(tools_dir);
    if (!dir) {
        perror("Erreur : impossible d'ouvrir le dossier outils");
        return 0;
    }

    struct dirent *entry;
    a->tools_count = 0;

    while ((entry = readdir(dir)) && a->tools_count < MAX_TOOLS) {
        // Ignorer les entrées spéciales
        if (entry->d_name[0] == '.') continue;
        
        // Ignorer les liens symboliques cassés
        if (strcmp(entry->d_name, "..") == 0) continue;
        
        // Vérifier la longueur du nom
        if (strlen(entry->d_name) >= NAME_LEN) {
            fprintf(stderr, "[!] Nom d'outil trop long, ignoré\n");
            continue;
        }
        
        Tool *t = &a->tools[a->tools_count];
        strncpy(t->name, entry->d_name, NAME_LEN - 1);
        t->name[NAME_LEN - 1] = '\0';  // Assurer la null-termination
        snprintf(t->path, PATH_LEN, "%s/%s", tools_dir, entry->d_name);
        t->path[PATH_LEN - 1] = '\0';  // Assurer la null-termination
        t->phase = (a->tools_count % 5);
        t->enabled = 0;
        a->tools_count++;
    }

    closedir(dir);
    printf("[*] %d outil(s) detecté(s)\n", a->tools_count);
    return a->tools_count;
}

// Charger les techniques depuis un fichier
int load_techniques_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("[!] Fichier techniques non trouvé, utilisation des techniques par défaut\n");
        return 0;
    }

    char line[512];
    techniques_count = 0;

    while (fgets(line, sizeof(line), file) && techniques_count < MAX_TECHNIQUES) {
        if (line[0] == '#' || line[0] == '\n') continue;
        
        line[strcspn(line, "\n")] = 0;
        
        char *token = strtok(line, "|");
        if (!token) continue;

        Technique *t = &all_techniques[techniques_count];
        strncpy(t->name, token, NAME_LEN - 1);
        strncpy(t->tool_name, (token = strtok(NULL, "|")) ? token : "", NAME_LEN - 1);
        t->phase = atoi((token = strtok(NULL, "|")) ? token : "0");
        t->success_rate = atoi((token = strtok(NULL, "|")) ? token : "75");
        t->unlocked = 0;

        techniques_count++;
    }

    fclose(file);
    return techniques_count;
}

// Créer des techniques par défaut
void create_default_techniques() {
    techniques_count = 0;
    
    Technique defaults[] = {
        { "Ping Test",           "nmap",    PHASE_RECON,    90,  0 },
        { "DNS Lookup",          "nmap",    PHASE_RECON,    85,  0 },
        { "Port Enumeration",    "nmap",    PHASE_SCAN,     80,  0 },
        { "Service Scan",        "nmap",    PHASE_SCAN,     75,  0 },
        { "Credential Crack",    "john",    PHASE_EXPLOIT,  45,  0 },
        { "Hash Brute Force",    "john",    PHASE_EXPLOIT,  40,  0 },
        { "Banner Grab",         "nmap",    PHASE_SCAN,     80,  0 },
        { "Vulnerability Scan",  "nmap",    PHASE_EXPLOIT,  55,  0 },
        { "Custom Attack",       "hdra",    PHASE_EXPLOIT,  30,  0 },
        { "System Audit",        "outils",  PHASE_RECON,    70,  0 },
        { "Network Recon",       "nmap",    PHASE_RECON,    95,  0 },
    };

    for (int i = 0; i < (int)(sizeof(defaults) / sizeof(defaults[0])); i++) {
        all_techniques[i] = defaults[i];
        techniques_count++;
    }
}